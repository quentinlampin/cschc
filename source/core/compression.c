#include "core/compression.h"
#include "parser/parser.h"
#include "parser/yang_handler.h"
#include "utils/binary.h"

/* ********************************************************************** */

// or int
static void __variable_length_field(uint8_t*     schc_packet,
                                    const size_t schc_packet_byte_len,
                                    int* bit_pos, const int len) {
  int     variable_length_len;
  uint8_t variable_length_residue[BYTE_LENGTH(len)];

  if (len < 15) {
    variable_length_len        = 4;
    variable_length_residue[0] = len;
  } else if (len < 255) {
    variable_length_len        = 12;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = len;
  } else {
    variable_length_len        = 28;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = 0xff;
    variable_length_residue[2] = (uint8_t) (len >> 8);
    variable_length_residue[3] = (uint8_t) (len & 0xff);
  }

  add_bits_to_buffer(schc_packet, variable_length_residue, variable_length_len,
                     bit_pos, schc_packet_byte_len);
}

/* ********************************************************************** */

int compress(uint8_t* schc_packet, const size_t schc_packet_byte_len,
             const rule_desc_t rule_desc, const uint8_t* packet,
             const size_t packet_len, const uint8_t* context,
             const size_t context_len) {
  if (BYTE_LENGTH(packet_len) > schc_packet_byte_len) {
    return 0;
  }

  // schc packet variables
  int schc_packet_rule_len;
  int schc_bit_pos;
  int schc_bit_to_add;

  // parsing variables
  int        parsing_offset;
  uint8_t*   field_ptr;
  ipv6_hdr_t ipv6_hdr;
  udp_hdr_t  udp_hdr;

  // context variables
  int               rule_field_desc_offset, tv_offset;
  rule_field_desc_t rule_field_desc;

  // header parsing ; to do : create a function !
  parsing_offset = parse_ipv6_header(&ipv6_hdr, packet, packet_len);
  parsing_offset =
      parse_udp_header(&udp_hdr, packet, parsing_offset, packet_len);

  // schc packet
  schc_packet_rule_len = bits_counter(rule_desc.id);
  schc_bit_pos         = 0;
  // add the rule used at the beginning of the packet
  add_bits_to_buffer(schc_packet, context + rule_desc.offset,
                     schc_packet_rule_len, &schc_bit_pos, schc_packet_byte_len);

  for (int i = 0; i < sizeof(field_fid_mappings) / sizeof(field_fid_mapping_t);
       i++) {
    // FID in rule
    if (get_rule_field_desc_offset_from_fid(&rule_field_desc_offset,
                                            field_fid_mappings[i].fid, context,
                                            context_len)) {
      // to do : create a function !
      if (field_fid_mappings[i].protocol == PROTOCOL_IPV6) {
        field_ptr = get_ipv6_field(&ipv6_hdr, field_fid_mappings[i].fid);
      } else if (field_fid_mappings[i].protocol == PROTOCOL_UDP) {
        field_ptr = get_udp_field(&udp_hdr, field_fid_mappings[i].fid);
      }
      // ...

      tv_offset = get_rule_field_desc_info(
          &rule_field_desc, rule_field_desc_offset, context, context_len);

      switch (rule_field_desc.cda) {
        case CDA_LSB: {
          schc_bit_to_add = rule_field_desc.len - rule_field_desc.msb_len;
          uint8_t field_residue[BYTE_LENGTH(schc_bit_to_add)];

          if (CDA_least_significant_bits(field_residue, field_ptr,
                                         rule_field_desc.len,
                                         schc_bit_to_add)) {
            add_bits_to_buffer(schc_packet, field_residue, schc_bit_to_add,
                               &schc_bit_pos, schc_packet_byte_len);
            break;
          }
        }

        case CDA_MAPPING_SENT: {
          schc_bit_to_add = bits_counter(rule_field_desc.card_target_value);
          uint8_t field_residue[BYTE_LENGTH(schc_bit_to_add)];

          if (CDA_mapping_sent(field_residue, field_ptr, rule_field_desc.len,
                               tv_offset, context, context_len)) {
            add_bits_to_buffer(schc_packet, field_residue, schc_bit_to_add,
                               &schc_bit_pos, schc_packet_byte_len);
            break;
          }
        }

        case CDA_NOT_SENT:
          schc_bit_to_add = 0;
          break;

        case CDA_COMPUTE:
          schc_bit_to_add = 0;
          break;

        default:  // CDA_VALUE_SENT
          schc_bit_to_add = rule_field_desc.len;
          add_bits_to_buffer(schc_packet, field_ptr, schc_bit_to_add,
                             &schc_bit_pos, schc_packet_byte_len);
          break;
      }

      // Variable-Length fields
      if ((rule_field_desc.cda == CDA_LSB ||
           rule_field_desc.cda == CDA_VALUE_SENT) &&
          rule_field_desc.len == 0) {
        __variable_length_field(schc_packet, schc_packet_byte_len,
                                &schc_bit_pos, schc_bit_to_add);
      }

    } else {
      // FID not found meaning the corresponding rule field descriptor is not in
      // the current rule. So we perform a value_sent.
      schc_bit_to_add = rule_field_desc.len;
      add_bits_to_buffer(schc_packet, field_ptr, schc_bit_to_add, &schc_bit_pos,
                         schc_packet_byte_len);
    }
  }

  return 1;
}
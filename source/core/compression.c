#include "core/compression.h"
#include "parser/parser.h"
#include "utils/binary.h"
#include "utils/log.h"

#include <stdio.h>
#include <string.h>

/* ********************************************************************** */
/*                           Static definitions                           */
/* ********************************************************************** */

/**
 * @brief Add SCHC Rule ID at the beginning of the compression residue.
 *
 * @details RFC 8724, Section 5.1.
 *
 * @param comp_residue Pointer to the compression residue.
 * @param comp_residue_max_byte_len Maximum byte length of the compression
 * residue.
 * @param bit_position Pointer to the current position, from where to add the
 * rule_id value.
 * @param rule_id Rule ID to add.
 * @return The status code, 1 for success, otherwise 0.
 */
static int __add_schc_rule_id(uint8_t*     comp_residue,
                              const size_t comp_residue_max_byte_len,
                              int* bit_position, const uint8_t rule_id);

/**
 * @brief Handle compression according to the compression nature.
 *
 * @param schc_packet Pointer to the SCHC packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the SCHC packet.
 * @param packet Pointer to the packet that needs to be compressed.
 * @param packet_byte_len Byte length of the packet to compress.
 * @param rule_desc Pointer to the Rule Descriptor used to compress the packet.
 * @param context Pointer to the context used to perform compression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the SCHC packet.
 */
static size_t __compression_handler(uint8_t*           schc_packet,
                                    const size_t       schc_packet_max_byte_len,
                                    const uint8_t*     packet,
                                    const size_t       packet_byte_len,
                                    const rule_desc_t* rule_desc,
                                    const uint8_t*     context,
                                    const size_t       context_byte_len);

/**
 * @brief Perform compression on the given header using a specified Rule
 * Descriptor.
 *
 * @param comp_residue Pointer to the compression residue.
 * @param comp_residue_max_byte_len Maximum byte length of the compression
 * residue.
 * @param bit_position Pointer to the current bit position in the compression
 * residue.
 * @param header Pointer to the header to be compressed.
 * @param rule_desc Pointer to the Rule Descriptor used for compression.
 * @param context Pointer to the context used for compression.
 * @param context_byte_len Byte length of the context.
 * @return The status code, 1 for success, otherwise 0.
 */
static int __compression(uint8_t*     comp_residue,
                         const size_t comp_residue_max_byte_len,
                         int* bit_position, const header_t* header,
                         const rule_desc_t* rule_desc, const uint8_t* context,
                         const size_t context_byte_len);

/**
 * @brief Handle packets that do not require compression.
 *
 * @param schc_packet Pointer to the SCHC packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the SCHC packet.
 * @param bit_position Pointer to the current bit position in the SCHC packet.
 * @param packet Pointer to the packet that does not require compression.
 * @param packet_byte_len Byte length of the packet.
 * @param rule_desc Pointer to the Rule Descriptor used for processing.
 * @param context Pointer to the context used for processing.
 * @param context_byte_len Byte length of the context.
 * @return The status code, 1 for success, otherwise 0.
 */
static int __no_compression(uint8_t*     schc_packet,
                            const size_t schc_packet_max_byte_len,
                            int* bit_position, const uint8_t* packet,
                            const size_t       packet_byte_len,
                            const rule_desc_t* rule_desc,
                            const uint8_t*     context,
                            const size_t       context_byte_len);

/**
 * @brief Handle fields with variable length during compression, basically CoAP.
 *
 * @details RFC 8724, Section 7.4.2.
 *
 * @param comp_residue Pointer to the compression residue.
 * @param comp_residue_max_byte_len Maximum byte length of the compression
 * residue.
 * @param bit_position Pointer to the current bit position in the compression
 * residue.
 * @param variable_len Length of the variable length field.
 * @return The status code, 1 for success, otherwise 0.
 */
static int __variable_length_field(uint8_t*     comp_residue,
                                   const size_t comp_residue_max_byte_len,
                                   int* bit_position, const int variable_len);

/* ********************************************************************** */
/*                         Main compress function                         */
/* ********************************************************************** */

size_t compress(uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
                const uint8_t* packet, const size_t packet_byte_len,
                const rule_desc_t* rule_desc, const uint8_t* context,
                const size_t context_byte_len) {
  size_t schc_packet_byte_len;

  schc_packet_byte_len = __compression_handler(
      schc_packet, schc_packet_max_byte_len, packet, packet_byte_len, rule_desc,
      context, context_byte_len);

  return schc_packet_byte_len;
}

/* ********************************************************************** */
/*                            Static functions                            */
/* ********************************************************************** */

static int __add_schc_rule_id(uint8_t*     comp_residue,
                              const size_t comp_residue_max_byte_len,
                              int* bit_position, const uint8_t rule_id) {
  int schc_compression_status;
  int rule_len;

  rule_len = bits_counter(rule_id);

  schc_compression_status = add_byte_to_buffer(
      comp_residue, comp_residue_max_byte_len, bit_position, rule_id, rule_len);

  return schc_compression_status;
}

/* ********************************************************************** */

static size_t __compression_handler(uint8_t*           schc_packet,
                                    const size_t       schc_packet_max_byte_len,
                                    const uint8_t*     packet,
                                    const size_t       packet_byte_len,
                                    const rule_desc_t* rule_desc,
                                    const uint8_t*     context,
                                    const size_t       context_byte_len) {
  int schc_compression_status;
  int bit_position;  // usefull to add field residue and determine the total
                     // byte length of schc packet
  int payload_offset;
  header_t header;

  bit_position = 0;

  // SCHC packet filling
  switch (rule_desc->nature) {
    case NATURE_COMPRESSION:
      // Packet Parsing
      payload_offset = 0;
      schc_compression_status =
          parse_header(&header, &payload_offset, packet, packet_byte_len);
      if (!schc_compression_status) {
        break;
      }

      // SCHC compression
      schc_compression_status =
          __compression(schc_packet, schc_packet_max_byte_len, &bit_position,
                        &header, rule_desc, context, context_byte_len);

      if (!schc_compression_status) {
        break;
      }

      // Add payload
      add_bits_to_buffer(schc_packet, schc_packet_max_byte_len, &bit_position,
                         packet + payload_offset,
                         8 * (packet_byte_len - payload_offset));
      break;

    case NATURE_FRAGMENTATION:
      // not implemented
      schc_compression_status = 0;
      break;

    default:  // NATURE_NO_COMPRESSION
      schc_compression_status = __no_compression(
          schc_packet, schc_packet_max_byte_len, &bit_position, packet,
          packet_byte_len, rule_desc, context, context_byte_len);
      break;
  }

  if (schc_compression_status) {
    return BYTE_LENGTH(bit_position);
  }

  return 0;
}

/* ********************************************************************** */

static int __compression(uint8_t*     comp_residue,
                         const size_t comp_residue_max_byte_len,
                         int* bit_position, const header_t* header,
                         const rule_desc_t* rule_desc, const uint8_t* context,
                         const size_t context_byte_len) {
  int               schc_compression_status;
  int               schc_bit_to_add;
  int               target_value_offset;
  int               rule_field_desc_offset;
  int               rule_field_desc_ind;
  rule_field_desc_t rule_field_desc;
  uint8_t*          field_ptr;

  // SCHC Rule ID
  schc_compression_status = __add_schc_rule_id(
      comp_residue, comp_residue_max_byte_len, bit_position, rule_desc->id);
  if (!schc_compression_status) {
    return schc_compression_status;
  }

  // SCHC Field Compression
  rule_field_desc_ind = 0;
  while (rule_field_desc_ind < rule_desc->card_rule_field_desc &&
         schc_compression_status) {
    schc_compression_status = get_rule_field_desc_offset(
        &rule_field_desc_offset, rule_desc, rule_field_desc_ind, context,
        context_byte_len);

    if (!schc_compression_status) {
      break;
    }

    // Get Rule Field Descriptor and First Target Value offset (if exist)
    target_value_offset = get_rule_field_desc_info(
        &rule_field_desc, rule_field_desc_offset, context, context_byte_len);

    // Get field to compress which correspond to the current FID
    schc_compression_status =
        get_header_field(&field_ptr, header, rule_field_desc.fid);

    if (!schc_compression_status) {
      break;
    }

    switch (rule_field_desc.cda) {
      case CDA_LSB: {
        schc_bit_to_add = rule_field_desc.len - rule_field_desc.msb_len;
        uint8_t field_residue[BYTE_LENGTH(schc_bit_to_add)];

        schc_compression_status = CDA_least_significant_bits(
            field_residue, field_ptr, rule_field_desc.len, schc_bit_to_add);

        if (schc_compression_status) {
          add_bits_to_buffer(comp_residue, comp_residue_max_byte_len,
                             bit_position, field_residue, schc_bit_to_add);
        }
        break;
      }

      case CDA_MAPPING_SENT: {
        schc_bit_to_add = bits_counter(rule_field_desc.card_target_value);
        uint8_t field_residue[BYTE_LENGTH(schc_bit_to_add)];

        schc_compression_status =
            CDA_mapping_sent(field_residue, field_ptr, rule_field_desc.len,
                             target_value_offset, context, context_byte_len);

        if (schc_compression_status) {
          add_bits_to_buffer(comp_residue, comp_residue_max_byte_len,
                             bit_position, field_residue, schc_bit_to_add);
        }
        break;
      }

      case CDA_NOT_SENT:
        schc_bit_to_add = 0;
        break;

      case CDA_COMPUTE:
        schc_bit_to_add = 0;
        break;

      default:  // CDA_VALUE_SENT
        schc_bit_to_add = rule_field_desc.len;
        add_bits_to_buffer(comp_residue, comp_residue_max_byte_len,
                           bit_position, field_ptr, schc_bit_to_add);
        break;
    }

    // Variable-Length fields
    if ((rule_field_desc.cda == CDA_LSB ||
         rule_field_desc.cda == CDA_VALUE_SENT) &&
        rule_field_desc.len == 0 && schc_compression_status) {
      schc_compression_status =
          __variable_length_field(comp_residue, comp_residue_max_byte_len,
                                  bit_position, schc_bit_to_add);
    }

    if (!schc_compression_status) {
      break;
    }

    rule_field_desc_ind++;
  }

  return schc_compression_status;
}

/* ********************************************************************** */

static int __no_compression(uint8_t*     schc_packet,
                            const size_t schc_packet_max_byte_len,
                            int* bit_position, const uint8_t* packet,
                            const size_t       packet_byte_len,
                            const rule_desc_t* rule_desc,
                            const uint8_t*     context,
                            const size_t       context_byte_len) {
  int     schc_compression_status;
  int     rule_id_len;
  uint8_t packet_last_byte;
  uint8_t schc_packet_last_byte;

  // No compression literally means that the packet is sent without any
  // modifications. However, the Rule ID needs to be added at the beginning
  // of the packet, which means that 1 more byte might be necessary.
  if (packet_byte_len >= schc_packet_max_byte_len) {
    schc_compression_status = 0;
    return schc_compression_status;
  }

  // We assume that as a rule_desc->id is a uint8_t meaning adding the Rule ID
  // at the beginning of schc_packet needs in the worth case 8 bits. This
  // asumption means that the schc packet byte len is longer of 1 byte maximum.
  memcpy(schc_packet, packet, packet_byte_len);
  rule_id_len = bits_counter(rule_desc->id);
  right_shift(schc_packet, packet_byte_len, rule_id_len);

  // First byte
  schc_packet[0] |= (rule_desc->id << (8 - rule_id_len));

  // Last byte
  packet_last_byte             = packet[packet_byte_len - 1];
  schc_packet_last_byte        = packet_last_byte & ((1 << rule_id_len) - 1);
  schc_packet[packet_byte_len] = (schc_packet_last_byte << (8 - rule_id_len));

  *bit_position           = 8 * packet_byte_len + rule_id_len;
  schc_compression_status = 1;

  return schc_compression_status;
}

/* ********************************************************************** */

static int __variable_length_field(uint8_t*     comp_residue,
                                   const size_t comp_residue_max_byte_len,
                                   int* bit_position, const int variable_len) {
  int     schc_compression_status;
  int     variable_length_len;
  uint8_t variable_length_residue[BYTE_LENGTH(variable_len)];

  if (variable_len < 15) {
    variable_length_len        = 4;
    variable_length_residue[0] = variable_len;
  } else if (variable_len < 255) {
    variable_length_len        = 12;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = variable_len;
  } else {
    variable_length_len        = 28;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = 0xff;
    variable_length_residue[2] = (uint8_t) (variable_len >> 8);
    variable_length_residue[3] = (uint8_t) (variable_len & 0xff);
  }

  schc_compression_status =
      add_bits_to_buffer(comp_residue, comp_residue_max_byte_len, bit_position,
                         variable_length_residue, variable_length_len);

  return schc_compression_status;
}
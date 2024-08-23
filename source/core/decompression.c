#include "context.h"
#include "decompression.h"
#include "protocols/headers.h"
#include "utils/binary.h"
#include "utils/memory.h"

#include <string.h>

/* ********************************************************************** */
/*                           Static definitions                           */
/* ********************************************************************** */

/**
 * @brief Handle decompression according to the Compression Nature.
 *
 * @param packet Pointer to the packet to fill.
 * @param packet_max_byte_len Maximum byte length of the packet.
 * @param packet_direction Packet Direction Indicator.
 * @param schc_packet Pointer to the SCHC packet that needs to be decompressed.
 * @param schc_packet_byte_len Byte length of the SCHC packet to decompress.
 * @param context Pointer to the SCHC Context used to perform decompression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the decompressed SCHC packet.
 */
static size_t __decompression_handler(
    uint8_t *packet, const size_t packet_max_byte_len,
    const direction_indicator_t packet_direction, const uint8_t *schc_packet,
    const size_t schc_packet_byte_len, const uint8_t *context,
    const size_t context_byte_len);

/**
 * @brief Get the SCHC Rule Descriptor used to perform compression and therefore
 * the one which will be use to decompress the packet.
 *
 * @param rule_descriptor Pointer to the Rule Descriptor to fill.
 * @param schc_packet Pointer to the SCHC packet that needs to be decompressed.
 * @param schc_packet_byte_len Byte length of the SCHC packet to decompress.
 * @param bit_position Pointer to the current bit position of SCHC packet.
 * @param context Pointer to the SCHC Context used to perform decompression.
 * @param context_byte_len Byte length of the context.
 * @return The decompression status code, 1 for success, otherwise 0.
 */
static int __get_schc_rule_descriptor(rule_descriptor_t *rule_descriptor,
                                      const uint8_t     *schc_packet,
                                      const size_t       schc_packet_byte_len,
                                      size_t            *bit_position,
                                      const uint8_t     *context,
                                      const size_t       context_byte_len);

/**
 * @brief Handle packets compressed with a no-compression Rule Descriptor.
 *
 * @param packet Pointer to the packet to fill.
 * @param packet_max_byte_len Maximum byte length of the packet.
 * @param bit_position Pointer to the current bit position of the packet.
 * @param schc_packet Pointer to the SCHC packet that needs to be decompressed.
 * @param schc_packet_byte_len Byte length of the SCHC packet to decompress.
 * @return The decompression status code, 1 for success, otherwise 0.
 */
static int __no_compression(uint8_t *packet, const size_t packet_max_byte_len,
                            const size_t   bit_position,
                            const uint8_t *schc_packet,
                            const size_t   schc_packet_byte_len);

/**
 * @brief Perform SCHC decompression on the given SCHC packet using a specified
 * Rule Descriptor.
 *
 * @param packet Pointer to the packet to fill.
 * @param packet_max_byte_len Maximum byte length of the packet.
 * @param packet_bit_position Pointer to the current bit position of the packet.
 * @param schc_packet_bit_position Bit position of the SCHC packet.
 * @param packet_direction Packet Direction Indicator.
 * @param schc_packet Pointer to the SCHC packet that needs to be decompressed.
 * @param schc_packet_byte_len Byte length of the SCHC packet to decompress.
 * @param rule_descriptor Pointer to the Rule Descriptor used to decompress.
 * @param context Pointer to the SCHC Context used to perform decompression.
 * @param context_byte_len Byte length of the context.
 * @return The decompression status code, 1 for success, otherwise 0.
 */
static int __compression(uint8_t *packet, const size_t packet_max_byte_len,
                         size_t                     *packet_bit_position,
                         size_t                      schc_packet_bit_position,
                         const direction_indicator_t packet_direction,
                         const uint8_t              *schc_packet,
                         const size_t                schc_packet_byte_len,
                         const rule_descriptor_t    *rule_descriptor,
                         const uint8_t *context, const size_t context_byte_len);

/**
 * @brief Move SCHC bit position according to the Variable-Length encoded value.
 *
 * @param schc_packet_bit_position Pointer to the current bit position of the
 * SCHC packet.
 * @param rule_field_descriptor Pointer to the Rule Field Descriptor of the
 * current field.
 * @param decompressed_field_len Bit length of the decompressed field.
 */
static void __variable_length_decoding(
    size_t                        *schc_packet_bit_position,
    const rule_field_descriptor_t *rule_field_descriptor,
    const size_t                   decompressed_field_len);

/**
 * @brief Update the Compute Values in the Packet.
 *
 * @param packet Pointer to the Packet.
 * @param packet_byte_length Byte length of the packet.
 * @param compute_entries Pointer to the Compute Entries which stores the
 * Compute values whihc have to be update.
 * @param card_compute_entries Number of compute entries to consider.
 * @param rule_descriptor Pointer to the current Rule Descriptor.
 * @param context Pointer to the SCHC Contxt.
 * @param context_byte_len Byte length of the Context.
 * @return The decompression status code, 1 for success, otherwise 0.
 */
static int __update_compute_entries(uint8_t         *packet,
                                    const size_t     packet_byte_length,
                                    compute_entry_t *compute_entries,
                                    const int        card_compute_entries,
                                    const rule_descriptor_t *rule_descriptor,
                                    const uint8_t           *context,
                                    const size_t             context_byte_len);

/* ********************************************************************** */
/*                        Main decompress function                        */
/* ********************************************************************** */

size_t decompress(uint8_t *packet, const size_t packet_max_byte_len,
                  const direction_indicator_t packet_direction,

                  const uint8_t *schc_packet, const size_t schc_packet_byte_len,
                  const uint8_t *context, const size_t context_byte_len) {
  size_t packet_byte_len;

  packet_byte_len = __decompression_handler(
      packet, packet_max_byte_len, packet_direction, schc_packet,
      schc_packet_byte_len, context, context_byte_len);

  return packet_byte_len;
}

/* ********************************************************************** */
/*                            Static functions                            */
/* ********************************************************************** */

static size_t __decompression_handler(
    uint8_t *packet, const size_t packet_max_byte_len,
    const direction_indicator_t packet_direction, const uint8_t *schc_packet,
    const size_t schc_packet_byte_len, const uint8_t *context,
    const size_t context_byte_len) {
  int                schc_decompression_status;
  size_t             schc_packet_bit_position;
  size_t             packet_bit_position;
  size_t             packet_byte_len;
  rule_descriptor_t *rule_descriptor;

  schc_packet_bit_position = 0;
  packet_bit_position      = 0;
  packet_byte_len          = 0;

  // Allocate rule_descriptor from the pool
  rule_descriptor = (rule_descriptor_t *) pool_alloc(sizeof(rule_descriptor_t));

  // Get a Rule Descriptor thanks to the Rule ID at the beginning of the SCHC
  // packet.
  schc_decompression_status = __get_schc_rule_descriptor(
      rule_descriptor, schc_packet, schc_packet_byte_len,
      &schc_packet_bit_position, context, context_byte_len);

  if (!schc_decompression_status) {
    return schc_decompression_status;
  }

  // Reset the packet
  memset(packet, 0x00, packet_max_byte_len);

  switch (rule_descriptor->nature) {
    case NATURE_COMPRESSION:
      schc_decompression_status = __compression(
          packet, packet_max_byte_len, &packet_bit_position,
          schc_packet_bit_position, packet_direction, schc_packet,
          schc_packet_byte_len, rule_descriptor, context, context_byte_len);

      if (schc_decompression_status) {
        packet_byte_len = BYTE_LENGTH(packet_bit_position);
      }

      break;

    case NATURE_FRAGMENTATION:
      // not implemented
      schc_decompression_status = 0;
      break;

    default:  // NATURE_NO_COMPRESSION
      schc_decompression_status = __no_compression(
          packet, packet_max_byte_len, schc_packet_bit_position, schc_packet,
          schc_packet_byte_len);

      if (schc_decompression_status) {
        packet_byte_len = schc_packet_byte_len - 1;
      }

      break;
  }

  // Deallocate rule_descriptor from the pool
  pool_dealloc(rule_descriptor, sizeof(rule_descriptor_t));

  return packet_byte_len;
}

/* ********************************************************************** */

static int __get_schc_rule_descriptor(rule_descriptor_t *rule_descriptor,
                                      const uint8_t     *schc_packet,
                                      const size_t       schc_packet_byte_len,
                                      size_t            *bit_position,
                                      const uint8_t     *context,
                                      const size_t       context_byte_len) {
  uint8_t card_rule_descriptor;
  uint8_t schc_packet_rule_id;
  size_t  rule_len;

  card_rule_descriptor = context[1];  // Number of Rule Descriptor
                                      // position is 1 in the context
  rule_len            = bits_counter(card_rule_descriptor - 1);
  schc_packet_rule_id = *schc_packet >> (8 - rule_len);

  for (uint8_t index_rule_descriptor = 0;
       index_rule_descriptor < card_rule_descriptor; index_rule_descriptor++) {
    // Get Rule Descriptor
    get_rule_descriptor(rule_descriptor, index_rule_descriptor, context,
                        context_byte_len);

    if (schc_packet_rule_id == rule_descriptor->id) {
      *bit_position += rule_len;
      return 1;
    }
  }

  return 0;
}

/* ********************************************************************** */

static int __no_compression(uint8_t *packet, const size_t packet_max_byte_len,
                            const size_t   bit_position,
                            const uint8_t *schc_packet,
                            const size_t   schc_packet_byte_len) {
  memcpy(packet, schc_packet, schc_packet_byte_len);
  left_shift(packet, schc_packet_byte_len, bit_position);

  return 1;
}

/* ********************************************************************** */

static int __compression(
    uint8_t *packet, const size_t packet_max_byte_len,
    size_t *packet_bit_position, size_t schc_packet_bit_position,
    const direction_indicator_t packet_direction, const uint8_t *schc_packet,
    const size_t schc_packet_byte_len, const rule_descriptor_t *rule_descriptor,
    const uint8_t *context, const size_t context_byte_len) {
  int                      schc_decompression_status;
  int                      index_rule_field_descriptor;
  int                      index_compute_entry;
  int                      card_compute_entries;
  size_t                   payload_byte_position;
  size_t                   decompressed_field_len;
  size_t                   schc_len_to_decompress;
  size_t                   msb_bit_position;
  size_t                   extracted_field_residue_byte_len;
  size_t                   decompressed_field_byte_len;
  size_t                   payload_byte_len;
  uint8_t                  coap_tkl;
  uint16_t                 coap_option_delta;
  uint16_t                 coap_option_length;
  uint16_t                 target_value_offset;
  uint8_t                 *extracted_field_residue;
  uint8_t                 *decompressed_field;
  uint8_t                 *payload;
  rule_field_descriptor_t *rule_field_descriptor;
  compute_entry_t         *compute_entries;

  schc_decompression_status   = 1;
  index_rule_field_descriptor = 0;
  index_compute_entry         = 0;
  card_compute_entries =
      get_cardinal_compute_entries(rule_descriptor, context, context_byte_len);
  payload_byte_position            = 0;
  decompressed_field_len           = 0;
  schc_len_to_decompress           = 0;
  msb_bit_position                 = 0;
  extracted_field_residue_byte_len = 0;
  decompressed_field_byte_len      = 0;
  payload_byte_len                 = 0;
  coap_tkl                         = 0x00;
  coap_option_delta                = 0x0000;
  coap_option_length               = 0x0000;
  target_value_offset              = 0x0000;
  extracted_field_residue          = NULL;
  decompressed_field               = NULL;
  payload                          = NULL;
  rule_field_descriptor            = NULL;
  compute_entries                  = NULL;

  // Allocate compute_entries from the pool
  if (card_compute_entries > 0) {
    compute_entries = (compute_entry_t *) pool_alloc(sizeof(compute_entry_t) *
                                                     card_compute_entries);
  }

  // Allocate rule_field_descriptor from the pool
  rule_field_descriptor =
      (rule_field_descriptor_t *) pool_alloc(sizeof(rule_field_descriptor_t));

  while (index_rule_field_descriptor <
             rule_descriptor->card_rule_field_descriptor &&
         schc_decompression_status) {
    // Get Rule Field Descriptor
    schc_decompression_status = get_rule_field_descriptor(
        rule_field_descriptor, index_rule_field_descriptor,
        rule_descriptor->offset, context, context_byte_len);

    // Check if the Rule Field Descriptor DI corresponds to the packet DI
    if (rule_field_descriptor->di != DI_BI &&
        rule_field_descriptor->di != packet_direction) {
      index_rule_field_descriptor++;
      continue;
    }

    // Set the bit length of compressed field
    if (rule_field_descriptor->len == 0) {
      if (rule_field_descriptor->sid == SID_COAP_TOKEN) {
        decompressed_field_len = 8 * coap_tkl;
      } else if (rule_field_descriptor->sid == SID_COAP_OPTION_DELTA_EXTENDED) {
        decompressed_field_len = get_coap_option_bit_length(
            coap_option_delta, rule_field_descriptor->sid);
      } else if (rule_field_descriptor->sid ==
                 SID_COAP_OPTION_LENGTH_EXTENDED) {
        decompressed_field_len = get_coap_option_bit_length(
            coap_option_length, rule_field_descriptor->sid);
      } else if (rule_field_descriptor->sid == SID_COAP_OPTION_VALUE) {
        decompressed_field_len = get_coap_option_bit_length(
            coap_option_length, rule_field_descriptor->sid);
      }
    } else {
      decompressed_field_len = rule_field_descriptor->len;
    }

    // Allocate decompressed_field from the pool
    decompressed_field_byte_len = BYTE_LENGTH(decompressed_field_len);
    decompressed_field =
        (uint8_t *) pool_alloc(sizeof(uint8_t) * decompressed_field_byte_len);

    if (!schc_decompression_status) {
      return schc_decompression_status;
    }

    // Variable-Length Decoding
    __variable_length_decoding(&schc_packet_bit_position, rule_field_descriptor,
                               decompressed_field_len);

    switch (rule_field_descriptor->cda) {
      case CDA_LSB:
        // Add MSB part from the context to the decompressed field
        memcpy(decompressed_field,
               context + rule_field_descriptor->first_target_value_offset,
               BYTE_LENGTH(rule_field_descriptor->msb_len));

        // Update the bit length
        schc_len_to_decompress =
            rule_field_descriptor->len - rule_field_descriptor->msb_len;

        // Allocate extracted_field_residue from the pool
        extracted_field_residue_byte_len = BYTE_LENGTH(schc_len_to_decompress);
        extracted_field_residue          = (uint8_t *) pool_alloc(
            sizeof(uint8_t) * extracted_field_residue_byte_len);

        // Extract LSB part from the packet in extracted_field_residue
        extract_bits(extracted_field_residue, extracted_field_residue_byte_len,
                     schc_len_to_decompress, &schc_packet_bit_position,
                     schc_packet, schc_packet_byte_len);

        // Shift on left the current MSB part into decompressed_field
        if (schc_len_to_decompress % 8 != 0) {
          left_shift(decompressed_field, decompressed_field_byte_len,
                     schc_len_to_decompress);
        }

        // Add LSB part to decompressed_field
        msb_bit_position = rule_field_descriptor->msb_len;
        add_bits_to_buffer(decompressed_field, decompressed_field_byte_len,
                           &msb_bit_position, extracted_field_residue,
                           schc_len_to_decompress);

        // Deallocate extracted_field_residue from the pool
        pool_dealloc(extracted_field_residue,
                     sizeof(uint8_t) * extracted_field_residue_byte_len);

        break;

      case CDA_MAPPING_SENT:
        // Update the bit length
        schc_len_to_decompress =
            bits_counter(rule_field_descriptor->card_target_value - 1);

        // Allocate extracted_field_residue from the pool
        extracted_field_residue_byte_len = BYTE_LENGTH(schc_len_to_decompress);
        extracted_field_residue          = (uint8_t *) pool_alloc(
            sizeof(uint8_t) * extracted_field_residue_byte_len);

        // Extract mapping index from the packet in extracted_field_residue
        extract_bits(extracted_field_residue, extracted_field_residue_byte_len,
                     schc_len_to_decompress, &schc_packet_bit_position,
                     schc_packet, schc_packet_byte_len);

        // Copy corresponding Target Value in decompressed_field
        if (rule_field_descriptor->card_target_value == 1) {
          target_value_offset =
              rule_field_descriptor->first_target_value_offset;
        } else {
          target_value_offset =
              ((uint16_t)
                   context[rule_field_descriptor->first_target_value_offset +
                           2 * (*extracted_field_residue)])
                  << 8 |
              context[rule_field_descriptor->first_target_value_offset +
                      2 * (*extracted_field_residue) + 1];
        }
        memcpy(decompressed_field, context + target_value_offset,
               decompressed_field_byte_len);

        // Deallocate extracted_field_residue from the pool
        pool_dealloc(extracted_field_residue,
                     sizeof(uint8_t) * extracted_field_residue_byte_len);

        break;

      case CDA_NOT_SENT:
        // Copy Target Value from context to decompressed_field
        memcpy(decompressed_field,
               context + rule_field_descriptor->first_target_value_offset,
               decompressed_field_byte_len);

        break;

      case CDA_COMPUTE:
        // Update current Compute Entry
        compute_entries[index_compute_entry].bit_position =
            *packet_bit_position;
        compute_entries[index_compute_entry].index_rule_field_descriptor =
            index_rule_field_descriptor;

        index_compute_entry++;
        break;

      default:  // CDA_VALUE_SENT
        // Extract directly from the schc_packet the field value
        extract_bits(decompressed_field, decompressed_field_byte_len,
                     decompressed_field_len, &schc_packet_bit_position,
                     schc_packet, schc_packet_byte_len);

        break;
    }

    // Set the current coap variable length value
    if (rule_field_descriptor->sid == SID_COAP_TKL) {
      coap_tkl = decompressed_field[0];
    } else if (rule_field_descriptor->sid == SID_COAP_OPTION_DELTA ||
               rule_field_descriptor->sid == SID_COAP_OPTION_DELTA_EXTENDED) {
      set_coap_option_variable_length(&coap_option_delta, decompressed_field,
                                      decompressed_field_byte_len,
                                      rule_field_descriptor->sid);

    } else if (rule_field_descriptor->sid == SID_COAP_OPTION_LENGTH ||
               rule_field_descriptor->sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
      set_coap_option_variable_length(&coap_option_length, decompressed_field,
                                      decompressed_field_byte_len,
                                      rule_field_descriptor->sid);
    }

    // Add decompressed_field into packet
    if (schc_decompression_status) {
      add_bits_to_buffer(packet, packet_max_byte_len, packet_bit_position,
                         decompressed_field, decompressed_field_len);
    }

    // Deallocate decompressed_field from the pool
    pool_dealloc(decompressed_field,
                 sizeof(uint8_t) * decompressed_field_byte_len);

    // Move to next Rule Field Descriptor
    index_rule_field_descriptor++;
  }

  // Deallocate rule_field_descriptor from the pool
  pool_dealloc(rule_field_descriptor, sizeof(rule_field_descriptor_t));

  // Allocate payload
  payload_byte_position = schc_packet_bit_position / 8;
  payload_byte_len      = schc_packet_byte_len - payload_byte_position;
  payload = (uint8_t *) pool_alloc(sizeof(uint8_t) * (payload_byte_len));

  // Copy payload content and remove unnecessary part
  memcpy(payload, schc_packet + payload_byte_position, payload_byte_len);
  if (schc_packet_bit_position % 8 != 0) {
    right_shift(payload, payload_byte_len, 8 - (schc_packet_bit_position % 8));
    memmove(payload, payload + 1, payload_byte_len--);
  }

  // Add payload at the end of the packet
  add_bits_to_buffer(packet, packet_max_byte_len, packet_bit_position, payload,
                     8 * (payload_byte_len));

  // Deallocate payload from the pool
  pool_dealloc(payload, sizeof(uint8_t) * payload_byte_len);

  // Handle Compute Entries
  if (card_compute_entries > 0) {
    // Update Compute entries
    schc_decompression_status = __update_compute_entries(
        packet, BYTE_LENGTH(*packet_bit_position), compute_entries,
        card_compute_entries, rule_descriptor, context, context_byte_len);

    // Deallocate compute_entries from the pool
    pool_dealloc(compute_entries,
                 sizeof(compute_entry_t) * card_compute_entries);
  }

  return schc_decompression_status;
}

/* ********************************************************************** */

static void __variable_length_decoding(
    size_t                        *schc_packet_bit_position,
    const rule_field_descriptor_t *rule_field_descriptor,
    const size_t                   decompressed_field_len) {
  if ((rule_field_descriptor->cda == CDA_LSB ||
       rule_field_descriptor->cda == CDA_VALUE_SENT) &&
      rule_field_descriptor->len == 0) {
    if (decompressed_field_len < 15) {
      *schc_packet_bit_position += 4;
    } else if (decompressed_field_len < 255) {
      *schc_packet_bit_position += 12;
    } else {
      *schc_packet_bit_position += 28;
    }
  }
}

/* ********************************************************************** */

static int __update_compute_entries(uint8_t         *packet,
                                    const size_t     packet_byte_length,
                                    compute_entry_t *compute_entries,
                                    const int        card_compute_entries,
                                    const rule_descriptor_t *rule_descriptor,
                                    const uint8_t           *context,
                                    const size_t             context_byte_len) {
  int                      schc_decompression_status;
  int                      index_compute_entry;
  size_t                   current_bit_position;
  size_t                   tmp_value;
  rule_field_descriptor_t *rule_field_descriptor;
  uint8_t                 *compute_value;

  schc_decompression_status = 1;
  index_compute_entry       = 0;
  current_bit_position      = 0;
  tmp_value                 = 0;
  rule_field_descriptor     = NULL;
  compute_value             = NULL;

  // Allocate rule_field_descriptor from the pool
  rule_field_descriptor =
      (rule_field_descriptor_t *) pool_alloc(sizeof(rule_field_descriptor_t));

  while (index_compute_entry < card_compute_entries &&
         schc_decompression_status) {
    // Get Rule Field Descriptor
    schc_decompression_status = get_rule_field_descriptor(
        rule_field_descriptor,
        compute_entries[index_compute_entry].index_rule_field_descriptor,
        rule_descriptor->offset, context, context_byte_len);

    if (rule_field_descriptor->sid == SID_IPV6_PAYLOAD_LENGTH ||
        rule_field_descriptor->sid == SID_UDP_LENGTH ||
        rule_field_descriptor->sid == SID_UDP_CHECKSUM) {
      // Allocate compute_value from the pool
      compute_value = (uint8_t *) pool_alloc(
          sizeof(uint8_t) * 2);  // IPv6 Payload Length, UDP Length and UDP
                                 // Checksum needs only 2 bytes.

      current_bit_position = compute_entries[index_compute_entry].bit_position;

      if (rule_field_descriptor->sid == SID_UDP_CHECKSUM) {
        udp_checksum(compute_value, 2, packet, packet_byte_length, 1);
      } else {
        tmp_value        = packet_byte_length - 40;
        compute_value[0] = (uint8_t) ((tmp_value >> 8) & 0xff);
        compute_value[1] = (uint8_t) (tmp_value & 0xff);
      }

      // Update the compute content directly into the packet
      schc_decompression_status = add_bits_to_buffer(
          packet, packet_byte_length, &current_bit_position, compute_value, 16);

      // Deallocate compute_value from the pool
      pool_dealloc(compute_value, sizeof(uint8_t) * 2);
    } else {
      schc_decompression_status = 0;
    }

    index_compute_entry++;
  }

  // Deallocate rule_field_descriptor from the pool
  pool_dealloc(rule_field_descriptor, sizeof(rule_field_descriptor_t));

  return schc_decompression_status;
}
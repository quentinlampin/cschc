#include "compression.h"
#include "context.h"
#include "protocols/headers.h"
#include "utils/binary.h"
#include "utils/memory.h"

#include <string.h>

/* ********************************************************************** */
/*                           Static definitions                           */
/* ********************************************************************** */

/**
 * @brief Handles compression according to the Compression Nature.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param packet_direction Packet Direction Indicator.
 * @param packet Pointer to the Packet that needs to be compressed.
 * @param packet_byte_len Byte length of the packet to compress.
 * @param context Pointer to the SCHC Context used to perform compression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the compressed SCHC packet.
 */
static size_t __compression_handler(
    uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
    const direction_indicator_t packet_direction, const uint8_t* packet,
    const size_t packet_byte_len, const uint8_t* context,
    const size_t context_byte_len);

/**
 * @brief Adds SCHC Rule ID at the beginning of the SCHC Packet (Compression
 * Residue).
 *
 * @details RFC 8724, Section 5.1.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param bit_position Pointer to the current position, from where to add the
 * rule_id.
 * @param rule_id Rule ID value to add.
 * @param card_rule_descriptor Number of Rule Descriptor in the SCHC Context.
 * @return The compression status code, 1 for success, otherwise 0.
 */
static int __add_schc_rule_id(uint8_t*     schc_packet,
                              const size_t schc_packet_max_byte_len,
                              size_t* bit_position, const uint8_t rule_id,
                              const uint8_t card_rule_descriptor);

/**
 * @brief Handles Packets with SCHC No-compression Nature.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param bit_position Pointer to the current position, from where to add the
 * packet content.
 * @param packet Pointer to the Packet to add to schc_packet.
 * @param packet_byte_len Byte length of the packet to compress.
 * @return The compression status code, 1 for success, otherwise 0.
 */
static int __no_compression(uint8_t*     schc_packet,
                            const size_t schc_packet_max_byte_len,
                            size_t* bit_position, const uint8_t* packet,
                            const size_t packet_byte_len);

/**
 * @brief Performs SCHC compression on the given packet using a specified Rule
 * Descriptor.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param bit_position Pointer to the current position, from where to add the
 * content.
 * @param packet_direction Packet Direction Indicator.
 * @param packet Pointer to the Packet that needs to be compressed.
 * @param packet_byte_len Byte length of the packet to compress.
 * @param rule_descriptor Pointer to the Rule Descriptor used to compress the
 * packet.
 * @param context Pointer to the SCHC Context used to perform compression.
 * @param context_byte_len Byte length of the context.
 * @return The compression status code, 1 for success, otherwise 0.
 */
static int __compression(uint8_t*                    schc_packet,
                         const size_t                schc_packet_max_byte_len,
                         size_t*                     bit_position,
                         const direction_indicator_t packet_direction,
                         const uint8_t* packet, const size_t packet_byte_len,
                         const rule_descriptor_t* rule_descriptor,
                         const uint8_t* context, const size_t context_byte_len);

/**
 * @brief Handles fields with Variable-Length during compression, basically CoAP
 * Token and Options.
 *
 * @details RFC 8724, Section 7.4.2.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param bit_position Pointer to the current position, from where to add the
 * content.
 * @param variable_len Bit length of the Variable-Length Field.
 * @return The compression status code, 1 for success, otherwise 0.
 */
static int __variable_length_encoding(uint8_t*     schc_packet,
                                      const size_t schc_packet_max_byte_len,
                                      size_t*      bit_position,
                                      const int    variable_len);

/* ********************************************************************** */
/*                         Main compress function                         */
/* ********************************************************************** */

size_t compress(uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
                const direction_indicator_t packet_direction,
                const uint8_t* packet, const size_t packet_byte_len,
                const uint8_t* context, const size_t context_byte_len) {
  size_t schc_packet_byte_len;

  schc_packet_byte_len = __compression_handler(
      schc_packet, schc_packet_max_byte_len, packet_direction, packet,
      packet_byte_len, context, context_byte_len);

  return schc_packet_byte_len;
}

/* ********************************************************************** */
/*                            Static functions                            */
/* ********************************************************************** */

static size_t __compression_handler(
    uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
    const direction_indicator_t packet_direction, const uint8_t* packet,
    const size_t packet_byte_len, const uint8_t* context,
    const size_t context_byte_len) {
  int     schc_compression_status;
  int     index_rule_descriptor;
  uint8_t card_rule_descriptor;
  size_t  bit_position;  // Usefull to append field_residue and determine the
                         // total byte length of the final schc_packet
  rule_descriptor_t* rule_descriptor;

  schc_compression_status = 0;  // Set to false
  index_rule_descriptor   = 0;
  card_rule_descriptor    = context[CARD_RULE_DESCRIPTOR_OFFSET];

  // Allocate rule_descriptor from the pool
  rule_descriptor = (rule_descriptor_t*) pool_alloc(sizeof(rule_descriptor_t));

  // As we expect the no-compression Rule as the last one in the Context, we
  // might not reach the default case before the last index feasible
  while (index_rule_descriptor < card_rule_descriptor &&
         !schc_compression_status) {
    // Init
    bit_position = 0;
    memset(schc_packet, 0x00, schc_packet_max_byte_len);

    // Get Rule Descriptor
    schc_compression_status = get_rule_descriptor(
        rule_descriptor, index_rule_descriptor, context, context_byte_len);

    // SCHC Rule ID
    schc_compression_status =
        __add_schc_rule_id(schc_packet, schc_packet_max_byte_len, &bit_position,
                           rule_descriptor->id, card_rule_descriptor);

    if (!schc_compression_status) {
      break;
    }

    // SCHC Packet filling
    switch (rule_descriptor->nature) {
      case NATURE_COMPRESSION:
        schc_compression_status =
            __compression(schc_packet, schc_packet_max_byte_len, &bit_position,
                          packet_direction, packet, packet_byte_len,
                          rule_descriptor, context, context_byte_len);
        break;

      case NATURE_FRAGMENTATION:
        // Not implemented yet
        schc_compression_status = 0;
        break;

      default:  // NATURE_NO_COMPRESSION
        schc_compression_status =
            __no_compression(schc_packet, schc_packet_max_byte_len,
                             &bit_position, packet, packet_byte_len);
        break;
    }

    // Move to the next Rule Descriptor index
    index_rule_descriptor++;
  }

  // Deallocate rule_descriptor from the pool
  pool_dealloc(rule_descriptor, sizeof(rule_descriptor_t));

  if (schc_compression_status) {
    return BYTE_LENGTH(bit_position);
  }

  return 0;
}

/* ********************************************************************** */

static int __add_schc_rule_id(uint8_t*     schc_packet,
                              const size_t schc_packet_max_byte_len,
                              size_t* bit_position, const uint8_t rule_id,
                              const uint8_t card_rule_descriptor) {
  int    schc_compression_status;
  size_t rule_len;

  rule_len = bits_counter(card_rule_descriptor - 1);

  schc_compression_status = add_byte_to_buffer(
      schc_packet, schc_packet_max_byte_len, bit_position, rule_id, rule_len);

  return schc_compression_status;
}

/* ********************************************************************** */

static int __no_compression(uint8_t*     schc_packet,
                            const size_t schc_packet_max_byte_len,
                            size_t* bit_position, const uint8_t* packet,
                            const size_t packet_byte_len) {
  int schc_compression_status;

  schc_compression_status =
      add_bits_to_buffer(schc_packet, schc_packet_max_byte_len, bit_position,
                         packet, 8 * packet_byte_len);

  return schc_compression_status;
}

/* ********************************************************************** */

static int __compression(uint8_t*                    schc_packet,
                         const size_t                schc_packet_max_byte_len,
                         size_t*                     bit_position,
                         const direction_indicator_t packet_direction,
                         const uint8_t* packet, const size_t packet_byte_len,
                         const rule_descriptor_t* rule_descriptor,
                         const uint8_t*           context,
                         const size_t             context_byte_len) {
  int                      schc_compression_status;
  int                      index_rule_field_descriptor;
  size_t                   packet_bit_position;
  size_t                   payload_byte_position;
  size_t                   schc_len_to_add;
  size_t                   field_residue_byte_len;
  size_t                   extracted_field_byte_len;
  uint8_t                  coap_tkl;
  uint16_t                 coap_option_delta;
  uint16_t                 coap_option_length;
  uint8_t*                 field_residue;
  uint8_t*                 extracted_field;
  rule_field_descriptor_t* rule_field_descriptor;

  // Init
  schc_compression_status     = 1;
  index_rule_field_descriptor = 0;
  packet_bit_position         = 0;
  coap_tkl                    = 0x00;
  coap_option_delta           = 0x0000;
  coap_option_length          = 0x0000;
  field_residue               = NULL;
  extracted_field             = NULL;
  rule_field_descriptor       = NULL;

  // Allocate rule_field_descriptor from the pool
  rule_field_descriptor =
      (rule_field_descriptor_t*) pool_alloc(sizeof(rule_field_descriptor_t));

  while (index_rule_field_descriptor <
             rule_descriptor->card_rule_field_descriptor &&
         schc_compression_status) {
    // Get Rule Field Descriptor
    schc_compression_status = get_rule_field_descriptor(
        rule_field_descriptor, index_rule_field_descriptor,
        rule_descriptor->offset, context, context_byte_len);

    if (!schc_compression_status) {
      break;
    }

    // Check if the Rule Field Descriptor DI corresponds to the Packet DI
    if (rule_field_descriptor->di != DI_BI &&
        rule_field_descriptor->di != packet_direction) {
      index_rule_field_descriptor++;
      continue;
    }

    // Set the bit length to extract
    if (rule_field_descriptor->len == 0) {
      if (rule_field_descriptor->sid == SID_COAP_TOKEN) {
        schc_len_to_add = 8 * coap_tkl;
      } else if (rule_field_descriptor->sid == SID_COAP_OPTION_DELTA_EXTENDED) {
        schc_len_to_add = get_coap_option_bit_length(
            coap_option_delta, rule_field_descriptor->sid);
      } else if (rule_field_descriptor->sid ==
                 SID_COAP_OPTION_LENGTH_EXTENDED) {
        schc_len_to_add = get_coap_option_bit_length(
            coap_option_length, rule_field_descriptor->sid);

      } else if (rule_field_descriptor->sid == SID_COAP_OPTION_VALUE) {
        schc_len_to_add = get_coap_option_bit_length(
            coap_option_length, rule_field_descriptor->sid);
      }
    } else {
      schc_len_to_add = rule_field_descriptor->len;
    }

    // Allocate extracted_field from the pool
    extracted_field_byte_len = BYTE_LENGTH(schc_len_to_add);
    extracted_field =
        (uint8_t*) pool_alloc(sizeof(uint8_t) * extracted_field_byte_len);
    // Extract the corresponding Field from Packet
    schc_compression_status =
        extract_bits(extracted_field, extracted_field_byte_len, schc_len_to_add,
                     &packet_bit_position, packet, packet_byte_len);

    if (!schc_compression_status) {
      break;
    }

    // Set the current CoAP Variable-Length value
    if (rule_field_descriptor->sid == SID_COAP_TKL) {
      coap_tkl = extracted_field[0];
    } else if (rule_field_descriptor->sid == SID_COAP_OPTION_DELTA ||
               rule_field_descriptor->sid == SID_COAP_OPTION_DELTA_EXTENDED) {
      set_coap_option_variable_length(&coap_option_delta, extracted_field,
                                      extracted_field_byte_len,
                                      rule_field_descriptor->sid);

    } else if (rule_field_descriptor->sid == SID_COAP_OPTION_LENGTH ||
               rule_field_descriptor->sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
      set_coap_option_variable_length(&coap_option_length, extracted_field,
                                      extracted_field_byte_len,
                                      rule_field_descriptor->sid);
    }

    // Compress the extracted_field
    // Steps :
    // 1. Update the amount of bits to add as Field Residue if necessary.
    // 2. Perform the (De)Compression Action defined in the current Rule Field
    // Descriptor.
    // 3. Add the Field Residue to the SCHC Packet.
    switch (rule_field_descriptor->cda) {
      case CDA_LSB:
        // Update the bit length
        schc_len_to_add =
            rule_field_descriptor->len - rule_field_descriptor->msb_len;

        // Allocate field_residue from the pool
        field_residue_byte_len = BYTE_LENGTH(schc_len_to_add);
        field_residue =
            (uint8_t*) pool_alloc(sizeof(uint8_t) * field_residue_byte_len);

        // Apply LSB on the extracted_field
        schc_compression_status = CDA_least_significant_bits(
            field_residue, extracted_field, rule_field_descriptor, context,
            context_byte_len);
        break;

      case CDA_MAPPING_SENT:
        // Update the bit length
        schc_len_to_add =
            bits_counter(rule_field_descriptor->card_target_value - 1);

        // Allocate field_residue from the pool
        field_residue_byte_len = BYTE_LENGTH(schc_len_to_add);
        field_residue =
            (uint8_t*) pool_alloc(sizeof(uint8_t) * field_residue_byte_len);

        // Apply Mapping Sent on the extracted_field
        schc_compression_status =
            CDA_mapping_sent(field_residue, extracted_field,
                             rule_field_descriptor, context, context_byte_len);
        break;

      case CDA_NOT_SENT:
        // Update the bit length
        schc_len_to_add = 0;

        // Apply Not Sent on the extracted_field
        schc_compression_status = CDA_not_sent(
            extracted_field, rule_field_descriptor, context, context_byte_len);
        break;

      case CDA_COMPUTE:
        // Update the bit length
        schc_len_to_add = 0;

        // Apply Compute
        schc_compression_status = CDA_compute();
        break;

      default:  // CDA_VALUE_SENT
        // Apply Value Sent
        schc_compression_status = CDA_value_sent();
        break;
    }

    // Handle Variable-Length Encoding
    if ((rule_field_descriptor->cda == CDA_LSB ||
         rule_field_descriptor->cda == CDA_VALUE_SENT) &&
        rule_field_descriptor->len == 0 && schc_compression_status) {
      schc_compression_status = __variable_length_encoding(
          schc_packet, schc_packet_max_byte_len, bit_position, schc_len_to_add);
    }

    // Add Field Residue or Extracted Field to SCHC Packet
    if (schc_len_to_add > 0) {
      if (rule_field_descriptor->cda == CDA_VALUE_SENT &&
          schc_compression_status) {
        schc_compression_status =
            add_bits_to_buffer(schc_packet, schc_packet_max_byte_len,
                               bit_position, extracted_field, schc_len_to_add);
      } else if (rule_field_descriptor->cda != CDA_VALUE_SENT &&
                 !schc_compression_status) {
        // This statement is only in case of error. Indeed, if compression
        // status is false. We still need to deallocate field_residue.
        pool_dealloc(field_residue, sizeof(uint8_t) * field_residue_byte_len);
      } else {
        schc_compression_status =
            add_bits_to_buffer(schc_packet, schc_packet_max_byte_len,
                               bit_position, field_residue, schc_len_to_add);

        // Deallocate field_residue from the pool
        pool_dealloc(field_residue, sizeof(uint8_t) * field_residue_byte_len);
      }
    }

    // Move to next Rule Field Descriptor index
    index_rule_field_descriptor++;

    // Deallocate extracted_field from the pool
    pool_dealloc(extracted_field, sizeof(uint8_t) * extracted_field_byte_len);
  }

  // Add payload at the end of the SCHC Packet.
  if (schc_compression_status) {
    payload_byte_position = BYTE_LENGTH(packet_bit_position);
    schc_compression_status =
        add_bits_to_buffer(schc_packet, schc_packet_max_byte_len, bit_position,
                           packet + payload_byte_position,
                           8 * (packet_byte_len - payload_byte_position));
  }

  // Deallocate rule_field_descriptor from the pool
  pool_dealloc(rule_field_descriptor, sizeof(rule_field_descriptor_t));

  return schc_compression_status;
}

/* ********************************************************************** */

static int __variable_length_encoding(uint8_t*     schc_packet,
                                      const size_t schc_packet_max_byte_len,
                                      size_t*      bit_position,
                                      const int    variable_len) {
  int      schc_compression_status;
  size_t   variable_length_len;
  uint8_t* variable_length_residue;

  // Steps:
  // 1. Allocate the right memory space in order to determine the Variable
  // Length Residue.
  // 2. Determine the corresponding Residue.
  // 3. Add the Residue to the SCHC Packet.
  if (variable_len < 15) {
    variable_length_residue    = (uint8_t*) pool_alloc(sizeof(uint8_t));
    variable_length_len        = 4;
    variable_length_residue[0] = variable_len;
  } else if (variable_len < 255) {
    variable_length_residue    = (uint8_t*) pool_alloc(sizeof(uint8_t) * 2);
    variable_length_len        = 12;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = variable_len;
  } else {
    variable_length_residue    = (uint8_t*) pool_alloc(sizeof(uint8_t) * 4);
    variable_length_len        = 28;
    variable_length_residue[0] = 0x0f;
    variable_length_residue[1] = 0xff;
    variable_length_residue[2] = (uint8_t) (variable_len >> 8);
    variable_length_residue[3] = (uint8_t) (variable_len & 0xff);
  }

  schc_compression_status =
      add_bits_to_buffer(schc_packet, schc_packet_max_byte_len, bit_position,
                         variable_length_residue, variable_length_len);

  // Deallocate variable_length_residue from the pool
  pool_dealloc(variable_length_residue,
               sizeof(uint8_t) * BYTE_LENGTH(variable_length_len));

  return schc_compression_status;
}
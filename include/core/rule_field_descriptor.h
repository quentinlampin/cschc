/**
 * @file rule_field_descriptor.h
 * @author Corentin Banier
 * @brief SCHC Rule Field Descriptor implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @details See Section 7.1 SCHC RFC 8724.
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _RULE_FIELD_DESCRIPTOR_H_
#define _RULE_FIELD_DESCRIPTOR_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Struct that defines a Rule Field Descriptor.
 */
typedef struct {
  uint16_t                           sid;  // Schema Item iDentifier
  uint16_t                           len;  // Field Length
  uint16_t                           pos;  // Field Position
  direction_indicator_t              di;   //  Direction Indicator
  matching_operator_t                mo;   //  Matching Operator
  compression_decompression_action_t cda;  // Compression Decompression Action
  uint16_t                           msb_len;  // Most Significant Bits Length
  uint8_t  card_target_value;                  // Number of Target Value
  uint16_t first_target_value_offset;  // If it is a Mapping Operator / Action,
                                       // then it represents the first byte of
                                       // the list of offsets of the Target
                                       // Values. Otherwise, the reconstructed
                                       // offset (uint16_t) directly indicates
                                       // the real offset of the Target Value in
                                       // the Context.
} rule_field_descriptor_t;

/**
 * @brief Extracts Direction Indicator, Matching Operator, and Compression
 * Decompression Action from a bit-packed value.
 *
 * @details See RFC SCHC 8724.
 *
 * @param di Pointer to the Direction Indicator to fill.
 * @param mo Pointer to the Matching Operator to fill.
 * @param cda Pointer to the Compression Decompression Action to fill.
 * @param packed_value Value to unpack.
 *
 * @remark The packed_value is defined on 8 bits but only 7 bits are necessary
 * to store the content of DI (2 bits), MO (2 bits) and CDA (3 bits).
 */
void unpack_di_mo_cda(direction_indicator_t *di, matching_operator_t *mo,
                      compression_decompression_action_t *cda,
                      const uint8_t                       packed_value);

/**
 * @brief Gets a Rule Field Descriptor using an index and the offset of the
 * associated Rule Descriptor.
 *
 * @param rule_field_descriptor Pointer to the Rule Field Descriptor to fill.
 * @param index Index of the Rule Field Descriptor to get in the Rule
 * Descriptor.
 * @param rule_descriptor_offset Offset of the corresponding Rule Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The status code, 1 for success, otherwise 0.
 */
int get_rule_field_descriptor(rule_field_descriptor_t *rule_field_descriptor,
                              const unsigned int       index,
                              const uint16_t           rule_descriptor_offset,
                              const uint8_t           *context,
                              const size_t             context_byte_len);

#endif  // _RULE_FIELD_DESCRIPTOR_H_
#ifndef _RULE_FIELD_DESCRIPTOR_H_
#define _RULE_FIELD_DESCRIPTOR_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Struct that defines a Rule Field Descriptor.
 */
typedef struct {
  uint16_t                           sid;
  uint16_t                           len;
  uint16_t                           pos;
  direction_indicator_t              di;
  matching_operator_t                mo;
  compression_decompression_action_t cda;
  uint16_t                           msb_len;
  uint8_t                            card_target_value;
  uint16_t                           first_target_value_offset;
} rule_field_descriptor_t;

/**
 * @brief Extracts DI, MO, and CDA from a bit-packed value.
 *
 * @details See RFC SCHC 8724.
 *
 * @param di Pointer to the Direction Indicator.
 * @param mo Pointer to the Matching Operator.
 * @param cda Pointer to the Compression Decompression Action.
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
 * @param rule_descriptor_offset Offset of the Rule Descriptor.
 * @param context Pointer to the CSCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The status code, 1 for success, otherwise 0.
 */
int get_rule_field_descriptor(rule_field_descriptor_t *rule_field_descriptor,
                              const unsigned int       index,
                              const uint16_t           rule_descriptor_offset,
                              const uint8_t           *context,
                              const size_t             context_byte_len);

#endif  // _RULE_FIELD_DESCRIPTOR_H_
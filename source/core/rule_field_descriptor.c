#include "rule_field_descriptor.h"

/* ********************************************************************** */

void unpack_di_mo_cda(direction_indicator_t *di, matching_operator_t *mo,
                      compression_decompression_action_t *cda,
                      const uint8_t                       packed_value) {
  *di  = (direction_indicator_t) ((packed_value >> 5) & 0x03);
  *mo  = (matching_operator_t) ((packed_value >> 3) & 0x03);
  *cda = (compression_decompression_action_t) (packed_value & 0x07);
}

/* ********************************************************************** */

int get_rule_field_descriptor(rule_field_descriptor_t *rule_field_descriptor,
                              const unsigned int       index,
                              const uint16_t           rule_descriptor_offset,
                              const uint8_t           *context,
                              const size_t             context_byte_len) {
  uint16_t rule_field_descriptor_offset;

  // context[rule_descriptor_offset + 2] represents the total number of the Rule
  // Field Descriptor in the corresponding Rule Descriptor.
  if (index >= context[rule_descriptor_offset + 2] ||
      rule_descriptor_offset + 3 + 2 * index + 1 >= context_byte_len) {
    return 0;
  }

  rule_field_descriptor_offset =
      ((uint16_t) context[rule_descriptor_offset + 3 + 2 * index] << 8) |
      context[rule_descriptor_offset + 3 + 2 * index + 1];

  rule_field_descriptor->sid =
      ((uint16_t) context[rule_field_descriptor_offset] << 8) |
      context[rule_field_descriptor_offset + 1];
  rule_field_descriptor_offset += 2;

  rule_field_descriptor->len =
      ((uint16_t) context[rule_field_descriptor_offset] << 8) |
      context[rule_field_descriptor_offset + 1];
  rule_field_descriptor_offset += 2;

  rule_field_descriptor->pos =
      ((uint16_t) context[rule_field_descriptor_offset] << 8) |
      context[rule_field_descriptor_offset + 1];
  rule_field_descriptor_offset += 2;

  unpack_di_mo_cda(&rule_field_descriptor->di, &rule_field_descriptor->mo,
                   &rule_field_descriptor->cda,
                   context[rule_field_descriptor_offset++]);

  if (rule_field_descriptor->mo == MO_MSB) {
    rule_field_descriptor->msb_len =
        ((uint16_t) context[rule_field_descriptor_offset] << 8) |
        context[rule_field_descriptor_offset + 1];
    rule_field_descriptor_offset += 2;
  } else {
    rule_field_descriptor->msb_len = 0;
  }

  rule_field_descriptor->card_target_value =
      context[rule_field_descriptor_offset++];

  if (rule_field_descriptor->card_target_value == 1) {
    rule_field_descriptor->first_target_value_offset =
        ((uint16_t) context[rule_field_descriptor_offset] << 8) |
        context[rule_field_descriptor_offset + 1];
  } else if (rule_field_descriptor->card_target_value > 1) {
    rule_field_descriptor->first_target_value_offset =
        rule_field_descriptor_offset;
  }

  else {
    rule_field_descriptor->first_target_value_offset = 0;
  }

  return 1;
}
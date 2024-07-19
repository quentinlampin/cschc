#include "core/rule_field_descriptor.h"

/* ********************************************************************** */

void unpack_dir_mo_cda(direction_t *dir, matching_operator_t *mo,
                       compression_decompression_action_t *cda,
                       const uint8_t                       packed_value) {
  *dir = (direction_t) ((packed_value >> 5) & 0b00000011);
  *mo  = (matching_operator_t) ((packed_value >> 3) & 0b00000011);
  *cda = (compression_decompression_action_t) (packed_value & 0b00000111);
}

/* ********************************************************************** */

int get_rule_field_desc_info(rule_field_desc_t *rule_field_desc, int offset,
                             const uint8_t *context, const size_t context_len) {
  // add checks on offset !
  if (context_len <= offset) {
    return -1;
  }

  uint8_t dir_mo_cda;

  rule_field_desc->fid =
      ((uint16_t) context[offset] << 8) | context[offset + 1];
  offset += 2;

  rule_field_desc->len =
      ((uint16_t) context[offset] << 8) | context[offset + 1];
  offset += 2;

  rule_field_desc->pos =
      ((uint16_t) context[offset] << 8) | context[offset + 1];
  offset += 2;

  dir_mo_cda = context[offset++];
  unpack_dir_mo_cda(&rule_field_desc->dir, &rule_field_desc->mo,
                    &rule_field_desc->cda, dir_mo_cda);

  if (rule_field_desc->mo == MO_MSB) {
    rule_field_desc->msb_len =
        ((uint16_t) context[offset] << 8) | context[offset + 1];
    offset += 2;
  } else {
    rule_field_desc->msb_len = 0;
  }

  rule_field_desc->card_target_value = context[offset];

  return offset;
}
#include "core/context.h"

/* ********************************************************************** */

int get_rule_field_desc_offset(int *offset, const rule_desc_t *rule_desc,
                               const int ind_factor, const uint8_t *context,
                               const size_t context_byte_len) {
  int status;

  status = (rule_desc->offset + 3 + 2 * ind_factor + 1 >= context_byte_len ||
            rule_desc->card_rule_field_desc == 0)
               ? 0
               : 1;

  if (status) {
    *offset = ((int) context[rule_desc->offset + 3 + 2 * ind_factor] << 8) |
              context[rule_desc->offset + 3 + 2 * ind_factor + 1];
  }

  status = status && (*offset < context_byte_len);

  return status;
}

/* ********************************************************************** */

/**
 * @details To fix :
 * Return when FID corresponds but it's possible to see multiple same FID in one
 * rule. Meaning the one return is maybe not the offset that we expect.
 */
int get_rule_field_desc_offset_from_fid(int *offset, const uint16_t fid,
                                        const rule_desc_t *rule_desc,
                                        const uint8_t     *context,
                                        const size_t       context_byte_len) {
  int      ctx_offset;
  int      rule_field_desc_offset;
  uint16_t current_fid;

  // add overflow checks in context

  ctx_offset =
      rule_desc->offset + 3;  // First byte of first Rule Field Descriptor

  for (int i = 0; i < rule_desc->card_rule_field_desc; i++) {
    rule_field_desc_offset = ((int) context[ctx_offset + 2 * i] << 8) |
                             context[ctx_offset + 2 * i + 1];

    current_fid = ((uint16_t) context[rule_field_desc_offset] << 8) |
                  context[rule_field_desc_offset + 1];

    if (current_fid == fid) {
      *offset = rule_field_desc_offset;
      return 1;
    }
  }

  return 0;
}
#include "core/context.h"

/* ********************************************************************** */

/**
 * @details To fix :
 * Return when FID correspond but it's possible to see multiple same FID in one
 * rule. Meaning the one return is maybe not the offset that we expect.
 */
int get_rule_field_desc_offset_from_fid(int *offset, const uint16_t fid,
                                        const uint8_t *context,
                                        const size_t   context_len) {
  // context
  int               ctx_offset, rule_desc_offset, rule_field_desc_offset;
  uint8_t           card_rule_desc;
  rule_desc_t       rule_desc;
  rule_field_desc_t rule_field_desc;

  ctx_offset     = 1;
  card_rule_desc = context[ctx_offset++];

  if (card_rule_desc > 0) {
    // add a for loop to iterate on different rule_desc
    rule_desc_offset =
        ((int) context[ctx_offset] << 8) | context[ctx_offset + 1];
    ctx_offset += 2;

    rule_desc_offset =
        get_rule_desc_info(&rule_desc, rule_desc_offset, context, context_len);

    if (rule_desc.card_rule_field_desc > 0) {
      for (int i = 0; i < rule_desc.card_rule_field_desc; i++) {
        rule_field_desc_offset =
            ((int) context[rule_desc_offset + 2 * i] << 8) |
            context[rule_desc_offset + 2 * i + 1];

        get_rule_field_desc_info(&rule_field_desc, rule_field_desc_offset,
                                 context, context_len);

        if (rule_field_desc.fid == fid) {
          *offset = rule_field_desc_offset;
          return 1;
        }
      }
    }
  }

  return 0;
}
#include "core/rule_descriptor.h"

/* ********************************************************************** */

int get_rule_desc_info(rule_desc_t *rule_desc, int offset,
                       const uint8_t *context, const size_t context_len) {
  // add checks on offset !
  if (context_len <= offset) {
    return -1;
  }

  rule_desc->offset               = (uint16_t) offset;
  rule_desc->id                   = context[offset++];
  rule_desc->nature               = (nature_t) (context[offset++]);
  rule_desc->card_rule_field_desc = context[offset++];

  return offset;
}
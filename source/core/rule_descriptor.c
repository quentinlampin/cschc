#include "rule_descriptor.h"
#include "utils/binary.h"

/* ********************************************************************** */

int get_rule_descriptor(rule_descriptor_t *rule_descriptor,
                        const unsigned int index, const uint8_t *context,
                        const size_t context_byte_len) {
  uint16_t rule_descriptor_offset;

  // context[1] represents the total number of Rule Descriptor in the Context
  if (index >= context[1] || 2 + 2 * index + 1 >= context_byte_len) {
    return 0;
  }

  rule_descriptor_offset =
      merge_uint8_t(context[2 + 2 * index], context[2 + 2 * index + 1]);

  rule_descriptor->offset = rule_descriptor_offset;
  rule_descriptor->id     = context[rule_descriptor_offset++];
  rule_descriptor->nature = (nature_t) context[rule_descriptor_offset++];
  rule_descriptor->card_rule_field_descriptor = context[rule_descriptor_offset];

  return 1;
}
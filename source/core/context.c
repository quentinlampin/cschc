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
#ifndef _RULE_FIELD_DESCRIPTOR_H_
#define _RULE_FIELD_DESCRIPTOR_H_

#include "core/actions.h"
#include "core/matching_operators.h"

#include <stddef.h>
#include <stdint.h>

typedef enum { DIR_UP = 0, DIR_DW, DIR_BI } direction_t;

typedef struct {
  uint16_t                           fid;
  uint16_t                           len;
  uint16_t                           pos;
  direction_t                        dir;
  matching_operator_t                mo;
  compression_decompression_action_t cda;
  uint16_t                           msb_len;
  uint8_t                            card_target_value;
} rule_field_desc_t;

void unpack_dir_mo_cda(direction_t *dir, matching_operator_t *mo,
                       compression_decompression_action_t *cda,
                       const uint8_t                       packed_value);

int get_rule_field_desc_info(rule_field_desc_t *rule_field_desc, int offset,
                             const uint8_t *context, const size_t context_len);

#endif  // _RULE_FIELD_DESCRIPTOR_H_
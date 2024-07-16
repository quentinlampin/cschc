#ifndef _RULE_DESCRIPTOR_H_
#define _RULE_DESCRIPTOR_H_

#include <stddef.h>
#include <stdint.h>

typedef enum {
  NATURE_COMPRESSION = 0,
  NATURE_NO_COMPRESSION,
  NATURE_FRAGMENTATION
} nature_t;

typedef struct {
  uint8_t  id;
  nature_t nature;
  uint8_t  card_rule_field_desc;
} rule_desc_t;

int get_rule_desc_info(rule_desc_t *rule_desc, int offset,
                       const uint8_t *context, const size_t context_len);

#endif  // _RULE_DESCRIPTOR_H_
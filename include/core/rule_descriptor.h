#ifndef _RULE_DESCRIPTOR_H_
#define _RULE_DESCRIPTOR_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Struct that defines a Rule Descriptor.
 */
typedef struct {
  uint16_t offset;
  uint8_t  id;
  nature_t nature;
  uint8_t  card_rule_field_descriptor;
} rule_descriptor_t;

/**
 * @brief Gets a Rule Descriptor thanks to an index.
 *
 * @param rule_descriptor Pointer to the Rule Descriptor to fill.
 * @param index Index of the Rule Descriptor to get in context.
 * @param context Pointer to the CSCHC Context.
 * @param context_byte_len Byte length of context.
 * @return The status code, 1 for success otherwise 0.
 */
int get_rule_descriptor(rule_descriptor_t *rule_descriptor,
                        const unsigned int index, const uint8_t *context,
                        const size_t context_byte_len);

#endif  // _RULE_DESCRIPTOR_H_
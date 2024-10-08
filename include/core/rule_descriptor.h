/**
 * @file rule_descriptor.h
 * @author Corentin Banier
 * @brief SCHC Rule Descriptor implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @details See Section 7.1 SCHC RFC 8724.
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _RULE_DESCRIPTOR_H_
#define _RULE_DESCRIPTOR_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

#define CARD_RULE_DESCRIPTOR_OFFSET 1  // Offset in CSCHC Context

/**
 * @brief Struct that defines a Rule Descriptor.
 */
typedef struct {
  uint16_t offset;  // Offset of the Rule Descriptor in a SCHC Context
  uint8_t  id;      // ID
  nature_t nature;  // Compression Nature
  uint8_t  card_rule_field_descriptor;  // Number of Rule Field Descriptor
} rule_descriptor_t;

/**
 * @brief Gets a Rule Descriptor thanks to an index.
 *
 * @param rule_descriptor Pointer to the Rule Descriptor to fill.
 * @param index Index of the Rule Descriptor to get in context.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of context.
 * @return The status code, 1 for success otherwise 0.
 */
int get_rule_descriptor(rule_descriptor_t *rule_descriptor,
                        const unsigned int index, const uint8_t *context,
                        const size_t context_byte_len);

#endif  // _RULE_DESCRIPTOR_H_
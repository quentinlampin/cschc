#ifndef _MATCHING_OPERATORS_H_
#define _MATCHING_OPERATORS_H_

#include "rule_field_descriptor.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Checks if the Field corresponds to the Target Value defined in the
 * current Rule Field Descriptor.
 *
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The matching result, 1 for success, otherwise 0.
 */
int MO_equal(const uint8_t*                 field,
             const rule_field_descriptor_t* rule_field_descriptor,
             const uint8_t* context, const size_t context_byte_len);

/**
 * @brief Basically does nothing (ignore).
 *
 * @return 1 for success.
 */
int MO_ignore(void);

/**
 * @brief Checks if the Most Significant Bits of the Field Value correspond to
 * the Target Value defined in the current Rule Field Descriptor.
 *
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The matching result, 1 for success, otherwise 0.
 */
int MO_most_significant_bits(
    const uint8_t* field, const rule_field_descriptor_t* rule_field_descriptor,
    const uint8_t* context, const size_t context_byte_len);

/**
 * @brief Checks if the Field Value corresponds to one of the Target Values
 * listed in the current Rule Field Descriptor.
 *
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The matching result, 1 for success, otherwise 0.
 */
int MO_match_mapping(const uint8_t*                 field,
                     const rule_field_descriptor_t* rule_field_descriptor,
                     const uint8_t* context, const size_t context_byte_len);

/**
 * @brief Checks if the Field Value corresponds to the Target Value directly
 * from its offset.
 *
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param target_value_offset Offset of the Target Value in the context.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The matching result, 1 for success, otherwise 0.
 */
int __MO_equal_from_offset(const uint8_t*                 field,
                           const rule_field_descriptor_t* rule_field_descriptor,
                           const uint16_t                 target_value_offset,
                           const uint8_t*                 context,
                           const size_t                   context_byte_len);

#endif  // _MATCHING_OPERATORS_H_
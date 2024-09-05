/**
 * @file actions.h
 * @author Corentin Banier and Quentin Lampin
 * @brief SCHC Compression Decompression Action implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @details See Section 7.4 SCHC RFC 8724.
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _COMPRESSION_DECOMPRESSION_ACTIONS_H_
#define _COMPRESSION_DECOMPRESSION_ACTIONS_H_

#include "rule_field_descriptor.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief (De)Compression Action which elides the Field only if the result of
 * MO_equal is true.
 *
 * @details See MO_equal(...) in matching_operators.h.
 *
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The (de)compression action status, 1 for success, otherwise 0.
 */
int CDA_not_sent(const uint8_t                 *field,
                 const rule_field_descriptor_t *rule_field_descriptor,
                 const uint8_t *context, const size_t context_byte_len);

/**
 * @brief Fill the Field Residue with the corresponding Least Significant Bits
 * of the Field.
 *
 * @param field_residue Pointer to the Field Residue to fill.
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The (de)compression action status, 1 for success, otherwise 0.
 *
 * @details A 0 return status means the MSB part doesn't match the one defined
 * in the current Rule Field Descriptor.
 */
int CDA_least_significant_bits(
    uint8_t *field_residue, const uint8_t *field,
    const rule_field_descriptor_t *rule_field_descriptor,
    const uint8_t *context, const size_t context_byte_len);

/**
 * @brief Fill the Field Residue with the corresponding index of the matching
 * Target Value among the list defined by the current Rule Field Descriptor.
 *
 * @param field_residue Pointer to the Field Residue to fill.
 * @param field Pointer to the Field Value.
 * @param rule_field_descriptor Pointer to the corresponding Rule Field
 * Descriptor.
 * @param context Pointer to the SCHC Context.
 * @param context_byte_len Byte length of the context.
 * @return The (de)compression action status, 1 for success, otherwise 0.
 *
 * @details A 0 return status means no Target Value described in the Rule Field
 * Descriptor matches the current field.
 */
int CDA_mapping_sent(uint8_t *field_residue, const uint8_t *field,
                     const rule_field_descriptor_t *rule_field_descriptor,
                     const uint8_t *context, const size_t context_byte_len);

/**
 * @brief Action which means the field is sent.
 *
 * @return 1 for success.
 */
int CDA_value_sent(void);

/**
 * @brief Action which means the field needs to be computed.
 *
 * @details This action is important only for decompression as, during
 * compression, the corresponding field with this action is elided (associated
 * with ignore MO). However, during decompression, we need to recompute the
 * initial value.
 *
 * @return 1 for success.
 */
int CDA_compute(void);

#endif  // _COMPRESSION_DECOMPRESSION_ACTIONS_H_
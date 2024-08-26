#ifndef _CONTEXT_H_
#define _CONTEXT_H_

/**
 * @brief In CSCHC, a SCHC Context is defined as a byte array.
 *
 * @details According to RFC 8724, a Context consists of various compression
 * Rule Descriptors. Each Rule Descriptor is composed of Rule Field Descriptors,
 * which may or may not include one or multiple Target Values.
 *
 * @note A Target Value of a Rule Field Descriptor can appear in different
 * places. Same idea for a Rule Field Descriptor. Therefore, the Context byte
 * array stores the offset of each object represented in order to optimize it
 * length.
 *
 * @struct
 * // Context
 * CTX_ID, NUM_RULE_DESC, RULE_DESC1_OFFSET_1, RULE_DESC1_OFFSET_2,
 *                          RULE_DESC2_OFFSET_1, RULE_DESC2_OFFSET_2,
 *                          ...,
 *
 * // Rule Descriptor
 * RULE_DESC_ID, RULE_NATURE, NUM_RULE_FIELD_DESC,
 *                 RULE_FIELD_DESC1_OFFSET_1, RULE_FIELD_DESC1_OFFSET_2,
 *                 RULE_FIELD_DESC2_OFFSET_1, RULE_FIELD_DESC2_OFFSET_2,
 *                 ...,
 * ...,
 *
 * // Rule Field Descriptor
 * RULE_FIELD_DESC_SID_1, RULE_FIELD_DESC_SID_2, LEN_1, LEN_2, POS_1, POS_2,
 * DIR_MO_CDA, (MSB_LEN_1, MSB_LEN_2), NUM_TARGET_VALUE, TARGET_VALUE1_OFFSET_1,
 * TARGET_VALUE1_OFFSET_2, TARGET_VALUE2_OFFSET_1, TARGET_VALUE2_OFFSET_2, ...,
 * ...,
 *
 * // Target Value
 * TARGET_VALUE1_1, TARGET_VALUE1_2, TARGET_VALUE1_3, TARGET_VALUE1_4,
 * TARGET_VALUE2_1,
 * TARGET_VALUE3_1, TARGET_VALUE3_2,
 * ...,
 */

#include "actions.h"
#include "matching_operators.h"
#include "rule_descriptor.h"
#include "rule_field_descriptor.h"
#include "schc8724.h"

/**
 * @brief Struct that stores the current bit position of a Rule Field Descriptor
 * which uses CDA_COMPUTE along with its position in the Rule Descriptor.
 */
typedef struct {
  size_t bit_position;  // Bit position of the compute entry in a packet
  int    index_rule_field_descriptor;  // Index of the corresponding Rule Field
                                       // Descriptor
} compute_entry_t;

/**
 * @brief Gets the number of CDA fields that are equal to CDA_COMPUTE among all
 * Rule Field Descriptors in a specific Rule Descriptor.
 *
 * @details As specified in SCHC RFC 8724, only a few fields can use the Compute
 * action. However, we can still use the Value-Sent action on these fields. This
 * function ensures that we know the correct number of fields to compute before
 * applying the compression, even though this function adds O(n) complexity.
 *
 * @param rule_descriptor Pointer to the Rule Descriptor used for decompression.
 * @param context Pointer to the SCHC Context used to perform decompression.
 * @param context_byte_len Byte length of the context.
 * @return The number of fields that use the Compute action.
 */
int get_cardinal_compute_entries(const rule_descriptor_t *rule_descriptor,
                                 const uint8_t           *context,
                                 const size_t             context_byte_len);

#endif  // _CONTEXT_H_
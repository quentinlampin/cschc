#ifndef _CONTEXT_H_
#define _CONTEXT_H_

/**
 * @brief A context is defined as a byte array.
 *
 * @details According to RFC 8724, a context consists of various compression
 * rules. Each rule is composed of rule field descriptors, which may or may not
 * include one or multiple target values.
 *
 * @note A target value of a rule field descriptor can appear in different
 * places. Same idea for a rule field descriptor and different rule descriptor.
 * Therefore, the context byte array stores the offset of each object
 * represented.
 *
 * @struct
 * // Context
 * CTX_ID, NUM_RULE_DESC, RULE_DESC1_OFFSET_1, RULE_DESC1_OFFSET_2,
 *                          RULE_DESC2_OFFSET_1, RULE_DESC2_OFFSET_2,
 *                          ...,
 *
 * // RuleDesc
 * RULE_DESC_ID, RULE_NATURE, NUM_RULE_FIELD_DESC,
 *                 RULE_FIELD_DESC1_OFFSET_1, RULE_FIELD_DESC1_OFFSET_2,
 *                 RULE_FIELD_DESC2_OFFSET_1, RULE_FIELD_DESC2_OFFSET_2,
 *                 ...,
 * ...,
 *
 * // RuleFieldDesc
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

#include "core/actions.h"
#include "core/matching_operators.h"
#include "core/rule_descriptor.h"
#include "core/rule_field_descriptor.h"

#include <stddef.h>
#include <stdint.h>

int get_rule_field_desc_offset_from_fid(int *offset, const uint16_t fid,
                                        const uint8_t *context,
                                        const size_t   context_len);

#endif  // _CONTEXT_H_
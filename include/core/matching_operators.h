#ifndef _MATCHING_OPERATORS_H_
#define _MATCHING_OPERATORS_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Enumeration which defines SCHC RFC 8724 matching operators.
 */
typedef enum {
  MO_EQUAL = 0,
  MO_IGNORE,
  MO_MSB,
  MO_MATCH_MAPPING
} matching_operator_t;

int MO_equal(const uint8_t* field, const size_t field_len,
             const uint16_t tv_offset, const uint8_t* context,
             const size_t context_len);

int MO_ignore(void);

int MO_most_significant_bits(const uint8_t* field, const size_t field_len,
                             const size_t msb_len, const uint16_t tv_offset,
                             const uint8_t* context, const size_t context_len);

int MO_match_mapping(const uint8_t* field, const size_t field_len,
                     const uint16_t list_tv_offset, const uint8_t* context,
                     const size_t context_len);

#endif  // _MATCHING_OPERATORS_H_
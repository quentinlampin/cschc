#ifndef _RFC_SCHC_8724_H_
#define _RFC_SCHC_8724_H_

/**
 * @brief Enumeration that defines SCHC RFC 8724 Nature.
 */
typedef enum {
  NATURE_COMPRESSION = 0,
  NATURE_NO_COMPRESSION,
  NATURE_FRAGMENTATION  // Not implemented
} nature_t;

/**
 * @brief Enumeration that defines SCHC Direction Indicator.
 *
 * @details See Section 7.1 in SCHC RFC 8724.
 */
typedef enum { DI_UP = 0, DI_DW, DI_BI } direction_indicator_t;

/**
 * @brief Enumeration that defines SCHC Matching Operators.
 *
 * @details See Section 7.3 in SCHC RFC 8724.
 */
typedef enum {
  MO_EQUAL = 0,
  MO_IGNORE,
  MO_MSB,
  MO_MATCH_MAPPING
} matching_operator_t;

/**
 * @brief Enumeration that defines SCHC RFC 8724 Compression Decompression
 * Actions.
 *
 * @details See Section 7.4 in SCHC RFC 8724.
 */
typedef enum {
  CDA_NOT_SENT = 0,
  CDA_LSB,
  CDA_MAPPING_SENT,
  CDA_VALUE_SENT,
  CDA_COMPUTE
} compression_decompression_action_t;

#endif  // _RFC_SCHC_8724_H_
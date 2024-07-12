#ifndef _COMPRESSION_DECOMPRESSION_ACTIONS_H_
#define _COMPRESSION_DECOMPRESSION_ACTIONS_H_

#include <stddef.h>
#include <stdint.h>

typedef enum {
  CDA_NOT_SENT = 0,
  CDA_LSB,
  CDA_MAPPING_SENT,
  CDA_VALUE_SENT,
  CDA_COMPUTE
} compression_decompression_action_t;

int CDA_not_sent(void);

int CDA_least_significant_bits(uint8_t *field_residue, const uint8_t *field,
                               const size_t field_len, const size_t lsb_len);

int CDA_mapping_sent(uint8_t *field_residue, const uint8_t *field,
                     const size_t field_len, const uint16_t list_tv_offset,
                     const uint8_t *context, const size_t context_len);

int CDA_value_sent(void);

int CDA_compute(void);  // not implemented yet

#endif  // _COMPRESSION_DECOMPRESSION_ACTIONS_H_
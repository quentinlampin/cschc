#include "core/matching_operators.h"
#include "utils/binarie.h"
#include "utils/log.h"

#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

int MO_equal(const uint8_t* field, const size_t field_len,
             const uint16_t tv_offset, const uint8_t* context,
             const size_t context_len) {
  if (tv_offset >= context_len) {
    return -1;
  }

  return (memcmp(field, context + tv_offset, BYTE_LENGTH(field_len)) == 0) ? 1
                                                                           : 0;
}

/* ********************************************************************** */

int MO_ignore(void) { return 1; }

/* ********************************************************************** */

int MO_most_significant_bits(const uint8_t* field, const size_t field_len,
                             const size_t msb_len, const uint16_t tv_offset,
                             const uint8_t* context, const size_t context_len) {
  if (msb_len >= field_len || tv_offset >= context_len) {
    return -1;
  }

  size_t  shifted_byte_len = BYTE_LENGTH(field_len);
  uint8_t shifted_field[shifted_byte_len];

  memcpy(shifted_field, field, shifted_byte_len);
  shifted_byte_len =
      right_shift(shifted_field, shifted_byte_len, field_len - msb_len);

  //  put to zero unnecessary part
  shifted_field[0] &= (1 << (msb_len % 8)) - 1;

  if (shifted_byte_len == -1) {
    return -1;
  }

  return (memcmp(shifted_field, context + tv_offset, shifted_byte_len) == 0)
             ? 1
             : 0;
}

/* ********************************************************************** */

int MO_match_mapping(const uint8_t* field, const size_t field_len,
                     const uint16_t list_tv_offset, const uint8_t* context,
                     const size_t context_len) {
  if (list_tv_offset >= context_len) {
    return -1;
  }

  uint8_t  nb_target_value;
  uint16_t tv_offset;
  int      is_matching_value;

  nb_target_value = context[list_tv_offset];

  for (uint8_t i = 0; i < nb_target_value; i++) {
    tv_offset = ((uint16_t) context[list_tv_offset + 1 + 2 * i]) << 8 |
                context[list_tv_offset + 2 + 2 * i];

    is_matching_value =
        MO_equal(field, field_len, tv_offset, context, context_len);

    if (is_matching_value) {
      break;
    }
  }

  return is_matching_value;
}
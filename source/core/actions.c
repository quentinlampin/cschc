#include "core/actions.h"
#include "core/matching_operators.h"
#include "utils/binarie.h"

#include <string.h>

/* ********************************************************************** */

int CDA_not_sent(void) { return 1; }

/* ********************************************************************** */

int CDA_least_significant_bits(uint8_t *field_residue, const uint8_t *field,
                               const size_t field_len, const size_t lsb_len) {
  // >= because if == the right action should be value_sent
  if (lsb_len >= field_len) {
    return 0;
  }

  size_t  field_byte_len;
  size_t  residue_byte_len;
  uint8_t mask;

  field_byte_len   = BYTE_LENGTH(field_len);
  residue_byte_len = BYTE_LENGTH(lsb_len);

  memcpy(field_residue, field + (field_byte_len - residue_byte_len),
         residue_byte_len);

  if (lsb_len % 8 != 0) {
    mask = (1 << (lsb_len % 8)) - 1;
    field_residue[0] &= mask;
  }

  return 1;
}

/* ********************************************************************** */

int CDA_mapping_sent(uint8_t *field_residue, const uint8_t *field,
                     const size_t field_len, const uint16_t list_tv_offset,
                     const uint8_t *context, const size_t context_len) {
  if (list_tv_offset >= context_len) {
    return 0;
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
      *field_residue = i;
      break;
    }
  }

  return is_matching_value;
}

/* ********************************************************************** */

int CDA_value_sent(void) { return 1; }
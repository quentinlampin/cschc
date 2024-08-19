#include "matching_operators.h"
#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */

int MO_equal(const uint8_t*                 field,
             const rule_field_descriptor_t* rule_field_descriptor,
             const uint8_t* context, const size_t context_byte_len) {
  int     status;
  uint8_t mask;

  mask = (1 << (8 - (rule_field_descriptor->len % 8))) - 1;

  // Testing the first byte
  // We mask it to optimize context definition, allowing the same target value
  // to be used for different rules with varying lengths. For example, using
  // 0x37, 0x45 on 20 bits means the real value is 0x07, 0x45, whereas on 23
  // bits it is still 0x37, 0x45.
  status = (field[0] & mask) ==
           context[rule_field_descriptor->first_target_value_offset];

  if (status && rule_field_descriptor->len > 8) {
    status =
        memcmp(field + 1,
               context + rule_field_descriptor->first_target_value_offset + 1,
               BYTE_LENGTH(rule_field_descriptor->len) - 1) == 0;
  }

  return status;
}

/* ********************************************************************** */

int MO_ignore(void) { return 1; }

/* ********************************************************************** */

int MO_most_significant_bits(
    const uint8_t* field, const rule_field_descriptor_t* rule_field_descriptor,
    const uint8_t* context, const size_t context_len) {
  size_t  msb_field_byte_len = BYTE_LENGTH(rule_field_descriptor->len);
  uint8_t msb_field[msb_field_byte_len];

  memcpy(msb_field, field, msb_field_byte_len);
  msb_field_byte_len =
      right_shift(msb_field, msb_field_byte_len,
                  rule_field_descriptor->len - rule_field_descriptor->msb_len);

  if (!msb_field_byte_len) {
    return 0;
  }

  //  put to zero unnecessary part
  if (rule_field_descriptor->msb_len % 8 != 0) {
    msb_field[0] &= (1 << (rule_field_descriptor->msb_len % 8)) - 1;
  }

  return (memcmp(msb_field,
                 context + rule_field_descriptor->first_target_value_offset,
                 msb_field_byte_len) == 0)
             ? 1
             : 0;
}

/* ********************************************************************** */

int MO_match_mapping(const uint8_t*                 field,
                     const rule_field_descriptor_t* rule_field_descriptor,
                     const uint8_t* context, const size_t context_byte_len) {
  uint16_t target_value_offset;

  if (rule_field_descriptor->card_target_value == 1) {
    return MO_equal(field, rule_field_descriptor, context, context_byte_len);
  }

  for (uint8_t i = 0; i < rule_field_descriptor->card_target_value; i++) {
    target_value_offset =
        ((uint16_t)
             context[rule_field_descriptor->first_target_value_offset + 2 * i])
            << 8 |
        context[rule_field_descriptor->first_target_value_offset + 2 * i + 1];

    if (__MO_equal_from_offset(field, rule_field_descriptor,
                               target_value_offset, context,
                               context_byte_len)) {
      return 1;
    }
  }

  return 0;
}

/* ********************************************************************** */

int __MO_equal_from_offset(const uint8_t*                 field,
                           const rule_field_descriptor_t* rule_field_descriptor,
                           const uint16_t                 target_value_offset,
                           const uint8_t*                 context,
                           const size_t                   context_byte_len) {
  int     status;
  uint8_t mask;

  mask = (1 << (8 - (rule_field_descriptor->len % 8))) - 1;

  // Test the first byte
  // We mask it to optimize context definition, allowing the same target value
  // to be used for different rules with varying lengths. For example, using
  // 0x37, 0x45 on 20 bits means the real value is 0x07, 0x45, whereas on 23
  // bits it is still 0x37, 0x45.
  status = (field[0] & mask) == context[target_value_offset];

  if (rule_field_descriptor->len > 8) {
    status = status && memcmp(field + 1, context + target_value_offset + 1,
                              BYTE_LENGTH(rule_field_descriptor->len) - 1) == 0;
  }

  return status;
}
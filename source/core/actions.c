#include "actions.h"
#include "matching_operators.h"
#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */

int CDA_not_sent(const uint8_t                 *field,
                 const rule_field_descriptor_t *rule_field_descriptor,
                 const uint8_t *context, const size_t context_byte_len) {
  return MO_equal(field, rule_field_descriptor, context, context_byte_len);
}

/* ********************************************************************** */

int CDA_least_significant_bits(
    uint8_t *field_residue, const uint8_t *field,
    const rule_field_descriptor_t *rule_field_descriptor,
    const uint8_t *context, const size_t context_byte_len) {
  size_t  lsb_len;
  size_t  field_byte_len;
  size_t  residue_byte_len;
  uint8_t mask;

  // If the MSB of field corresponds to the rule_field_descriptor then we
  // perform LSB. Else we return 0 as an error, meaning the rule do not
  // correspond to the current field.
  if (!MO_most_significant_bits(field, rule_field_descriptor, context,
                                context_byte_len)) {
    return 0;
  }

  lsb_len        = rule_field_descriptor->len - rule_field_descriptor->msb_len;
  field_byte_len = BYTE_LENGTH(rule_field_descriptor->len);
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
                     const rule_field_descriptor_t *rule_field_descriptor,
                     const uint8_t *context, const size_t context_byte_len) {
  uint16_t target_value_offset;

  if (rule_field_descriptor->card_target_value == 1) {
    if (MO_equal(field, rule_field_descriptor, context, context_byte_len)) {
      *field_residue = 0;
      return 1;
    }
    return 0;
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
      *field_residue = i;
      return 1;
    }
  }

  return 0;
}

/* ********************************************************************** */

int CDA_value_sent(void) { return MO_ignore(); }

/* ********************************************************************** */

int CDA_compute(void) { return MO_ignore(); }
#include "core/actions.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_CDA_not_sent(void) {
  // Nothing to test
  assert(CDA_not_sent());
}

/* ********************************************************************** */

void test_CDA_least_significant_bits(void) {
  const uint8_t field1[] = {0b00011010, 0b11010011};
  const uint8_t field2[] = {0b10011010, 0b01011010, 0b11010011};

  const uint8_t field1_expected_lsb2[] = {0b00000011};
  const uint8_t field1_expected_lsb8[] = {0b11010011};

  const uint8_t field2_expected_lsb7[]  = {0b01010011};
  const uint8_t field2_expected_lsb16[] = {0b01011010, 0b11010011};
  const uint8_t field2_expected_lsb20[] = {0b00001010, 0b01011010, 0b11010011};

  uint8_t residue[3];

  assert(CDA_least_significant_bits(residue, field1, 13, 2));
  assert(memcmp(residue, field1_expected_lsb2, 1) == 0);

  assert(CDA_least_significant_bits(residue, field1, 13, 8));
  assert(memcmp(residue, field1_expected_lsb8, 1) == 0);

  assert(!CDA_least_significant_bits(residue, field1, 13, 13));

  assert(CDA_least_significant_bits(residue, field2, 24, 7));
  assert(memcmp(residue, field2_expected_lsb7, 1) == 0);

  assert(CDA_least_significant_bits(residue, field2, 24, 16));
  assert(memcmp(residue, field2_expected_lsb16, 2) == 0);

  assert(CDA_least_significant_bits(residue, field2, 24, 20));
  assert(memcmp(residue, field2_expected_lsb20, 3) == 0);
}

/* ********************************************************************** */

void test_CDA_mapping_sent(void) {
  uint8_t ctx_example[] = {
      // ...

      // RuleFieldDesc
      66, 73, 0, 8, 0, 1, 90, 2, 0, 36, 0, 38,          // list_tv_offset : 7
      66, 73, 0, 8, 0, 1, 90, 1, 0, 37,                 // list_tv_offset : 19
      66, 72, 0, 20, 0, 1, 90, 3, 0, 39, 0, 42, 0, 45,  // list_tv_offset : 29

      // TargetValue
      6,            // 36
      0,            // 37
      23,           // 38
      15, 88, 15,   // 39
      9, 170, 242,  // 42
      7, 7, 7       // 45
  };
  size_t ctx_len = sizeof(ctx_example);

  const uint8_t field1[] = {23};
  const uint8_t field2[] = {0};
  const uint8_t field3[] = {15, 88, 15};
  const uint8_t field4[] = {7, 7, 7};

  const uint8_t residue_expected1[] = {1};
  const uint8_t residue_expected2[] = {0};
  const uint8_t residue_expected3[] = {0};
  const uint8_t residue_expected4[] = {2};

  uint8_t residue[1];

  // field1 in RuleFieldDesc1
  assert(CDA_mapping_sent(residue, field1, 8, 7, ctx_example, ctx_len));
  assert(memcmp(residue, residue_expected1, 1) == 0);

  // field1 not in RuleFieldDesc2
  assert(!CDA_mapping_sent(residue, field1, 8, 19, ctx_example, ctx_len));

  // field2 not in RuleFieldDesc1
  assert(!CDA_mapping_sent(residue, field2, 8, 7, ctx_example, ctx_len));

  // field2 in RuleFieldDesc2
  assert(CDA_mapping_sent(residue, field2, 8, 19, ctx_example, ctx_len));
  assert(memcmp(residue, residue_expected2, 1) == 0);

  // field3 in RuleFieldDesc3
  assert(CDA_mapping_sent(residue, field3, 8, 29, ctx_example, ctx_len));
  assert(memcmp(residue, residue_expected3, 1) == 0);

  // field4 in RuleFieldDesc3
  assert(CDA_mapping_sent(residue, field4, 8, 29, ctx_example, ctx_len));
  assert(memcmp(residue, residue_expected4, 1) == 0);
}

/* ********************************************************************** */

void test_CDA_value_sent(void) {
  // Nothing to test
  assert(CDA_not_sent());
}

/* ********************************************************************** */

int main(void) {
  test_CDA_not_sent();
  test_CDA_least_significant_bits();
  test_CDA_mapping_sent();
  test_CDA_value_sent();

  printf("All tests passed!\n");

  return 0;
}
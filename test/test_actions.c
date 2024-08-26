#include "core/actions.h"
#include "utils/memory.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_CDA_not_sent(void) {
  const uint8_t short_context[] = {
      // ...

      // Rule Descriptor
      0, 0, 5, 0, 13, 0, 23, 0, 33, 0, 43, 0, 53,

      // Rule Field Descriptor
      0x13, 0x01, 0, 4, 0, 1, 64, 1, 0, 63,   // 13
      0x13, 0x02, 0, 8, 0, 1, 64, 1, 0, 64,   // 23
      0x13, 0x03, 0, 20, 0, 1, 64, 1, 0, 65,  // 33
      0x13, 0x04, 0, 2, 0, 1, 64, 1, 0, 68,   // 43
      0x13, 0x05, 0, 9, 0, 1, 64, 1, 0, 69,   // 53

      // Target Value
      0x06,              // 63
      0xf4,              // 64
      0x07, 0xde, 0xa2,  // 65
      0x03,              // 68
      0x01, 0x7a         // 69
  };
  const size_t short_context_byte_len = sizeof(short_context);

  rule_field_descriptor_t rule_field_descriptor;

  /**
   * @brief Rule Field Descriptor 0
   *
   * @details Target Value associated with RFD 0 is {0x06} on 4 bits.
   *
   * - CDA_not_sent on {0x04} return 0
   * - CDA_not_sent on {0x06} return 1
   * - CDA_not_sent on {0x60} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_0[] = {0x04};
  const uint8_t value_2_rule_field_0[] = {0x06};
  const uint8_t value_3_rule_field_0[] = {0x60};

  assert(!CDA_not_sent(value_1_rule_field_0, &rule_field_descriptor,
                       short_context, short_context_byte_len));
  assert(CDA_not_sent(value_2_rule_field_0, &rule_field_descriptor,
                      short_context, short_context_byte_len));
  assert(!CDA_not_sent(value_3_rule_field_0, &rule_field_descriptor,
                       short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Value associated with RFD 1 is {0xf4} on 8 bits.
   *
   * - CDA_not_sent on {0x00} return 0
   * - CDA_not_sent on {0xff} return 0
   * - CDA_not_sent on {0xf4} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_1[] = {0x00};
  const uint8_t value_2_rule_field_1[] = {0xff};
  const uint8_t value_3_rule_field_1[] = {0xf4};

  assert(!CDA_not_sent(value_1_rule_field_1, &rule_field_descriptor,
                       short_context, short_context_byte_len));
  assert(!CDA_not_sent(value_2_rule_field_1, &rule_field_descriptor,
                       short_context, short_context_byte_len));
  assert(CDA_not_sent(value_3_rule_field_1, &rule_field_descriptor,
                      short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Value associated with RFD 2 is {0x07, 0xde, 0xa2} on 20
   * bits.
   *
   * - CDA_not_sent on {0x37, 0xde, 0xa2} return 1
   * - CDA_not_sent on {0x07, 0xde, 0xa3} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_2[] = {0x37, 0xde, 0xa2};
  const uint8_t value_2_rule_field_2[] = {0x07, 0xde, 0xa3};

  assert(CDA_not_sent(value_1_rule_field_2, &rule_field_descriptor,
                      short_context, short_context_byte_len));
  assert(!CDA_not_sent(value_2_rule_field_2, &rule_field_descriptor,
                       short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 3
   *
   * @details Target Value associated with RFD 3 is {0x03} on 2 bits.
   *
   * - CDA_not_sent on {0x03} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 3, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_3[] = {0x03};

  assert(CDA_not_sent(value_1_rule_field_3, &rule_field_descriptor,
                      short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 4
   *
   * @details Target Value associated with RFD 4 is {0x01, 0x7a} on 9 bits.
   *
   * - CDA_not_sent on {0x02, 0x7a} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 4, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_4[] = {0x02, 0x7a};

  assert(!CDA_not_sent(value_1_rule_field_4, &rule_field_descriptor,
                       short_context, short_context_byte_len));
}

/* ********************************************************************** */

void test_CDA_least_significant_bits(void) {
  const uint8_t short_context[] = {
      // ...
      // Rule Descriptor
      0, 0, 5, 0, 13, 0, 25, 0, 37, 0, 49, 0, 61,

      // Rule Field Descriptor
      0x13, 0x01, 0, 13, 0, 1, 81, 0, 11, 1, 0, 73,  // 13
      0x13, 0x01, 0, 13, 0, 1, 81, 0, 5, 1, 0, 75,   // 25
      0x13, 0x02, 0, 24, 0, 1, 81, 0, 17, 1, 0, 76,  // 37
      0x13, 0x02, 0, 24, 0, 1, 81, 0, 9, 1, 0, 79,   // 49
      0x13, 0x02, 0, 24, 0, 1, 81, 0, 4, 1, 0, 81,   // 61

      // Target Value
      0x06, 0xb4,        // 73
      0x1a,              // 75
      0x01, 0x34, 0xb5,  // 76
      0x01, 0x34,        // 79
      0x09               // 81
  };
  const size_t short_context_byte_len = sizeof(short_context);

  uint8_t                 field_residue[3];
  rule_field_descriptor_t rule_field_descriptor;

  const uint8_t value_1[] = {0x1a, 0xd3};
  const uint8_t value_2[] = {0x9a, 0x5a, 0xd3};
  const uint8_t value_3[] = {0x1a, 0x13};
  const uint8_t value_4[] = {0x9a, 0x80, 0xd3};

  /**
   * @brief Rule Field Descriptor 0
   *
   * @details Target Value associated with RFD 0 is {0b00000110, 0b10110100}
   * define on 13 bits. The MSB length is 11 bits, leading to LSB length of 2
   * bits.
   *
   * - LSB(2) on {0b00011010, 0b11010011} return 1 with
   * field_residue = {0b00000011}
   * - LSB(2) on {0b00011010, 0b00010011} return 0 because MSB(11) doesn't
   * corresponds to the Target Value.
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_expected_rfd0[] = {0x03};
  assert(CDA_least_significant_bits(field_residue, value_1,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_1_expected_rfd0, 1) == 0);

  assert(!CDA_least_significant_bits(field_residue, value_3,
                                     &rule_field_descriptor, short_context,
                                     short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Value associated with RFD 1 is {0b00011010}
   * define on 13 bits. The MSB length is 5 bits, leading to LSB length of 8
   * bits.
   *
   * - LSB(8) on {0b00011010, 0b11010011} return 1 with
   * field_residue = {0b00000011}
   * - LSB(8) on {0b00011010, 0b00010011} return 1 with
   * field_residue = {0b00010011}
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_expected_rfd1[] = {0xd3};
  assert(CDA_least_significant_bits(field_residue, value_1,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_1_expected_rfd1, 1) == 0);

  const uint8_t value_3_expected_rfd1[] = {0x13};
  assert(CDA_least_significant_bits(field_residue, value_3,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_3_expected_rfd1, 1) == 0);

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Value associated with RFD 2 is {0b00000001, 0b00110100,
   * 0b10110101} define on 24 bits. The MSB length is 17 bits, leading to LSB
   * length of 7 bits.
   *
   * - LSB(7) on {0b10011010, 0b01011010, 0b11010011} return 1 with
   * field_residue = {0b01010011}
   * - LSB(7) on {0b10011010, 0b10000000, 0b11010011} return 0 because MSB(17)
   * doesn't corresponds to the Target Value.
   */
  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_2_expected_rfd2[] = {0x53};
  assert(CDA_least_significant_bits(field_residue, value_2,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_2_expected_rfd2, 1) == 0);

  assert(!CDA_least_significant_bits(field_residue, value_4,
                                     &rule_field_descriptor, short_context,
                                     short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 3
   *
   * @details Target Value associated with RFD 3 is {0b00000001, 0b00110100}
   * define on 24 bits. The MSB length is 9 bits, leading to LSB length of 15
   * bits.
   *
   * - LSB(15) on {0b10011010, 0b01011010, 0b11010011} return 1 with
   * field_residue = {0b01011010, 0b11010011}
   * - LSB(15) on {0b10011010, 0b10000000, 0b11010011} return 0 because MSB(9)
   * doesn't corresponds to the Target Value.
   */
  get_rule_field_descriptor(&rule_field_descriptor, 3, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_2_expected_rfd3[] = {0x5a, 0xd3};
  assert(CDA_least_significant_bits(field_residue, value_2,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_2_expected_rfd3, 1) == 0);

  assert(!CDA_least_significant_bits(field_residue, value_4,
                                     &rule_field_descriptor, short_context,
                                     short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 4
   *
   * @details Target Value associated with RFD 4 is {0b00001001} define on 24
   * bits. The MSB length is 20 bits, leading to LSB length of 20 bits.
   *
   * - LSB(20) on {0b10011010, 0b01011010, 0b11010011} return 1 with
   * field_residue = {0b00001010, 0b01011010, 0b11010011}
   * - LSB(20) on {0b10011010, 0b10000000, 0b11010011} 1 with
   * field_residue = {0b00001010, 0b10000000, 0b11010011}
   */
  get_rule_field_descriptor(&rule_field_descriptor, 4, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_2_expected_rfd4[] = {0x0a, 0x5a, 0xd3};
  assert(CDA_least_significant_bits(field_residue, value_2,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_2_expected_rfd4, 3) == 0);

  const uint8_t value_4_expected_rfd4[] = {0x0a, 0x80, 0xd3};
  assert(CDA_least_significant_bits(field_residue, value_4,
                                    &rule_field_descriptor, short_context,
                                    short_context_byte_len));
  assert(memcmp(field_residue, value_4_expected_rfd4, 3) == 0);
}

/* ********************************************************************** */

void test_CDA_mapping_sent(void) {
  const uint8_t short_context[] = {
      // ...
      // Rule Descriptor
      0, 0, 3, 0, 9, 0, 21, 0, 31,

      // Rule Field Descriptor
      0x13, 0x01, 0, 8, 0, 1, 90, 2, 0, 45, 0, 47,          //  9
      0x13, 0x01, 0, 8, 0, 1, 90, 1, 0, 46,                 // 21
      0x13, 0x02, 0, 20, 0, 1, 90, 3, 0, 54, 0, 51, 0, 48,  // 31

      // Target Value
      6,            // 45
      0,            // 46
      23,           // 47
      15, 88, 15,   // 48
      9, 170, 242,  // 51
      7, 7, 7       // 54
  };
  const size_t short_context_byte_len = sizeof(short_context);

  uint8_t                 field_residue[3];
  rule_field_descriptor_t rule_field_descriptor;

  /**
   * @brief Rule Field Descriptor 0
   *
   * @details Target Values associated with RFD 0 (8 bits) are :
   * - {6}
   * - {23}
   *
   * - CDA_mapping_sent on {6}  return 1 with field_residue = {0}
   * - CDA_mapping_sent on {38} return 0
   * - CDA_mapping_sent on {0}  return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_0_1[] = {6};
  const uint8_t value_2_rule_field_0_1[] = {38};
  const uint8_t value_3_rule_field_0_1[] = {0};

  const uint8_t expected_index_value_1_rfd0[] = {0};

  assert(CDA_mapping_sent(field_residue, value_1_rule_field_0_1,
                          &rule_field_descriptor, short_context,
                          short_context_byte_len));
  assert(memcmp(field_residue, expected_index_value_1_rfd0, 1) == 0);

  assert(!CDA_mapping_sent(field_residue, value_2_rule_field_0_1,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));

  assert(!CDA_mapping_sent(field_residue, value_3_rule_field_0_1,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Values associated with RFD 1 (8 bits) is {0}
   *
   * - CDA_mapping_sent on {6}  return 0
   * - CDA_mapping_sent on {38} return 0
   * - CDA_mapping_sent on {0}  return 1 with field_residue = {0}
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  assert(!CDA_mapping_sent(field_residue, value_1_rule_field_0_1,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));

  assert(!CDA_mapping_sent(field_residue, value_2_rule_field_0_1,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));

  const uint8_t expected_index_value_3_rfd1[] = {0};
  assert(CDA_mapping_sent(field_residue, value_3_rule_field_0_1,
                          &rule_field_descriptor, short_context,
                          short_context_byte_len));
  assert(memcmp(field_residue, expected_index_value_3_rfd1, 1) == 0);

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Values associated with RFD 2 (20 bits) are :
   * - { 9, 170, 242}
   * - { 7,   7,   7}
   * - {15,  88,  15}
   *
   * - CDA_mapping_sent on {15, 88, 15} return 1 with field_residue = {2}
   * - CDA_mapping_sent on { 7,  7,  0} return 0
   * - CDA_mapping_sent on { 0,  7,  7} return 0
   */

  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_2[] = {15, 88, 15};
  const uint8_t value_2_rule_field_2[] = {7, 7, 0};
  const uint8_t value_3_rule_field_2[] = {0, 7, 7};

  const uint8_t expected_index_value_1_rfd2[] = {2};

  assert(CDA_mapping_sent(field_residue, value_1_rule_field_2,
                          &rule_field_descriptor, short_context,
                          short_context_byte_len));
  assert(memcmp(field_residue, expected_index_value_1_rfd2, 1) == 0);

  assert(!CDA_mapping_sent(field_residue, value_2_rule_field_2,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));

  assert(!CDA_mapping_sent(field_residue, value_3_rule_field_2,
                           &rule_field_descriptor, short_context,
                           short_context_byte_len));
}

/* ********************************************************************** */

void test_CDA_value_sent(void) {
  // Nothing to test
  assert(CDA_value_sent());
}

/* ********************************************************************** */

void test_CDA_compute(void) {
  // Nothing to test
  assert(CDA_compute());
}

/* ********************************************************************** */

int main(void) {
  test_CDA_not_sent();

  init_memory_pool();
  test_CDA_least_significant_bits();  // Needs the pool to be allocated
  destroy_memory_pool();

  test_CDA_mapping_sent();
  test_CDA_value_sent();
  test_CDA_compute();

  printf("All tests passed!\n");

  return 0;
}
#include "core/matching_operators.h"
#include "utils/memory.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_MO_equal(void) {
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
   * - MO_equal on {0x04} return 0
   * - MO_equal on {0x06} return 1
   * - MO_equal on {0x60} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_0[] = {0x04};
  const uint8_t value_2_rule_field_0[] = {0x06};
  const uint8_t value_3_rule_field_0[] = {0x60};

  assert(!MO_equal(value_1_rule_field_0, &rule_field_descriptor, short_context,
                   short_context_byte_len));
  assert(MO_equal(value_2_rule_field_0, &rule_field_descriptor, short_context,
                  short_context_byte_len));
  assert(!MO_equal(value_3_rule_field_0, &rule_field_descriptor, short_context,
                   short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Value associated with RFD 1 is {0xf4} on 8 bits.
   *
   * - MO_equal on {0x00} return 0
   * - MO_equal on {0xff} return 0
   * - MO_equal on {0xf4} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_1[] = {0x00};
  const uint8_t value_2_rule_field_1[] = {0xff};
  const uint8_t value_3_rule_field_1[] = {0xf4};

  assert(!MO_equal(value_1_rule_field_1, &rule_field_descriptor, short_context,
                   short_context_byte_len));
  assert(!MO_equal(value_2_rule_field_1, &rule_field_descriptor, short_context,
                   short_context_byte_len));
  assert(MO_equal(value_3_rule_field_1, &rule_field_descriptor, short_context,
                  short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Value associated with RFD 2 is {0x07, 0xde, 0xa2} on 20
   * bits.
   *
   * - MO_equal on {0x37, 0xde, 0xa2} return 1
   * - MO_equal on {0x07, 0xde, 0xa3} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_2[] = {0x37, 0xde, 0xa2};
  const uint8_t value_2_rule_field_2[] = {0x07, 0xde, 0xa3};

  assert(MO_equal(value_1_rule_field_2, &rule_field_descriptor, short_context,
                  short_context_byte_len));
  assert(!MO_equal(value_2_rule_field_2, &rule_field_descriptor, short_context,
                   short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 3
   *
   * @details Target Value associated with RFD 3 is {0x03} on 2 bits.
   *
   * - MO_equal on {0x03} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 3, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_3[] = {0x03};

  assert(MO_equal(value_1_rule_field_3, &rule_field_descriptor, short_context,
                  short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 4
   *
   * @details Target Value associated with RFD 4 is {0x01, 0x7a} on 9 bits.
   *
   * - MO_equal on {0x02, 0x7a} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 4, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_4[] = {0x02, 0x7a};

  assert(!MO_equal(value_1_rule_field_4, &rule_field_descriptor, short_context,
                   short_context_byte_len));
}

/* ********************************************************************** */

void test_MO_ignore(void) {
  // Nothing to test
  assert(MO_ignore());
}

/* ********************************************************************** */

void test_MO_most_significant_bits(void) {
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

  rule_field_descriptor_t rule_field_descriptor;

  const uint8_t value_1_rule_field_0_1[] = {0x1a, 0xd3};
  const uint8_t value_2_rule_field_0_1[] = {0xfa, 0xd3};
  const uint8_t value_3_rule_field_0_1[] = {0x1a, 0xd2};
  const uint8_t value_4_rule_field_0_1[] = {0x1a, 0x53};

  const uint8_t value_1_rule_field_2_3_4[] = {0x9a, 0x5a, 0xff};
  const uint8_t value_2_rule_field_2_3_4[] = {0x9a, 0x52, 0xd3};
  const uint8_t value_3_rule_field_2_3_4[] = {0xfa, 0x5a, 0xd3};
  const uint8_t value_4_rule_field_2_3_4[] = {0x96, 0x5a, 0xd3};
  /**
   * @brief Rule Field Descriptor 0
   *
   * @details Target Value associated with RFD 0 is {0b00000110, 0b10110100}
   * define on 13 bits. The MSB length is 11 bits.
   *
   * - MO_most_significant_bits on {0b00011010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b11111010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b00011010, 0b11010010} return 1
   * - MO_most_significant_bits on {0b00011010, 0b01010011} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  assert(MO_most_significant_bits(value_1_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_2_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_3_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(!MO_most_significant_bits(value_4_rule_field_0_1,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Value associated with RFD 1 is {0b00011010}
   * define on 13 bits. The MSB length is 5 bits.
   *
   * - MO_most_significant_bits on {0b00011010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b11111010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b00011010, 0b11010010} return 1
   * - MO_most_significant_bits on {0b00011010, 0b01010011} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  assert(MO_most_significant_bits(value_1_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_2_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_3_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_4_rule_field_0_1,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Value associated with RFD 2 is {0b00000001, 0b00110100,
   * 0b10110101} define on 24 bits. The MSB length is 17 bits.
   *
   * - MO_most_significant_bits on {0b10011010, 0b01011010, 0b11111111} return 1
   * - MO_most_significant_bits on {0b10011010, 0b01010010, 0b11010011} -->
   * false
   * - MO_most_significant_bits on {0b11111010, 0b01011010, 0b11010011} -->
   * false
   * - MO_most_significant_bits on {0b10010110, 0b01011010, 0b11010011} -->
   * false
   */
  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  assert(MO_most_significant_bits(value_1_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(!MO_most_significant_bits(value_2_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  assert(!MO_most_significant_bits(value_3_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  assert(!MO_most_significant_bits(value_4_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  /**
   * @brief Rule Field Descriptor 3
   *
   * @details Target Value associated with RFD 3 is {0b00000001, 0b00110100}
   * define on 24 bits. The MSB length is 9 bits.
   *
   * - MO_most_significant_bits on {0b10011010, 0b01011010, 0b11111111} return 1
   * - MO_most_significant_bits on {0b10011010, 0b01010010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b11111010, 0b01011010, 0b11010011} -->
   * false
   * - MO_most_significant_bits on {0b10010110, 0b01011010, 0b11010011} -->
   * false
   */
  get_rule_field_descriptor(&rule_field_descriptor, 3, 0, short_context,
                            short_context_byte_len);

  assert(MO_most_significant_bits(value_1_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_2_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(!MO_most_significant_bits(value_3_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  assert(!MO_most_significant_bits(value_4_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  /**
   * @brief Rule Field Descriptor 4
   *
   * @details Target Value associated with RFD 4 is {0b00001001} define on 24
   * bits. The MSB length is 4 bits.
   *
   * - MO_most_significant_bits on {0b10011010, 0b01011010, 0b11111111} return 1
   * - MO_most_significant_bits on {0b10011010, 0b01010010, 0b11010011} return 1
   * - MO_most_significant_bits on {0b11111010, 0b01011010, 0b11010011} -->
   * false
   * - MO_most_significant_bits on {0b10010110, 0b01011010, 0b11010011} return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 4, 0, short_context,
                            short_context_byte_len);

  assert(MO_most_significant_bits(value_1_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(MO_most_significant_bits(value_2_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
  assert(!MO_most_significant_bits(value_3_rule_field_2_3_4,
                                   &rule_field_descriptor, short_context,
                                   short_context_byte_len));
  assert(MO_most_significant_bits(value_4_rule_field_2_3_4,
                                  &rule_field_descriptor, short_context,
                                  short_context_byte_len));
}

/* ********************************************************************** */

void test_MO_match_mapping(void) {
  const uint8_t short_context[] = {
      // ...
      // Rule Descriptor
      0, 0, 3, 0, 9, 0, 21, 0, 31,

      // Rule Field Descriptor
      0x13, 0x01, 0, 8, 0, 1, 90, 2, 0, 45, 0, 47,          //  9
      0x13, 0x01, 0, 8, 0, 1, 90, 1, 0, 46,                 // 21
      0x13, 0x02, 0, 20, 0, 1, 90, 3, 0, 48, 0, 51, 0, 54,  // 31

      // Target Value
      6,            // 45
      0,            // 46
      23,           // 47
      15, 88, 15,   // 48
      9, 170, 242,  // 51
      7, 7, 7       // 54
  };
  const size_t short_context_byte_len = sizeof(short_context);

  rule_field_descriptor_t rule_field_descriptor;

  /**
   * @brief Rule Field Descriptor 0
   *
   * @details Target Values associated with RFD 0 (8 bits) are :
   * - {6}
   * - {23}
   *
   * - MO_match_mapping on {6}  return 1
   * - MO_match_mapping on {38} return 0
   * - MO_match_mapping on {0}  return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 0, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_0_1[] = {6};
  const uint8_t value_2_rule_field_0_1[] = {38};
  const uint8_t value_3_rule_field_0_1[] = {0};

  assert(MO_match_mapping(value_1_rule_field_0_1, &rule_field_descriptor,
                          short_context, short_context_byte_len));
  assert(!MO_match_mapping(value_2_rule_field_0_1, &rule_field_descriptor,
                           short_context, short_context_byte_len));
  assert(!MO_match_mapping(value_3_rule_field_0_1, &rule_field_descriptor,
                           short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 1
   *
   * @details Target Values associated with RFD 1 (8 bits) is {0}
   *
   * - MO_match_mapping on {6}  return 0
   * - MO_match_mapping on {38} return 0
   * - MO_match_mapping on {0}  return 1
   */
  get_rule_field_descriptor(&rule_field_descriptor, 1, 0, short_context,
                            short_context_byte_len);

  assert(!MO_match_mapping(value_1_rule_field_0_1, &rule_field_descriptor,
                           short_context, short_context_byte_len));
  assert(!MO_match_mapping(value_2_rule_field_0_1, &rule_field_descriptor,
                           short_context, short_context_byte_len));
  assert(MO_match_mapping(value_3_rule_field_0_1, &rule_field_descriptor,
                          short_context, short_context_byte_len));

  /**
   * @brief Rule Field Descriptor 2
   *
   * @details Target Values associated with RFD 2 (20 bits) are :
   * - {15,  88,  15}
   * - { 9, 170, 242}
   * - { 7,   7,   7}
   *
   * - MO_match_mapping on {15, 88, 15} return 1
   * - MO_match_mapping on { 7,  7,  0} return 0
   * - MO_match_mapping on { 0,  7,  7} return 0
   */
  get_rule_field_descriptor(&rule_field_descriptor, 2, 0, short_context,
                            short_context_byte_len);

  const uint8_t value_1_rule_field_2[] = {15, 88, 15};
  const uint8_t value_2_rule_field_2[] = {7, 7, 0};
  const uint8_t value_3_rule_field_2[] = {0, 7, 7};

  assert(MO_match_mapping(value_1_rule_field_2, &rule_field_descriptor,
                          short_context, short_context_byte_len));
  assert(!MO_match_mapping(value_2_rule_field_2, &rule_field_descriptor,
                           short_context, short_context_byte_len));
  assert(!MO_match_mapping(value_3_rule_field_2, &rule_field_descriptor,
                           short_context, short_context_byte_len));
}

/* ********************************************************************** */

int main(void) {
  test_MO_equal();
  test_MO_ignore();

  init_memory_pool();
  test_MO_most_significant_bits();  // Needs the pool to be allocated
  destroy_memory_pool();

  test_MO_match_mapping();

  printf("All tests passed!\n");

  return 0;
}
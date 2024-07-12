#include "core/matching_operators.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_MO_equal(void) {
  const uint8_t simplified_context[] = {
      // TV0 ; offset = 0
      0x06,
      // TV1 ; offset = 1
      0x00,
      0x00,
      0x06,
      // TV2 ; offset = 4
      0x11,
      // TV3 ; offset = 5
      0xfe,
      0x80,
      0x00,
      0x00,
      0xa2,
  };
  const size_t ctx_len = sizeof(ctx_len);

  const uint8_t field1[]                     = {0x06};
  const uint8_t ipv6_prefix[]                = {0xfe, 0x80, 0x00, 0x00, 0xa2};
  const uint8_t ipv6_prefix_last_byte_diff[] = {0xfe, 0x80, 0x00, 0x00, 0xa3};

  assert(MO_equal(field1, 4, ctx_len, simplified_context, ctx_len) == -1);

  assert(MO_equal(field1, 4, 0, simplified_context, ctx_len));

  assert(MO_equal(field1, 8, 3, simplified_context, ctx_len));

  assert(!MO_equal(field1, 8, 4, simplified_context, ctx_len));

  assert(MO_equal(ipv6_prefix, 40, 5, simplified_context, ctx_len));

  assert(!MO_equal(ipv6_prefix_last_byte_diff, 40, 5, simplified_context,
                   ctx_len));
  assert(
      MO_equal(ipv6_prefix_last_byte_diff, 32, 5, simplified_context, ctx_len));
}

/* ********************************************************************** */

void test_MO_ignore(void) {
  // Nothing to test
  assert(MO_ignore());
}

/* ********************************************************************** */

void test_MO_most_significant_bits(void) {
  const uint8_t field1[] = {0b00110111};
  const uint8_t field2[] = {0b00000001};
  const uint8_t field3[] = {0b00011010, 0b11010011};
  const uint8_t field4[] = {0b10011010, 0b11010000};
  const uint8_t field5[] = {0b00011010, 0b01010011};

  const uint8_t simplified_context[] = {// short pattern : len=4 ; offset=0
                                        0b00001101,
                                        // pattern       : len=9 ; offset=1
                                        0b00000001, 0b10101101};

  const size_t ctx_len           = sizeof(simplified_context);
  const size_t short_pattern_len = 4;
  const size_t pattern_len       = 9;

  assert(MO_most_significant_bits(field1, 6, short_pattern_len, 0,
                                  simplified_context, ctx_len));

  assert(!MO_most_significant_bits(field1, 5, short_pattern_len, 0,
                                   simplified_context, ctx_len));

  assert(MO_most_significant_bits(field2, 1, short_pattern_len, 0,
                                  simplified_context, ctx_len) == -1);

  assert(MO_most_significant_bits(field3, 13, short_pattern_len, 0,
                                  simplified_context, ctx_len));

  assert(MO_most_significant_bits(field3, 13, pattern_len, 1,
                                  simplified_context, ctx_len));

  // there is a 1 on the 16th bit but only 13 bits are considered meaning not
  // the 3 most left bits.
  assert(MO_most_significant_bits(field4, 13, short_pattern_len, 0,
                                  simplified_context, ctx_len));

  assert(MO_most_significant_bits(field4, 13, pattern_len, 1,
                                  simplified_context, ctx_len));

  assert(MO_most_significant_bits(field5, 13, short_pattern_len, 0,
                                  simplified_context, ctx_len));

  assert(!MO_most_significant_bits(field5, 13, pattern_len, 1,
                                   simplified_context, ctx_len));
}

/* ********************************************************************** */

void test_MO_match_mapping(void) {
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

  const uint8_t field1[] = {6};
  const uint8_t field2[] = {38};
  const uint8_t field3[] = {15, 88, 15};
  const uint8_t field4[] = {7, 7, 0};
  const uint8_t field5[] = {0, 7, 7};

  assert(MO_match_mapping(field1, 8, 7, ctx_example,
                          ctx_len));  // field1 in RuleFieldDesc1
  assert(!MO_match_mapping(field1, 8, 19, ctx_example,
                           ctx_len));  // field1 not in RuleFieldDesc2

  assert(!MO_match_mapping(field2, 8, 7, ctx_example,
                           ctx_len));  // field2 in RuleFieldDesc1
  assert(!MO_match_mapping(field2, 8, 19, ctx_example,
                           ctx_len));  // field2 not in RuleFieldDesc2

  assert(MO_match_mapping(field3, 20, 29, ctx_example,
                          ctx_len));  // field3 in RuleFieldDesc3
  assert(!MO_match_mapping(field4, 20, 19, ctx_example,
                           ctx_len));  // field4 not in RuleFieldDesc3
  assert(!MO_match_mapping(field5, 20, 19, ctx_example,
                           ctx_len));  // field5 not in RuleFieldDesc3
}

/* ********************************************************************** */

int main(void) {
  test_MO_equal();
  test_MO_ignore();
  test_MO_most_significant_bits();
  test_MO_match_mapping();

  printf("All tests passed!\n");

  return 0;
}
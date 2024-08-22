#include "utils/binary.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */
/*                                Shifting                                */
/* ********************************************************************** */

void test_right_shift(void) {
  uint8_t      buffer[]        = {0b10011010, 0b01010011, 0b10110111};
  const size_t buffer_byte_len = 3;

  const uint8_t shifted_buffer_1[]  = {0b01001101, 0b00101001, 0b11011011};
  const uint8_t shifted_buffer_8[]  = {0b10011010, 0b01010011};
  const uint8_t shifted_buffer_20[] = {0b00001001};

  uint8_t ptr_buffer[buffer_byte_len];
  size_t  shifted_buffer_byte_len;

  // Right shift of 1 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = right_shift(ptr_buffer, buffer_byte_len, 1);
  assert(shifted_buffer_byte_len == 3);
  assert(memcmp(ptr_buffer, shifted_buffer_1, shifted_buffer_byte_len) == 0);

  // Right shift of 8 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = right_shift(ptr_buffer, buffer_byte_len, 8);
  assert(shifted_buffer_byte_len == 2);
  assert(memcmp(ptr_buffer, shifted_buffer_8, shifted_buffer_byte_len) == 0);

  // Right shift of 20 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = right_shift(ptr_buffer, buffer_byte_len, 20);
  assert(shifted_buffer_byte_len == 1);
  assert(memcmp(ptr_buffer, shifted_buffer_20, shifted_buffer_byte_len) == 0);

  // Right shift of 25 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = right_shift(ptr_buffer, buffer_byte_len, 25);
  assert(shifted_buffer_byte_len == 0);
}

/* ********************************************************************** */

void test_left_shift(void) {
  uint8_t      buffer[]        = {0b10011010, 0b01010011, 0b10110111};
  const size_t buffer_byte_len = 3;

  const uint8_t shifted_buffer_1[]  = {0b00110100, 0b10100111, 0b01101110};
  const uint8_t shifted_buffer_8[]  = {0b01010011, 0b10110111};
  const uint8_t shifted_buffer_20[] = {0b01110000};

  uint8_t ptr_buffer[buffer_byte_len];
  size_t  shifted_buffer_byte_len;

  // Left shift of 1 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = left_shift(ptr_buffer, buffer_byte_len, 1);
  assert(shifted_buffer_byte_len == 3);
  assert(memcmp(ptr_buffer, shifted_buffer_1, shifted_buffer_byte_len) == 0);

  // Left shift of 8 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = left_shift(ptr_buffer, buffer_byte_len, 8);
  assert(shifted_buffer_byte_len == 2);
  assert(memcmp(ptr_buffer, shifted_buffer_8, shifted_buffer_byte_len) == 0);

  // Left shift of 20 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = left_shift(ptr_buffer, buffer_byte_len, 20);
  assert(shifted_buffer_byte_len == 1);
  assert(memcmp(ptr_buffer, shifted_buffer_20, shifted_buffer_byte_len) == 0);

  // Left shift of 25 bit
  memcpy(ptr_buffer, buffer, buffer_byte_len);
  shifted_buffer_byte_len = left_shift(ptr_buffer, buffer_byte_len, 25);
  assert(shifted_buffer_byte_len == 0);
}

/* ********************************************************************** */
/*                                 Adding                                 */
/* ********************************************************************** */

void test_add_byte_to_buffer(void) {
  const uint8_t expected_buffer1[] = {0b01100000};
  const uint8_t expected_buffer2[] = {0b01101101, 0b10000000};
  const uint8_t expected_buffer3[] = {0b01101101, 0b11111111};

  size_t       bit_pos         = 0;
  uint8_t      buffer[2]       = {0};
  const size_t buffer_byte_len = 2;

  // Try to add more than 8 bits
  assert(!add_byte_to_buffer(buffer, buffer_byte_len, &bit_pos, 0x00, 9));

  // Add 0110 to buffer (empty)
  assert(add_byte_to_buffer(buffer, buffer_byte_len, &bit_pos, 0b0110, 4));
  assert(bit_pos == 4);
  assert(memcmp(expected_buffer1, buffer, 1) == 0);

  // Add 11011 to buffer (0110)
  assert(add_byte_to_buffer(buffer, buffer_byte_len, &bit_pos, 0b11011, 5));
  assert(bit_pos == 9);
  assert(memcmp(expected_buffer2, buffer, 2) == 0);

  // Add 11111111 to buffer (011011011)
  // But the maximum size if buffer will be reached (17 bits instead of 16
  // available)
  assert(!add_byte_to_buffer(buffer, buffer_byte_len, &bit_pos, 0b11111111, 8));
  assert(bit_pos == 9);

  // Add 1111111 to buffer (011011011)
  assert(add_byte_to_buffer(buffer, buffer_byte_len, &bit_pos, 0b1111111, 7));
  assert(bit_pos == 16);
  assert(memcmp(expected_buffer3, buffer, 2) == 0);
}

/* ********************************************************************** */

void test_add_bits_to_buffer(void) {
  const uint8_t content1[]         = {0b0110};
  const uint8_t expected_buffer1[] = {0b01100000};

  const uint8_t content2[]         = {0b11011};
  const uint8_t expected_buffer2[] = {0b01101101, 0b10000000};

  const uint8_t content3[]         = {0b1000, 0b10101001, 0b11000011};
  const uint8_t expected_buffer3[] = {0b01101101, 0b11000101, 0b01001110,
                                      0b00011000};

  const uint8_t content4[]         = {0b11111111};
  const uint8_t expected_buffer4[] = {0b01101101, 0b11000101, 0b01001110,
                                      0b00011111, 0b11111000};

  size_t       bit_pos         = 0;
  uint8_t      buffer[5]       = {0};
  const size_t buffer_byte_len = 5;

  // Add content1 (0110) to buffer (empty)
  assert(add_bits_to_buffer(buffer, buffer_byte_len, &bit_pos, content1, 4));
  assert(bit_pos == 4);
  assert(memcmp(expected_buffer1, buffer, 1) == 0);

  // Add content2 (11011) to buffer (0110)
  assert(add_bits_to_buffer(buffer, buffer_byte_len, &bit_pos, content2, 5));
  assert(bit_pos == 9);
  assert(memcmp(expected_buffer2, buffer, 2) == 0);

  // Add content3 (10001010100111000011) to buffer (011011011)
  assert(add_bits_to_buffer(buffer, buffer_byte_len, &bit_pos, content3, 20));
  assert(bit_pos == 29);
  assert(memcmp(expected_buffer3, buffer, 4) == 0);

  // Add content4 (11111111) to buffer (01101101110001010100111000011)
  assert(add_bits_to_buffer(buffer, buffer_byte_len, &bit_pos, content4, 8));
  assert(bit_pos == 37);
  assert(memcmp(expected_buffer4, buffer, 5) == 0);

  // Add content1 again (0110) to buffer (0110110111000101010011100001111111111)
  // But the maximum size if buffer will be reached (41 bits instead of 40
  // available)
  assert(!add_bits_to_buffer(buffer, buffer_byte_len, &bit_pos, content1, 4));
}

/* ********************************************************************** */
/*                               Extraction                               */
/* ********************************************************************** */

void test_extract_bits(void) {
  uint8_t extracted_content[5];
  size_t  bit_position = 0;

  const uint8_t buffer1[] = {0b01101101, 0b11000101, 0b01001110, 0b00011111,
                             0b11111000};
  const size_t  buffer1_byte_len = sizeof(buffer1);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 4 bits from bit_pos = 0 :
   * - value = {0b0110}
   * - bit_position = 4
   * - return 1
   */
  const uint8_t expected_buffer1_1[] = {0b0110};
  assert(extract_bits(extracted_content, 5, 4, &bit_position, buffer1,
                      buffer1_byte_len));
  assert(bit_position == 4);
  assert(memcmp(extracted_content, expected_buffer1_1, 1) == 0);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 8 bits from bit_pos = 4 :
   * - value = {0b11011100}
   * - bit_position = 4 + 8 = 12
   * - return 1
   */
  const uint8_t expected_buffer1_2[] = {0b11011100};
  assert(extract_bits(extracted_content, 5, 8, &bit_position, buffer1,
                      buffer1_byte_len));
  assert(bit_position == 12);
  assert(memcmp(extracted_content, expected_buffer1_2, 1) == 0);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 2 bits from bit_pos = 12 :
   * - value = {0b01}
   * - bit_position = 12 + 2 = 14
   * - return 1
   */
  const uint8_t expected_buffer1_3[] = {0b01};
  assert(extract_bits(extracted_content, 5, 2, &bit_position, buffer1,
                      buffer1_byte_len));
  assert(bit_position == 14);
  assert(memcmp(extracted_content, expected_buffer1_3, 1) == 0);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 18 bits from bit_pos = 14 :
   * - value = {0b01, 0b01001110, 0b00011111}
   * - bit_position = 14 + 18 = 32
   * - return 1
   */
  const uint8_t expected_buffer1_4[] = {0b01, 0b01001110, 0b00011111};
  assert(extract_bits(extracted_content, 5, 18, &bit_position, buffer1,
                      buffer1_byte_len));
  assert(bit_position == 32);
  assert(memcmp(extracted_content, expected_buffer1_4, 1) == 0);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 9 bits from bit_pos = 32 :
   * - value = null
   * - bit_position = 32
   * - return 0 (Only 8 bits remain from the buffer)
   */
  assert(!extract_bits(extracted_content, 5, 32, &bit_position, buffer1,
                       buffer1_byte_len));
  assert(bit_position == 32);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 8 bits from bit_pos = 32 :
   * - value = {0b11111000}
   * - bit_position = 14 + 18 = 32
   * - return 1
   */
  const uint8_t expected_buffer1_5[] = {0b11111000};
  assert(extract_bits(extracted_content, 5, 8, &bit_position, buffer1,
                      buffer1_byte_len));
  assert(bit_position == 40);
  assert(memcmp(extracted_content, expected_buffer1_5, 1) == 0);

  /**
   * @brief Extraction on buffer1 = 0110110111000101010011100001111111111000.
   *
   * @details Extract 1 bit from bit_pos = 40 :
   * - value = null
   * - bit_position = 40
   * - return 0 (The end of buffer has been reached)
   */
  assert(!extract_bits(extracted_content, 5, 1, &bit_position, buffer1,
                       buffer1_byte_len));
  assert(bit_position == 40);
}

/* ********************************************************************** */
/*                                Other(s)                                */
/* ********************************************************************** */

void test_bits_counter(void) {
  assert(bits_counter(0x00) == 1);
  assert(bits_counter(0x01) == 1);
  assert(bits_counter(0x02) == 2);
  assert(bits_counter(0x03) == 2);
  assert(bits_counter(0x07) == 3);
  assert(bits_counter(0xff) == 8);
}

/* ********************************************************************** */

void test_split_uint16_t(void) {
  uint8_t left, right;

  /**
   * @brief Split on value1 = 0x0000
   *
   * @details left = 0x00 and right = 0x00
   */
  const uint16_t value1 = 0x0000;
  split_uint16_t(&left, &right, value1);
  assert(left == 0x00 && right == 0x00);

  /**
   * @brief Split on value2 = 0x0001
   *
   * @details left = 0x00 and right = 0x01
   */
  const uint16_t value2 = 0x0001;
  split_uint16_t(&left, &right, value2);
  assert(left == 0x00 && right == 0x01);

  /**
   * @brief Split on value3 = 0x1000
   *
   * @details left = 0x10 and right = 0x00
   */
  const uint16_t value3 = 0x1000;
  split_uint16_t(&left, &right, value3);
  assert(left == 0x10 && right == 0x00);

  /**
   * @brief Split on value4 = 0xabcd
   *
   * @details left = 0xab and right = 0xcd
   */
  const uint16_t value4 = 0xabcd;
  split_uint16_t(&left, &right, value4);
  assert(left == 0xab && right == 0xcd);
}

/* ********************************************************************** */

void test_merge_uint8_t(void) {
  uint16_t value;

  /**
   * @brief Merge on left1 = 0x00 and right1 = 0x00
   *
   * @details value = 0x0000
   */
  const uint8_t left1 = 0x00, right1 = 0x00;
  value = merge_uint8_t(left1, right1);
  assert(value == 0x0000);

  /**
   * @brief Merge on left2 = 0x00 and right2 = 0x01
   *
   * @details value = 0x0001
   */
  const uint8_t left2 = 0x00, right2 = 0x01;
  value = merge_uint8_t(left2, right2);
  assert(value == 0x0001);

  /**
   * @brief Merge on left3 = 0x10 and right3 = 0x00
   *
   * @details value = 0x1000
   */
  const uint8_t left3 = 0x10, right3 = 0x00;
  value = merge_uint8_t(left3, right3);
  assert(value == 0x1000);

  /**
   * @brief Merge on left4 = 0xab and right4 = 0xcd
   *
   * @details value = 0xabcd
   */
  const uint8_t left4 = 0xab, right4 = 0xcd;
  value = merge_uint8_t(left4, right4);
  assert(value == 0xabcd);
}

/* ********************************************************************** */

int main(void) {
  test_right_shift();
  test_left_shift();
  test_add_byte_to_buffer();
  test_add_bits_to_buffer();
  test_extract_bits();
  test_bits_counter();
  test_split_uint16_t();
  test_merge_uint8_t();

  printf("All tests passed!\n");

  return 0;
}
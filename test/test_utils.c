#include "utils/binary.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_right_shift(void) {
  uint8_t buffer[]        = {0b10011010, 0b01010011, 0b10110111};
  size_t  buffer_byte_len = 3;

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
  assert(shifted_buffer_byte_len == -1);
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

  int          bit_pos = 0;
  uint8_t      buffer[5];
  const size_t buffer_byte_len = 5;

  // Add content1 (0110) to buffer (empty)
  assert(add_bits_to_buffer(buffer, content1, 4, &bit_pos, buffer_byte_len));
  assert(bit_pos == 4);
  assert(memcmp(expected_buffer1, buffer, 1) == 0);

  // Add content2 (11011) to buffer (0110)
  assert(add_bits_to_buffer(buffer, content2, 5, &bit_pos, buffer_byte_len));
  assert(bit_pos == 9);
  assert(memcmp(expected_buffer2, buffer, 2) == 0);

  // Add content3 (10001010100111000011) to buffer (011011011)
  assert(add_bits_to_buffer(buffer, content3, 20, &bit_pos, buffer_byte_len));
  assert(bit_pos == 29);
  assert(memcmp(expected_buffer3, buffer, 4) == 0);

  // Add content4 (11111111) to buffer (01101101110001010100111000011)
  assert(add_bits_to_buffer(buffer, content4, 8, &bit_pos, buffer_byte_len));
  assert(bit_pos == 37);
  assert(memcmp(expected_buffer4, buffer, 5) == 0);

  // Add content1 again (0110) to buffer (0110110111000101010011100001111111111)
  // But the maximum size if buffer will be reached (41 bits instead of 40
  // available)
  assert(!add_bits_to_buffer(buffer, content1, 4, &bit_pos, buffer_byte_len));
}

int main(void) {
  test_right_shift();
  test_add_bits_to_buffer();

  printf("All tests passed!\n");

  return 0;
}
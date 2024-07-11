#include "utils/binarie.h"

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

int main(void) {
  test_right_shift();

  printf("All tests passed!\n");

  return 0;
}
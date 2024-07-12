#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */

size_t right_shift(uint8_t* buffer, size_t buffer_byte_len, int shift_amount) {
  if (shift_amount <= 0 || buffer_byte_len == 0 ||
      shift_amount > buffer_byte_len * 8) {
    return -1;
  }

  int    byte_to_remove;
  int    bit_shift;
  size_t shifted_buffer_byte_len;

  byte_to_remove          = shift_amount / 8;
  bit_shift               = shift_amount % 8;
  shifted_buffer_byte_len = buffer_byte_len - byte_to_remove;

  if (byte_to_remove > 0) {
    for (size_t i = buffer_byte_len - 1; i >= byte_to_remove; i--) {
      buffer[i] = buffer[i - byte_to_remove];
    }
    for (size_t i = 0; i < byte_to_remove; i++) {
      buffer[i] = 0;
    }
  }

  if (bit_shift > 0) {
    for (size_t i = buffer_byte_len - 1; i > 0; i--) {
      buffer[i] = (buffer[i] >> bit_shift) | (buffer[i - 1] << (8 - bit_shift));
    }
    buffer[0] >>= bit_shift;
  }

  memcpy(buffer, buffer + byte_to_remove, shifted_buffer_byte_len);

  return shifted_buffer_byte_len;
}
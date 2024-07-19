#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */

static int __add_byte_to_buffer(uint8_t* buffer, const uint8_t content,
                                const size_t content_len, int* bit_pos,
                                size_t buffer_byte_len);

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

/* ********************************************************************** */

int add_bits_to_buffer(uint8_t* buffer, const uint8_t* content,
                       const size_t content_len, int* bit_pos,
                       size_t buffer_byte_len) {
  if (BYTE_LENGTH(*bit_pos + content_len) > buffer_byte_len) {
    return 0;
  }

  int content_ind;
  int len_remainder;
  int content_byte_len;

  content_ind      = 0;
  len_remainder    = content_len % 8;
  content_byte_len = content_len / 8;

  if (len_remainder > 0) {
    __add_byte_to_buffer(buffer, content[content_ind++], len_remainder, bit_pos,
                         buffer_byte_len);
  }

  while (content_ind <= content_byte_len) {
    __add_byte_to_buffer(buffer, content[content_ind++], 8, bit_pos,
                         buffer_byte_len);
  }

  return 1;
}

/* ********************************************************************** */

static int __add_byte_to_buffer(uint8_t* buffer, const uint8_t content,
                                const size_t content_len, int* bit_pos,
                                size_t buffer_byte_len) {
  if (BYTE_LENGTH(*bit_pos + content_len) > buffer_byte_len) {
    return 0;
  }

  int byte_pos;
  int bit_offset;
  int remaining_bits;

  byte_pos   = *bit_pos / 8;
  bit_offset = *bit_pos % 8;

  if (bit_offset + content_len <= 8) {
    buffer[byte_pos] |= (content << (8 - bit_offset - content_len));
  } else {
    // fill partially the current byte with (8 - bit_offset) bits
    buffer[byte_pos] |= (content >> (content_len - (8 - bit_offset)));

    // fill the remainin part of content in the following byte
    remaining_bits = content_len - (8 - bit_offset);
    buffer[++byte_pos] |= (content << (8 - remaining_bits));
  }

  *bit_pos += content_len;

  return 1;
}
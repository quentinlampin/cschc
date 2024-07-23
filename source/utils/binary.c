#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */

size_t right_shift(uint8_t* buffer, const size_t buffer_byte_len,
                   int shift_amount) {
  if (shift_amount <= 0 || buffer_byte_len == 0 ||
      shift_amount > buffer_byte_len * 8) {
    return 0;
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

int add_byte_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       int* bit_position, const uint8_t content,
                       const size_t content_len) {
  int status;
  int byte_pos;
  int bit_offset;
  int remaining_bits;

  status = (content_len > 8 ||
            BYTE_LENGTH(*bit_position + content_len) > buffer_byte_len)
               ? 0
               : 1;

  if (!status) {
    return status;
  }

  byte_pos   = *bit_position / 8;
  bit_offset = *bit_position % 8;

  if (bit_offset + content_len <= 8) {
    buffer[byte_pos] |= (content << (8 - bit_offset - content_len));
  } else {
    // fill partially the current byte with (8 - bit_offset) bits
    buffer[byte_pos] |= (content >> (content_len - (8 - bit_offset)));

    // fill the remainin part of content in the following byte
    remaining_bits = content_len - (8 - bit_offset);
    buffer[++byte_pos] |= (content << (8 - remaining_bits));
  }

  *bit_position += content_len;

  return status;
}

/* ********************************************************************** */

int add_bits_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       int* bit_position, const uint8_t* content,
                       const size_t content_len) {
  int status;
  int content_ind;
  int len_remainder;
  int content_byte_len;

  status = (BYTE_LENGTH(*bit_position + content_len) > buffer_byte_len) ? 0 : 1;

  if (!status) {
    return status;
  }

  content_ind      = 0;
  len_remainder    = content_len % 8;
  content_byte_len = content_len / 8;

  if (len_remainder > 0) {
    status = add_byte_to_buffer(buffer, buffer_byte_len, bit_position,
                                content[content_ind++], len_remainder);
  }

  while (status && ((len_remainder > 0 && content_ind <= content_byte_len) ||
                    (content_ind < content_byte_len))) {
    status = add_byte_to_buffer(buffer, buffer_byte_len, bit_position,
                                content[content_ind++], 8);
  }

  return status;
}

/* ********************************************************************** */

int bits_counter(uint8_t value) {
  if (value == 0x00) {
    return 1;
  }
  int count;

  count = 0;
  while (value) {
    count++;
    value >>= 1;
  }

  return count;
}
#include "utils/binary.h"

#include <string.h>

/* ********************************************************************** */
/*                                Shifting                                */
/* ********************************************************************** */

size_t right_shift(uint8_t* buffer, const size_t buffer_byte_len,
                   const size_t shift_amount) {
  size_t byte_to_remove;
  size_t bit_shift;
  size_t shifted_buffer_byte_len;

  if (buffer == NULL || shift_amount == 0 || buffer_byte_len == 0 ||
      shift_amount > buffer_byte_len * 8) {
    return 0;
  }

  byte_to_remove          = shift_amount / 8;
  bit_shift               = shift_amount % 8;
  shifted_buffer_byte_len = buffer_byte_len - byte_to_remove;

  // Shift the byte(s)
  if (byte_to_remove > 0) {
    for (size_t i = buffer_byte_len - 1; i >= byte_to_remove; i--) {
      buffer[i] = buffer[i - byte_to_remove];
    }
    for (size_t i = 0; i < byte_to_remove; i++) {
      buffer[i] = 0;
    }
  }

  // Shift the bit(s)
  if (bit_shift > 0) {
    for (size_t i = buffer_byte_len - 1; i > 0; i--) {
      buffer[i] = (buffer[i] >> bit_shift) | (buffer[i - 1] << (8 - bit_shift));
    }
    buffer[0] >>= bit_shift;
  }

  memmove(buffer, buffer + byte_to_remove, shifted_buffer_byte_len);

  return shifted_buffer_byte_len;
}

/* ********************************************************************** */

size_t left_shift(uint8_t* buffer, const size_t buffer_byte_len,
                  const size_t shift_amount) {
  size_t byte_to_remove;
  size_t bit_shift;
  size_t shifted_buffer_byte_len;

  if (buffer == NULL || shift_amount == 0 || buffer_byte_len == 0 ||
      shift_amount > buffer_byte_len * 8) {
    return 0;
  }

  byte_to_remove          = shift_amount / 8;
  bit_shift               = shift_amount % 8;
  shifted_buffer_byte_len = buffer_byte_len - byte_to_remove;

  // Shift the byte(s)
  if (byte_to_remove > 0) {
    for (size_t i = 0; i < buffer_byte_len - byte_to_remove; i++) {
      buffer[i] = buffer[i + byte_to_remove];
    }
    for (size_t i = buffer_byte_len - byte_to_remove; i < buffer_byte_len;
         i++) {
      buffer[i] = 0;
    }
  }

  // Shift the bit(s)
  if (bit_shift > 0) {
    for (size_t i = 0; i < shifted_buffer_byte_len - 1; i++) {
      buffer[i] = (buffer[i] << bit_shift) | (buffer[i + 1] >> (8 - bit_shift));
    }
    buffer[shifted_buffer_byte_len - 1] <<= bit_shift;
  }

  return shifted_buffer_byte_len;
}

/* ********************************************************************** */
/*                                 Adding                                 */
/* ********************************************************************** */

int add_byte_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       size_t* bit_position, const uint8_t content,
                       const size_t content_len) {
  size_t byte_pos;
  size_t bit_offset;
  size_t remaining_bits;

  if (content_len > 8 ||
      BYTE_LENGTH(*bit_position + content_len) > buffer_byte_len) {
    return 0;
  }

  byte_pos   = *bit_position / 8;
  bit_offset = *bit_position % 8;

  // Fill the buffer with the content
  if (bit_offset + content_len <= 8) {
    // Fill the entire content at byte_pos
    buffer[byte_pos] |= (content << (8 - bit_offset - content_len));
  } else {
    // Fill the current byte partially with (8 - bit_offset) bits
    buffer[byte_pos] |= (content >> (content_len - (8 - bit_offset)));

    // Fill the remaining part of the content in the following byte
    remaining_bits = content_len - (8 - bit_offset);
    buffer[++byte_pos] |= (content << (8 - remaining_bits));
  }

  // Update the bit_position
  *bit_position += content_len;

  return 1;
}

/* ********************************************************************** */

int add_bits_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       size_t* bit_position, const uint8_t* content,
                       const size_t content_len) {
  int    status;
  size_t content_ind;
  size_t len_remainder;
  size_t content_byte_len;

  status = (BYTE_LENGTH(*bit_position + content_len) > buffer_byte_len) ? 0 : 1;

  if (!status) {
    return status;
  }

  content_ind      = 0;
  len_remainder    = content_len % 8;
  content_byte_len = content_len / 8;

  // The buffer is left-padded; therefore, if the content length is not a
  // multiple of 8, a residue of the content appears in the first byte
  if (len_remainder > 0) {
    status = add_byte_to_buffer(buffer, buffer_byte_len, bit_position,
                                content[content_ind++], len_remainder);
  }

  // Add the content to the buffer byte by byte
  while (status && ((len_remainder > 0 && content_ind <= content_byte_len) ||
                    (content_ind < content_byte_len))) {
    status = add_byte_to_buffer(buffer, buffer_byte_len, bit_position,
                                content[content_ind++], 8);
  }

  return status;
}

/* ********************************************************************** */
/*                               Extraction                               */
/* ********************************************************************** */

int extract_bits(uint8_t* content, const size_t content_max_byte_len,
                 const size_t content_len, size_t* bit_position,
                 const uint8_t* buffer, const size_t buffer_byte_len) {
  int    status;
  size_t byte_pos;
  size_t bit_offset;
  size_t extra_bits;
  size_t end_byte;
  size_t content_byte_len;

  byte_pos         = *bit_position / 8;
  bit_offset       = *bit_position % 8;
  end_byte         = (*bit_position + content_len - 1) / 8;
  content_byte_len = BYTE_LENGTH(content_len);

  status =
      (content_byte_len > content_max_byte_len || end_byte >= buffer_byte_len)
          ? 0
          : 1;

  if (!status) {
    return status;
  }

  // Reset content
  memset(content, 0x00, content_max_byte_len);

  // Copy the interesting part into content
  // Note: If the bit_offset is a multiple of 8, then the content is already
  // correct
  memcpy(content, buffer + byte_pos, end_byte - byte_pos + 1);

  // Remove unwanted data, meaning the content needs to be shifted
  if (bit_offset != 0) {
    for (size_t i = 0; i < content_byte_len; i++) {
      content[i] =
          (content[i] << bit_offset) | (content[i + 1] >> (8 - bit_offset));
    }
  }

  // Consider the last part of content which can contain unwanted bit(s)
  extra_bits = (content_byte_len * 8) - content_len;
  if (extra_bits > 0) {
    content[content_byte_len - 1] &= (0xff << extra_bits);
  }

  // Shift the content on the right
  right_shift(content, content_byte_len, -content_len % 8);

  // Update the bit_position
  *bit_position += content_len;

  return 1;
}

/* ********************************************************************** */
/*                                Other(s)                                */
/* ********************************************************************** */

size_t bits_counter(uint8_t value) {
  size_t count;

  if (value == 0x00) {
    return 1;
  }

  count = 0;
  while (value) {
    count++;
    value >>= 1;
  }

  return count;
}

/* ********************************************************************** */

void split_uint16_t(uint8_t* left_byte, uint8_t* right_byte,
                    const uint16_t value) {
  *left_byte  = (uint8_t) (value >> 8);
  *right_byte = (uint8_t) (value & 0xff);
}

/* ********************************************************************** */

uint16_t merge_uint8_t(const uint8_t left_byte, const uint8_t right_byte) {
  return ((uint16_t) (left_byte << 8)) | right_byte;
}
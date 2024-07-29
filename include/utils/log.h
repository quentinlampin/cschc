#ifndef _LOG_H_
#define _LOG_H_

#include <stddef.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                    \
  ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),     \
      ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'), \
      ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'), \
      ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

/**
 * @brief Displays a uint8_t array as binary.
 *
 * @param buffer Pointer to the buffer.
 * @param buffer_byte_len Byte length of the buffer.
 */
void print_binary(const uint8_t *buffer, const size_t buffer_byte_len);

#endif  // _LOG_H_
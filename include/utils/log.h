/**
 * @file log.h
 * @author Corentin Banier
 * @brief Logging implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief These macros originate from
 * https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format.
 */
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

/**
 * @brief Displays a uint8_t array as hexadecimal array.
 *
 * @param buffer Pointer to the buffer.
 * @param buffer_byte_len Byte length of the buffer.
 * @param with_space Indicates the display format. 1 for adding space and comma
 * between elements, 0 otherwise.
 */
void print_hex_packet(const uint8_t *buffer, const size_t buffer_byte_len,
                      int with_space);

#endif  // _LOG_H_
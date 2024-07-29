#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Computes the byte length associated with a bit length.
 */
#define BYTE_LENGTH(x) (((x) % 8 == 0) ? (x) / 8 : 1 + (x) / 8)

/**
 * @brief Performs a right shift of shift_amount on a buffer.
 *
 * @param buffer Pointer to the buffer to shift.
 * @param buffer_byte_len Byte length of the buffer.
 * @param shift_amount Amount of bit shift to perform.
 * @return The right-shifted buffer byte length.
 */
size_t right_shift(uint8_t* buffer, const size_t buffer_byte_len,
                   int shift_amount);

/**
 * @brief Adds a uint8_t into a buffer.
 *
 * @param buffer Pointer to the buffer.
 * @param buffer_byte_len Byte length of the buffer.
 * @param bit_position Pointer to the current bit position in the buffer.
 * @param content Data to add.
 * @param content_len Data bit length.
 * @return The status code, 1 for success otherwise 0.
 *
 * @remark As content is a uint8_t, it means that the content_len parameter
 * value is between 0 and 8.
 */
int add_byte_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       int* bit_position, const uint8_t content,
                       const size_t content_len);

/**
 * @brief Adds bits into a buffer.
 *
 * @param buffer Pointer to the buffer.
 * @param buffer_byte_len Byte length of the buffer.
 * @param bit_position Pointer to the current bit position in the buffer.
 * @param content Pointer to the data to add.
 * @param content_len Data bit length.
 * @return The status code, 1 for success otherwise 0.
 *
 * @details See int add_byte_to_buffer.
 */
int add_bits_to_buffer(uint8_t* buffer, const size_t buffer_byte_len,
                       int* bit_position, const uint8_t* content,
                       const size_t content_len);

/**
 * @brief Counts the number of bits needed to represent a value.
 *
 * @param value The uint8_t value.
 * @return The number of bits.
 */
int bits_counter(uint8_t value);

#endif  // _BINARY_H_
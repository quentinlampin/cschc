#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Macro that computes the byte length associated with a bit length.
 */
#define BYTE_LENGTH(x) (((x) % 8 == 0) ? (x) / 8 : 1 + (x) / 8)

/* ********************************************************************** */
/*                                Shifting                                */
/* ********************************************************************** */

/**
 * @brief Performs a right shift of shift_amount on a buffer.
 *
 * @param buffer Pointer to the buffer to shift.
 * @param buffer_byte_len Byte length of the buffer.
 * @param shift_amount Amount of bit shift to perform.
 * @return The right-shifted buffer byte length.
 */
size_t right_shift(uint8_t* buffer, size_t buffer_byte_len,
                   size_t shift_amount);

/**
 * @brief Performs a left shift of shift_amount on a buffer.
 *
 * @param buffer Pointer to the buffer to shift.
 * @param buffer_byte_len Byte length of the buffer.
 * @param shift_amount Amount of bit shift to perform.
 * @return The left-shifted buffer byte length.
 */
size_t left_shift(uint8_t* buffer, size_t buffer_byte_len, size_t shift_amount);

/* ********************************************************************** */
/*                                 Adding                                 */
/* ********************************************************************** */

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
int add_byte_to_buffer(uint8_t* buffer, size_t buffer_byte_len,
                       size_t* bit_position, uint8_t content,
                       size_t content_len);

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
int add_bits_to_buffer(uint8_t* buffer, size_t buffer_byte_len,
                       size_t* bit_position, const uint8_t* content,
                       size_t content_len);

/* ********************************************************************** */
/*                               Extraction                               */
/* ********************************************************************** */

/**
 * @brief Extracts the content of a buffer from a bit position and a bit length.
 *
 * @param content Pointer to the extracted content.
 * @param content_max_byte_len Maximum byte length of content.
 * @param content_len The bit length to extract.
 * @param bit_position Pointer to the bit position in the buffer.
 * @param buffer Pointer to the buffer.
 * @param buffer_byte_len Byte length of the buffer.
 * @return The status code, 1 for success otherwise 0.
 *
 * @details The bit_position is updated thanks to content_len.
 */
int extract_bits(uint8_t* content, size_t content_max_byte_len,
                 size_t content_len, size_t* bit_position,
                 const uint8_t* buffer, size_t buffer_byte_len);

/* ********************************************************************** */
/*                                Other(s)                                */
/* ********************************************************************** */

/**
 * @brief Counts the number of bits needed to represent a value.
 *
 * @param value The uint8_t value.
 * @return The corresponding number of bits.
 */
size_t bits_counter(uint8_t value);

/**
 * @brief Splits a uint16_t value into two uint8_t values.
 *
 * @param left_byte Pointer to store the left byte.
 * @param right_byte Pointer to store the right byte.
 * @param value The uint16_t value to split.
 */
void split_uint16_t(uint8_t* left_byte, uint8_t* right_byte,
                    const uint16_t value);

/**
 * @brief Merges two uint8_t values into a uint16_t value.
 *
 * @param left_byte The left byte.
 * @param right_byte The right byte.
 * @return The merged uint16_t value.
 */
uint16_t merge_uint8_t(const uint8_t left_byte, const uint8_t right_byte);

#endif  // _BINARY_H_
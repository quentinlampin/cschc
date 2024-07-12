#ifndef _BINARY_H_
#define _BINARY_H_

#include <stddef.h>
#include <stdint.h>

#define BYTE_LENGTH(x) (((x) % 8 == 0) ? (x) / 8 : 1 + (x) / 8)

size_t right_shift(uint8_t* buffer, size_t buffer_byte_len, int shift_amount);

#endif  // _BINARY_H_
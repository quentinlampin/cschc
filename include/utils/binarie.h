#ifndef _BINARIE_H_
#define _BINARIE_H_

#include <stddef.h>
#include <stdint.h>

#define BYTE_LENGTH(x) (((x) % 8 == 0) ? (x) / 8 : 1 + (x) / 8)

size_t right_shift(uint8_t* buffer, size_t buffer_byte_len, int shift_amount);

#endif  // _BINARIE_H_
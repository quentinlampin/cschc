#include "utils/log.h"

#include <stdio.h>

/* ********************************************************************** */

void print_binary(const uint8_t *buffer, const size_t buffer_byte_len) {
  for (size_t i = 0; i < buffer_byte_len; i++) {
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(buffer[i]));
    printf(" ");
  }
  printf("\n");
}
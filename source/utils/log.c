#include "utils/log.h"

#include <stdio.h>

/* ********************************************************************** */

void print_binary(const uint8_t *data, const size_t size) {
  for (size_t i = 0; i < size; i++) {
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(data[i]));
    printf(" ");
  }
  printf("\n");
}
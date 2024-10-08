#include "protocols/headers.h"
#include "utils/memory.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */
/*                          UDP Header functions                          */
/* ********************************************************************** */

void test_udp_checksum(void) {
  init_memory_pool();

  uint8_t*     checksum          = (uint8_t*) pool_alloc(sizeof(uint8_t) * 2);
  const size_t checksum_byte_len = sizeof(uint8_t) * 2;

  // UDP Checksum is set to 0x0000 in all initial packets

  /**
   * @brief Computes UDP Checksum on packet1 (IPv6).
   *
   * @details UDP Checksum = {0x21, 0x4e}
   */
  const uint8_t packet1[] = {
      0x60, 0x0f, 0xdb, 0xce, 0x00, 0x26, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x03, 0x16, 0x33, 0x90, 0xa0, 0x00, 0x26, 0x00, 0x00,
      0x42, 0x03, 0x2f, 0x17, 0x9c, 0x73, 0xb4, 0x33, 0x33, 0x30, 0x38, 0x01,
      0x30, 0x04, 0x35, 0x39, 0x30, 0x30, 0x11, 0x3c, 0xff, 0xfb, 0x40, 0x34,
      0xf5, 0xad, 0x8c, 0x25, 0x46, 0x37};
  const size_t  packet1_byte_len     = sizeof(packet1);
  const uint8_t expected_checksum1[] = {0x21, 0x4e};

  udp_checksum(checksum, checksum_byte_len, packet1, packet1_byte_len, 1);
  assert(memcmp(checksum, expected_checksum1, checksum_byte_len) == 0);

  /**
   * @brief Computes UDP Checksum on packet2 (IPv6).
   *
   * @details UDP Checksum = {0x50, 0x3b}
   */
  const uint8_t packet2[] = {
      0x60, 0x0f, 0xf8, 0x5f, 0x00, 0x16, 0x11, 0x40, 0x20, 0x01, 0x0d,
      0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x03, 0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x90, 0xa0, 0x16, 0x33,
      0x00, 0x16, 0x00, 0x00, 0x52, 0x45, 0x14, 0xb2, 0x37, 0x09, 0x61,
      0x73, 0x61, 0x3c, 0xff, 0xf9, 0x4c, 0x60};
  const size_t  packet2_byte_len     = sizeof(packet2);
  const uint8_t expected_checksum2[] = {0x50, 0x3b};

  udp_checksum(checksum, checksum_byte_len, packet2, packet2_byte_len, 1);
  assert(memcmp(checksum, expected_checksum2, checksum_byte_len) == 0);

  /**
   * @brief Computes UDP Checksum on packet3 (IPv6).
   *
   * @details UDP Checksum = {0xd7, 0x25}
   */
  const uint8_t packet3[] = {
      0x60, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x11, 0x40, 0xaa, 0xaa, 0xaa,
      0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
      0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb,
      0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0x12, 0x34, 0x56, 0x78,
      0x00, 0x3b, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
      0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
      0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
      0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
      0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32};
  const size_t  packet3_byte_len     = sizeof(packet3);
  const uint8_t expected_checksum3[] = {0xd7, 0x25};

  udp_checksum(checksum, checksum_byte_len, packet3, packet3_byte_len, 1);
  assert(memcmp(checksum, expected_checksum3, checksum_byte_len) == 0);

  destroy_memory_pool();
}

/* ********************************************************************** */
/*                         CoAP Header functions                          */
/* ********************************************************************** */

void test_set_coap_option_variable_length(void) {
  // To do
}

/* ********************************************************************** */

void test_get_coap_option_bit_length(void) {
  // To do
}

/* ********************************************************************** */

int main(void) {
  test_udp_checksum();
  test_set_coap_option_variable_length();
  test_get_coap_option_bit_length();

  printf("All tests passed!\n");

  return 0;
}
#include "parser/protocol/udp.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_udp_parser(void) {
  /**
   * @brief Testing the UDP parser on a UDP header.
   *
   * The packet is made of a UDP header with following fields:
   *  - id='App Port'     val={0x23, 0x29}
   *  - id='Dev Port'     val={0x23, 0x2a}
   *  - id='Length'       val={0x00, 0x10}
   *  - id='Checksum'     val={0x2d, 0xa1}
   *
   */
  const uint8_t udp_header[] = {
      0x23, 0x29, 0x23, 0x2a, 0x00, 0x10, 0x2d, 0xa1,
  };

  // UDP expected fields
  const uint8_t udp_expected_app_port[] = {0x23, 0x29};
  const uint8_t udp_expected_dev_port[] = {0x23, 0x2a};
  const uint8_t udp_expected_length[]   = {0x00, 0x10};
  const uint8_t udp_expected_checksum[] = {0x2d, 0xa1};

  int       next_offset;
  udp_hdr_t udp_parsed_header;

  next_offset = parse_udp_header(&udp_parsed_header, udp_header, 0, 8);

  assert(next_offset == 8);

  // UDP App Port
  assert(memcmp(udp_parsed_header.app_port, udp_expected_app_port,
                UDP_APP_PORT_BYTE_LENGTH) == 0);

  // UDP Dev Port
  assert(memcmp(udp_parsed_header.dev_port, udp_expected_dev_port,
                UDP_DEV_PORT_BYTE_LENGTH) == 0);

  // UDP Length
  assert(memcmp(udp_parsed_header.length, udp_expected_length,
                UDP_LENGTH_BYTE_LENGTH) == 0);

  // UDP Checksum
  assert(memcmp(udp_parsed_header.checksum, udp_expected_checksum,
                UDP_CHECKSUM_BYTE_LENGTH) == 0);
}

/* ********************************************************************** */

void test_udp_parser_from_packet(void) {
  /**
   * @brief Testing the UDP parser on an IPv6 and UDP header.
   *
   * The packet is made of a UDP header with following fields:
   *  - id='App Port'     val={0x1f, 0x90}
   *  - id='Dev Port'     val={0x00, 0x35}
   *  - id='Length'       val={0x00, 0x14}
   *  - id='Checksum'     val={0x00, 0x00}
   *
   */
  const uint8_t ipv6_udp_packet[] = {
      0x6f, 0xf9, 0xaa, 0xf2, 0x00, 0x14, 0x11, 0x40, 0xfe, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01,
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff,
      0xfe, 0x00, 0x00, 0x02, 0x1f, 0x90, 0x00, 0x35, 0x00, 0x14, 0x00, 0x00};

  // UDP expected fields
  const uint8_t udp_expected_app_port[] = {0x1f, 0x90};
  const uint8_t udp_expected_dev_port[] = {0x00, 0x35};
  const uint8_t udp_expected_length[]   = {0x00, 0x14};
  const uint8_t udp_expected_checksum[] = {0x00, 0x00};

  int       next_offset;
  udp_hdr_t udp_parsed_header;

  next_offset = parse_udp_header(&udp_parsed_header, ipv6_udp_packet, 40, 48);

  assert(next_offset == 48);

  // UDP App Port
  assert(memcmp(udp_parsed_header.app_port, udp_expected_app_port,
                UDP_APP_PORT_BYTE_LENGTH) == 0);

  // UDP Dev Port
  assert(memcmp(udp_parsed_header.dev_port, udp_expected_dev_port,
                UDP_DEV_PORT_BYTE_LENGTH) == 0);

  // UDP Length
  assert(memcmp(udp_parsed_header.length, udp_expected_length,
                UDP_LENGTH_BYTE_LENGTH) == 0);

  // UDP Checksum
  assert(memcmp(udp_parsed_header.checksum, udp_expected_checksum,
                UDP_CHECKSUM_BYTE_LENGTH) == 0);
}

/* ********************************************************************** */

int main() {
  test_udp_parser();
  test_udp_parser_from_packet();

  printf("All tests passed!\n");

  return 0;
}
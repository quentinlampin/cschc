#include "parser/parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_udp_parser(void) {
  /**
   * @brief Testing the UDP parser on a UDP header.
   *
   * The packet is made of a UDP header with following fields:
   *  - id='Source Port'          len=16  pos=0  value={0x23, 0x29}
   *  - id='Destination Port'     len=16  pos=0  value={0x23, 0x2a}
   *  - id='Length'               len=16  pos=0  value={0x00, 0x10}
   *  - id='Checksum'             len=16  pos=0  value={0x2d, 0xa1}
   *
   */
  const uint8_t udp_header[] = {
      0x23, 0x29, 0x23, 0x2a, 0x00, 0x10, 0x2d, 0xa1,
  };

  const uint16_t udp_expected_source_port      = 0x2329;
  const uint16_t udp_expected_destination_port = 0x232a;
  const uint16_t udp_expected_length           = 0x0010;
  const uint16_t udp_expected_checksum         = 0x2da1;

  udp_hdr_t udp_parsed_header;
  uint16_t  udp_parsed_source_port;
  uint16_t  udp_parsed_destination_port;
  uint16_t  udp_parsed_length;
  uint16_t  udp_parsed_checksum;

  parse_udp_header(&udp_parsed_header, udp_header, 0, 8);

  // Source Port
  udp_parsed_source_port = network_to_host_short(udp_parsed_header.source_port);
  assert(udp_parsed_source_port == udp_expected_source_port);

  // Destination Port
  udp_parsed_destination_port =
      network_to_host_short(udp_parsed_header.destination_port);
  assert(udp_parsed_destination_port == udp_expected_destination_port);

  // Length
  udp_parsed_length = network_to_host_short(udp_parsed_header.length);
  assert(udp_parsed_length == udp_expected_length);

  // Checksum
  udp_parsed_checksum = network_to_host_short(udp_parsed_header.checksum);
  assert(udp_parsed_checksum == udp_expected_checksum);
}

/* ********************************************************************** */

void test_udp_parser_from_packet(void) {
  /**
   * @brief Testing the UDP parser on an IPv6 and UDP header.
   *
   * The packet is made of a UDP header with following fields:
   *  - id='Source Port'          len=16  pos=0  value={0x1f, 0x90}
   *  - id='Destination Port'     len=16  pos=0  value={0x00, 0x35}
   *  - id='Length'               len=16  pos=0  value={0x00, 0x14}
   *  - id='Checksum'             len=16  pos=0  value={0x00, 0x00}
   *
   */
  const uint8_t ipv6_udp_packet[] = {
      0x60, 0x00, 0x00, 0x00, 0x00, 0x14, 0x11, 0x40, 0xfe, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01,
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff,
      0xfe, 0x00, 0x00, 0x02, 0x1f, 0x90, 0x00, 0x35, 0x00, 0x14, 0x00, 0x00};

  const uint16_t udp_expected_source_port      = 0x1f90;
  const uint16_t udp_expected_destination_port = 0x0035;
  const uint16_t udp_expected_length           = 0x0014;
  const uint16_t udp_expected_checksum         = 0x0000;

  udp_hdr_t udp_parsed_header;
  uint16_t  udp_parsed_source_port;
  uint16_t  udp_parsed_destination_port;
  uint16_t  udp_parsed_length;
  uint16_t  udp_parsed_checksum;

  parse_udp_header(&udp_parsed_header, ipv6_udp_packet, 40, 48);

  // Source Port
  udp_parsed_source_port = network_to_host_short(udp_parsed_header.source_port);
  assert(udp_parsed_source_port == udp_expected_source_port);

  // Destination Port
  udp_parsed_destination_port =
      network_to_host_short(udp_parsed_header.destination_port);
  assert(udp_parsed_destination_port == udp_expected_destination_port);

  // Length
  udp_parsed_length = network_to_host_short(udp_parsed_header.length);
  assert(udp_parsed_length == udp_expected_length);

  // Checksum
  udp_parsed_checksum = network_to_host_short(udp_parsed_header.checksum);
  assert(udp_parsed_checksum == udp_expected_checksum);
}

/* ********************************************************************** */

int main() {
  test_udp_parser();
  test_udp_parser_from_packet();

  printf("All tests passed!\n");

  return 0;
}
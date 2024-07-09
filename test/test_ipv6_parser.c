#include "parser/parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_ipv6_parser(void) {
  /**
   * @brief Testing the IPv6 parser on an IPv6 header.
   *
   * The packet is made of an IPv6 header with following fields:
   *  - id='Version'              len=4    pos=0  val={0x06}
   *  - id='Traffic Class'        len=8    pos=0  val={0x00}
   *  - id='Flow Label'           len=20   pos=0  val={0x00, 0x00, 0x00}
   *  - id='Payload Length'       len=16   pos=0  val={0x10}
   *  - id='Next Header'          len=8    pos=0  val={0x11}
   *  - id='Hop Limit'            len=8    pos=0  val={0x40}
   *  - id='Source Address'       len=128  pos=0  val={0xfe, 0x80, 0x00, 0x00,
   * 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1}
   *  - id='Destination Address'  len=128  pos=0  val={0xfe, 0x80, 0x00, 0x00,
   * 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2}
   */
  const uint8_t ipv6_header[] = {
      0x60, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x40, 0xfe, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1,
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0xa2, 0x23, 0x29, 0x23, 0x2a, 0x00, 0x10, 0x2d, 0xa1,
      0x64, 0x65, 0x61, 0x64, 0x62, 0x65, 0x65, 0x66};

  const uint8_t ipv6_expected_version[]        = {0x06};
  const uint8_t ipv6_expected_traffic_class[]  = {0x00};
  const uint8_t ipv6_expected_flow_label[]     = {0x00, 0x00, 0x00};
  const uint8_t ipv6_expected_payload_length[] = {0x00, 0x10};
  const uint8_t ipv6_expected_next_header[]    = {0x11};
  const uint8_t ipv6_expected_hop_limit[]      = {0x40};
  const uint8_t ipv6_expected_source_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1};
  const uint8_t ipv6_expected_destination_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2};

  ipv6_hdr_t ipv6_parsed_header;
  parse_ipv6_header(&ipv6_parsed_header, ipv6_header, 40);

  // Version
  assert(memcmp(ipv6_parsed_header.version, ipv6_expected_version,
                IPV6_VERSION_BYTE_LENGTH) == 0);

  // Traffic Class
  assert(memcmp(ipv6_parsed_header.traffic_class, ipv6_expected_traffic_class,
                IPV6_TRAFFIC_CLASS_BYTE_LENGTH) == 0);

  // Flow Label
  assert(memcmp(ipv6_parsed_header.flow_label, ipv6_expected_flow_label,
                IPV6_FLOW_LABEL_BYTE_LENGTH) == 0);

  // Payload Length
  assert(memcmp(ipv6_parsed_header.payload_length, ipv6_expected_payload_length,
                IPV6_PAYLOAD_LENGTH_BYTE_LENGTH) == 0);

  // Next Header
  assert(memcmp(ipv6_parsed_header.next_header, ipv6_expected_next_header,
                IPV6_NEXT_HEADER_BYTE_LENGTH) == 0);

  // Hop Limit
  assert(memcmp(ipv6_parsed_header.hop_limit, ipv6_expected_hop_limit,
                IPV6_HOP_LIMIT_BYTE_LENGTH) == 0);

  // Source Address
  assert(memcmp(ipv6_parsed_header.source_address, ipv6_expected_source_address,
                IPV6_SRC_ADDRESS_BYTE_LENGTH) == 0);

  // Destination Address
  assert(memcmp(ipv6_parsed_header.destination_address,
                ipv6_expected_destination_address,
                IPV6_DST_ADDRESS_BYTE_LENGTH) == 0);
}

/* ********************************************************************** */

int main(void) {
  test_ipv6_parser();

  printf("All tests passed!\n");

  return 0;
}
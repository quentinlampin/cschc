#include "parser/protocol/ipv6.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_ipv6_parser(void) {
  /**
   * @brief Testing the IPv6 parser on an IPv6 header.
   *
   * The packet is made of an IPv6 header with following fields:
   *  - id='Version'              val={0x06}
   *  - id='Traffic Class'        val={0xff}
   *  - id='Flow Label'           val={0x09, 0xaa, 0xf2}
   *  - id='Payload Length'       val={0x14}
   *  - id='Next Header'          val={0x11}
   *  - id='Hop Limit'            val={0x40}
   *  - id='Source Address'       val={0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   * 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01}
   *  - id='Destination Address'  val={0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   * 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x02}
   */
  const uint8_t ipv6_header[] = {
      0x6f, 0xf9, 0xaa, 0xf2, 0x00, 0x14, 0x11, 0x40, 0xfe, 0x80,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff,
      0xfe, 0x00, 0x00, 0x01, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x02};

  // IPv6 expected fields
  const uint8_t ipv6_expected_version[]        = {0x06};
  const uint8_t ipv6_expected_traffic_class[]  = {0xff};
  const uint8_t ipv6_expected_flow_label[]     = {0x09, 0xaa, 0xf2};
  const uint8_t ipv6_expected_payload_length[] = {0x00, 0x14};
  const uint8_t ipv6_expected_next_header[]    = {0x11};
  const uint8_t ipv6_expected_hop_limit[]      = {0x40};
  const uint8_t ipv6_expected_source_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01};
  const uint8_t ipv6_expected_destination_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x02};

  int        next_offset;
  ipv6_hdr_t ipv6_parsed_header;

  next_offset = parse_ipv6_header(&ipv6_parsed_header, ipv6_header, 40);

  assert(next_offset == 40);

  // IPv6 Version
  assert(memcmp(ipv6_parsed_header.version, ipv6_expected_version,
                IPV6_VERSION_BYTE_LENGTH) == 0);

  // IPv6 Traffic Class
  assert(memcmp(ipv6_parsed_header.traffic_class, ipv6_expected_traffic_class,
                IPV6_TRAFFIC_CLASS_BYTE_LENGTH) == 0);

  // IPv6 Flow Label
  assert(memcmp(ipv6_parsed_header.flow_label, ipv6_expected_flow_label,
                IPV6_FLOW_LABEL_BYTE_LENGTH) == 0);

  // IPv6 Payload Length
  assert(memcmp(ipv6_parsed_header.payload_length, ipv6_expected_payload_length,
                IPV6_PAYLOAD_LENGTH_BYTE_LENGTH) == 0);

  // IPv6 Next Header
  assert(memcmp(ipv6_parsed_header.next_header, ipv6_expected_next_header,
                IPV6_NEXT_HEADER_BYTE_LENGTH) == 0);

  // IPv6 Hop Limit
  assert(memcmp(ipv6_parsed_header.hop_limit, ipv6_expected_hop_limit,
                IPV6_HOP_LIMIT_BYTE_LENGTH) == 0);

  // IPv6 Source Address
  assert(memcmp(ipv6_parsed_header.source_address, ipv6_expected_source_address,
                IPV6_SRC_ADDRESS_BYTE_LENGTH) == 0);

  // IPv6 Destination Address
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
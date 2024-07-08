#include "parser/parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_unpack_ver_tc_fl(void) {
  const uint32_t packed_value          = 0b01101111111110011010101011110010;
  const uint8_t  ipv6_expected_version = 0b0110;
  const uint8_t  ipv6_expected_traffic_class = 0b11111111;
  const uint32_t ipv6_expected_flow_label    = 0b10011010101011110010;

  uint8_t  ipv6_version, ipv6_traffic_class;
  uint32_t ipv6_flow_label;

  unpack_ver_tc_fl(&ipv6_version, &ipv6_traffic_class, &ipv6_flow_label,
                   packed_value);

  assert(ipv6_version == ipv6_expected_version);
  assert(ipv6_traffic_class == ipv6_expected_traffic_class);
  assert(ipv6_flow_label == ipv6_expected_flow_label);
}

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

  const uint8_t  ipv6_expected_version          = 0x06;
  const uint8_t  ipv6_expected_traffic_class    = 0x00;
  const uint32_t ipv6_expected_flow_label       = 0x000000;
  const uint16_t ipv6_expected_payload_length   = 0x0010;
  const uint8_t  ipv6_expected_next_header      = 0x11;
  const uint8_t  ipv6_expected_hop_limit        = 0x40;
  const uint8_t  ipv6_expected_source_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1};
  const uint8_t ipv6_expected_destination_address[] = {
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2};

  ipv6_hdr_t ipv6_parsed_header;
  uint32_t   ipv6_parsed_ver_tc_fl;
  uint8_t    ipv6_parsed_version;
  uint8_t    ipv6_parsed_traffic_class;
  uint32_t   ipv6_parsed_flow_label;
  uint16_t   ipv6_parsed_payload_length;
  uint8_t    ipv6_parsed_next_header;
  uint8_t    ipv6_parsed_hop_limit;
  uint8_t    ipv6_parsed_source_address[16];
  uint8_t    ipv6_parsed_destination_address[16];

  parse_ipv6_header(&ipv6_parsed_header, ipv6_header, 40);

  // Version, Traffic Class and Flow Label
  ipv6_parsed_ver_tc_fl = network_to_host_long(ipv6_parsed_header.ver_tc_fl);
  unpack_ver_tc_fl(&ipv6_parsed_version, &ipv6_parsed_traffic_class,
                   &ipv6_parsed_flow_label, ipv6_parsed_ver_tc_fl);
  assert(ipv6_parsed_version == ipv6_expected_version);
  assert(ipv6_parsed_traffic_class == ipv6_expected_traffic_class);
  assert(ipv6_parsed_flow_label == ipv6_expected_flow_label);

  // Payload Length
  ipv6_parsed_payload_length =
      network_to_host_short(ipv6_parsed_header.payload_length);
  assert(ipv6_parsed_payload_length == ipv6_expected_payload_length);

  // Next Header
  ipv6_parsed_next_header = ipv6_parsed_header.next_header;
  assert(ipv6_parsed_next_header == ipv6_expected_next_header);

  // Hop Limit
  ipv6_parsed_hop_limit = ipv6_parsed_header.hop_limit;
  assert(ipv6_parsed_hop_limit == ipv6_expected_hop_limit);

  // Source Address
  memcpy(ipv6_parsed_source_address, ipv6_parsed_header.source_address, 16);
  assert(memcmp(ipv6_parsed_source_address, ipv6_expected_source_address, 16) ==
         0);

  // Destination Address
  memcpy(ipv6_parsed_destination_address,
         ipv6_parsed_header.destination_address, 16);
  assert(memcmp(ipv6_parsed_destination_address,
                ipv6_expected_destination_address, 16) == 0);
}

/* ********************************************************************** */

int main(void) {
  test_unpack_ver_tc_fl();

  test_ipv6_parser();

  printf("All tests passed!\n");

  return 0;
}
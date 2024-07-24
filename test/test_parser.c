#include "parser/parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_ipv6_udp_parser_header(void) {
  /**
   * @brief Testing the header_t parser on an IPv6 and UDP stack.
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
   *
   * Then UDP header fields are the following:
   *  - id='App Port'             val={0x1f, 0x90}
   *  - id='Dev Port'             val={0x00, 0x35}
   *  - id='Length'               val={0x00, 0x14}
   *  - id='Checksum'             val={0x00, 0x00}
   */
  const uint8_t ipv6_udp_packet[] = {
      0x6f, 0xf9, 0xaa, 0xf2, 0x00, 0x14, 0x11, 0x40, 0xfe, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01,
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff,
      0xfe, 0x00, 0x00, 0x02, 0x1f, 0x90, 0x00, 0x35, 0x00, 0x14, 0x00, 0x00};

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

  // UDP expected fields
  const uint8_t udp_expected_app_port[] = {0x1f, 0x90};
  const uint8_t udp_expected_dev_port[] = {0x00, 0x35};
  const uint8_t udp_expected_length[]   = {0x00, 0x14};
  const uint8_t udp_expected_checksum[] = {0x00, 0x00};

  header_t hdr;
  int      offset = 0;
  assert(parse_header(&hdr, &offset, ipv6_udp_packet, 48));
  assert(offset == 48);

  // IPv6 Version
  assert(memcmp(hdr.ipv6_hdr.version, ipv6_expected_version,
                IPV6_VERSION_BYTE_LENGTH) == 0);

  // IPv6 Traffic Class
  assert(memcmp(hdr.ipv6_hdr.traffic_class, ipv6_expected_traffic_class,
                IPV6_TRAFFIC_CLASS_BYTE_LENGTH) == 0);

  // IPv6 Flow Label
  assert(memcmp(hdr.ipv6_hdr.flow_label, ipv6_expected_flow_label,
                IPV6_FLOW_LABEL_BYTE_LENGTH) == 0);

  // IPv6 Payload Length
  assert(memcmp(hdr.ipv6_hdr.payload_length, ipv6_expected_payload_length,
                IPV6_PAYLOAD_LENGTH_BYTE_LENGTH) == 0);

  // IPv6 Next Header
  assert(memcmp(hdr.ipv6_hdr.next_header, ipv6_expected_next_header,
                IPV6_NEXT_HEADER_BYTE_LENGTH) == 0);

  // IPv6 Hop Limit
  assert(memcmp(hdr.ipv6_hdr.hop_limit, ipv6_expected_hop_limit,
                IPV6_HOP_LIMIT_BYTE_LENGTH) == 0);

  // IPv6 Source Address
  assert(memcmp(hdr.ipv6_hdr.source_address, ipv6_expected_source_address,
                IPV6_SRC_ADDRESS_BYTE_LENGTH) == 0);

  // IPv6 Destination Address
  assert(memcmp(hdr.ipv6_hdr.destination_address,
                ipv6_expected_destination_address,
                IPV6_DST_ADDRESS_BYTE_LENGTH) == 0);

  // UDP App Port
  assert(memcmp(hdr.udp_hdr.app_port, udp_expected_app_port,
                UDP_APP_PORT_BYTE_LENGTH) == 0);

  // UDP Dev Port
  assert(memcmp(hdr.udp_hdr.dev_port, udp_expected_dev_port,
                UDP_DEV_PORT_BYTE_LENGTH) == 0);

  // UDP Length
  assert(memcmp(hdr.udp_hdr.length, udp_expected_length,
                UDP_LENGTH_BYTE_LENGTH) == 0);

  // UDP Checksum
  assert(memcmp(hdr.udp_hdr.checksum, udp_expected_checksum,
                UDP_CHECKSUM_BYTE_LENGTH) == 0);
}

/* ********************************************************************** */

void test_ipv6_udp_parser_header_wrong_next_header_field(void) {
  /**
   * @brief Testing the header_t parser on an "IPv6 and UDP stack".
   *
   * @details The testing packet (ipv6_udp_packet) is the same as in
   * test_ipv6_udp_parser_header(), but, IPv6 next header value is wrong in this
   * packet. Indeed, 0x06 (TCP) instead of 0x11 (UDP).
   */
  const uint8_t ipv6_udp_packet[] = {
      0x6f, 0xf9, 0xaa, 0xf2, 0x00, 0x14, 0x06, 0x40, 0xfe, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff, 0xfe, 0x00, 0x00, 0x01,
      0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x5e, 0xff,
      0xfe, 0x00, 0x00, 0x02, 0x1f, 0x90, 0x00, 0x35, 0x00, 0x14, 0x00, 0x00};

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

  header_t hdr;
  int      offset = 0;
  assert(!parse_header(&hdr, &offset, ipv6_udp_packet, 48));

  // IPv6 Version
  assert(memcmp(hdr.ipv6_hdr.version, ipv6_expected_version,
                IPV6_VERSION_BYTE_LENGTH) == 0);

  // IPv6 Traffic Class
  assert(memcmp(hdr.ipv6_hdr.traffic_class, ipv6_expected_traffic_class,
                IPV6_TRAFFIC_CLASS_BYTE_LENGTH) == 0);

  // IPv6 Flow Label
  assert(memcmp(hdr.ipv6_hdr.flow_label, ipv6_expected_flow_label,
                IPV6_FLOW_LABEL_BYTE_LENGTH) == 0);

  // IPv6 Payload Length
  assert(memcmp(hdr.ipv6_hdr.payload_length, ipv6_expected_payload_length,
                IPV6_PAYLOAD_LENGTH_BYTE_LENGTH) == 0);

  // IPv6 Next Header
  assert(memcmp(hdr.ipv6_hdr.next_header, ipv6_expected_next_header,
                IPV6_NEXT_HEADER_BYTE_LENGTH) != 0);

  // IPv6 Hop Limit
  assert(memcmp(hdr.ipv6_hdr.hop_limit, ipv6_expected_hop_limit,
                IPV6_HOP_LIMIT_BYTE_LENGTH) == 0);

  // IPv6 Source Address
  assert(memcmp(hdr.ipv6_hdr.source_address, ipv6_expected_source_address,
                IPV6_SRC_ADDRESS_BYTE_LENGTH) == 0);

  // IPv6 Destination Address
  assert(memcmp(hdr.ipv6_hdr.destination_address,
                ipv6_expected_destination_address,
                IPV6_DST_ADDRESS_BYTE_LENGTH) == 0);

  // No needs to test UDP field because there are not parsed has the ipv6 next
  // header is wrong
}

/* ********************************************************************** */

int main() {
  test_ipv6_udp_parser_header();
  test_ipv6_udp_parser_header_wrong_next_header_field();

  printf("All tests passed!\n");

  return 0;
}
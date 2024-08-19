#include "core/compression.h"
#include "utils/memory.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void test_compression_microschc(void) {
  const uint8_t packet[] = {
      0x60, 0x00, 0xef, 0x2d, 0x00, 0x68, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0xd1, 0x00, 0x16, 0x33, 0x00, 0x68, 0x5c, 0x21,
      0x68, 0x45, 0x22, 0xf6, 0xb8, 0x30, 0x0e, 0xfe, 0xe6, 0x62, 0x91, 0x22,
      0xc1, 0x6e, 0xff, 0x5b, 0x7b, 0x22, 0x62, 0x6e, 0x22, 0x3a, 0x22, 0x2f,
      0x36, 0x2f, 0x22, 0x2c, 0x22, 0x6e, 0x22, 0x3a, 0x22, 0x30, 0x2f, 0x30,
      0x22, 0x2c, 0x22, 0x76, 0x22, 0x3a, 0x35, 0x34, 0x2e, 0x30, 0x7d, 0x2c,
      0x7b, 0x22, 0x6e, 0x22, 0x3a, 0x22, 0x30, 0x2f, 0x31, 0x22, 0x2c, 0x22,
      0x76, 0x22, 0x3a, 0x34, 0x38, 0x2e, 0x30, 0x7d, 0x2c, 0x7b, 0x22, 0x6e,
      0x22, 0x3a, 0x22, 0x30, 0x2f, 0x35, 0x22, 0x2c, 0x22, 0x76, 0x22, 0x3a,
      0x31, 0x36, 0x36, 0x36, 0x32, 0x36, 0x33, 0x33, 0x33, 0x39, 0x7d, 0x5d};
  const size_t packet_byte_len = sizeof(packet);

  const uint8_t schc_expected_packet[] = {
      0xc0, 0x1a, 0x00, 0x80, 0x0d, 0x0b, 0x84, 0x30, 0x8a, 0x45, 0xed, 0xe8,
      0x17, 0x06, 0x01, 0xdf, 0xdc, 0xcc, 0x52, 0x24, 0x43, 0x0d, 0xcb, 0x6f,
      0x64, 0x4c, 0x4d, 0xc4, 0x47, 0x44, 0x45, 0xe6, 0xc5, 0xe4, 0x45, 0x84,
      0x4d, 0xc4, 0x47, 0x44, 0x46, 0x05, 0xe6, 0x04, 0x45, 0x84, 0x4e, 0xc4,
      0x47, 0x46, 0xa6, 0x85, 0xc6, 0x0f, 0xa5, 0x8f, 0x64, 0x4d, 0xc4, 0x47,
      0x44, 0x46, 0x05, 0xe6, 0x24, 0x45, 0x84, 0x4e, 0xc4, 0x47, 0x46, 0x87,
      0x05, 0xc6, 0x0f, 0xa5, 0x8f, 0x64, 0x4d, 0xc4, 0x47, 0x44, 0x46, 0x05,
      0xe6, 0xa4, 0x45, 0x84, 0x4e, 0xc4, 0x47, 0x46, 0x26, 0xc6, 0xc6, 0xc6,
      0x46, 0xc6, 0x66, 0x66, 0x67, 0x2f, 0xab, 0xa0};
  const size_t schc_expected_byte_len = sizeof(schc_expected_packet);

  const uint8_t context[] = {
      // Context
      0, 4, 0, 6, 0, 53,  // TO DO CHANGE THIS CONTEXT

      // Rule Descriptor
      0x03, 0, 22, 0, 56, 0, 66, 0, 76, 0, 86, 0, 94, 0, 104, 0, 114, 0, 126, 0,
      136, 0, 146, 0, 156, 0, 164, 0, 172, 0, 182, 0, 192, 0, 200, 0, 208, 0,
      216, 0, 224, 0, 234, 0, 242, 0, 250,  // Rule for compression
      0x01, 1, 0,                           // Rule for no-compression

      // Rule Field Descriptor
      0x13, 0xcc, 0, 4, 0, 0, 64, 1, 0x01, 0x04,  // sid-ipv6-version
                                                  // bi/eq/ns
      0x13, 0xc9, 0, 8, 0, 0, 64, 1, 0x01, 0x05,  // sid-ipv6-trafficclass
                                                  // bi/eq/ns
      0x13, 0xc5, 0, 20, 0, 0, 0, 1, 0x01, 0x06,  // sid-ipv6-flow-label
                                                  // up/eq/ns
      0x13, 0xc8, 0, 16, 0, 0, 75, 0,             // sid-ipv6-payload-length
                                                  // bi/ig/vs
      0x13, 0xc7, 0, 8, 0, 0, 64, 1, 0x01, 0x09,  // sid-ipv6-nextheader
                                                  // bi/eq/ns
      0x13, 0xc6, 0, 8, 0, 0, 64, 1, 0x01, 0x0a,  // sid-ipv6-hop-limit
                                                  // bi/eq/ns
      0x13, 0xc1, 0, 128, 0, 0, 17, 0, 120, 1, 0x01,
      0x0b,  // sid-ipv6-sourceaddress
             // up/msb/lsb
      0x13, 0xc4, 0, 128, 0, 0, 90, 1, 0x01,
      0x0b,  // sid-ipv6-destinationaddress
             // bi/map/map

      0x13, 0xce, 0, 16, 0, 1, 0, 1, 0x01, 0x1b,  // sid-udp-app-port
                                                  // up/eq/ns
      0x13, 0xd1, 0, 16, 0, 1, 0, 1, 0x01, 0x1d,  // sid-udp-dev-port
                                                  // up/eq/ns
      0x13, 0xd2, 0, 16, 0, 1, 75, 0,             // sid-udp-length
                                                  // bi/ig/vs
      0x13, 0xd0, 0, 16, 0, 1, 75, 0,             // sid-udp-checksum
                                                  // bi/ig/vs

      0x13, 0xbf, 0, 2, 0, 0, 64, 1, 0x01, 0x1f,  // sid-coap-version
                                                  // bi/eq/ns
      0x13, 0xbe, 0, 2, 0, 0, 64, 1, 0x01, 0x20,  // sid-coap-type
                                                  // bi/eq/ns
      0x13, 0xbc, 0, 4, 0, 0, 75, 0,              // sid-coap-tkl
                                                  // bi/ig/vs
      0x13, 0x9f, 0, 8, 0, 0, 75, 0,              // sid-coap-code
                                                  // bi/ig/vs
      0x13, 0xa2, 0, 16, 0, 0, 75, 0,             // sid-coap-mid
                                                  // bi/ig/vs
      0x13, 0xbd, 0, 0, 0, 0, 75, 0,              // sid-coap-token
                                                  // bi/ig/vs

      0x14, 0x10, 0, 4, 0, 0, 0, 1, 0x01, 0x21,  // sid-option-delta
                                                 // up/eq/ns
      0x14, 0x12, 0, 4, 0, 0, 11, 0,             // sid-option-length
                                                 // up/ig/vs
      0x14, 0x14, 0, 0, 0, 0, 11, 0,             // sid-option-value
                                                 // up/ig/vs
      0x14, 0x15, 0, 8, 0, 0, 0, 1, 0x01, 0x22,  // sid-payload-marker
                                                 // up/eq/ns
      // Target Value
      0x06,              // IPv6 Version
      0x00,              // IPv6 Traffic class
      0x00, 0xef, 0x2d,  // IPv6 Flow Label
      0x11,              // IPv6 Next Header
      0x40,              // IPv6 Hop Limit
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // IPv6 Addr (SRC + DST)
      0xd1, 0x00,              // UDP App Port
      0x16, 0x33,              // UDP Dev Port
      0x01,                    // CoAP Version
      0x02,                    // CoAP Type
      0x0c,                    // CoAP Option Delta
      0xff                     // CoAP Payload Maker
  };
  const size_t context_len = sizeof(context);

  uint8_t schc_packet[packet_byte_len + 1];
  memset(schc_packet, 0, packet_byte_len + 1);

  size_t schc_byte_len =
      compress(schc_packet, packet_byte_len + 1, DI_UP, packet, packet_byte_len,
               context, context_len);

  assert(schc_byte_len == schc_expected_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet, schc_byte_len) == 0);
}

/* ********************************************************************** */

void test_no_compression(void) {
  const uint8_t packet[] = {
      0x60, 0x00, 0xef, 0x2d, 0x00, 0x68, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0xd1, 0x00, 0x16, 0x33, 0x00, 0x68, 0x5c, 0x21,
      0x68, 0x45, 0x22, 0xf6, 0xb8, 0x30, 0x0e, 0xfe, 0xe6, 0x62, 0x91, 0x22,
      0xc1, 0x6e, 0xff, 0x5b, 0x7b, 0x22, 0x62, 0x6e, 0x22, 0x3a, 0x22, 0x2f,
      0x36, 0x2f, 0x22, 0x2c, 0x22, 0x6e, 0x22, 0x3a, 0x22, 0x30, 0x2f, 0x30,
      0x22, 0x2c, 0x22, 0x76, 0x22, 0x3a, 0x35, 0x34, 0x2e, 0x30, 0x7d, 0x2c,
      0x7b, 0x22, 0x6e, 0x22, 0x3a, 0x22, 0x30, 0x2f, 0x31, 0x22, 0x2c, 0x22,
      0x76, 0x22, 0x3a, 0x34, 0x38, 0x2e, 0x30, 0x7d, 0x2c, 0x7b, 0x22, 0x6e,
      0x22, 0x3a, 0x22, 0x30, 0x2f, 0x35, 0x22, 0x2c, 0x22, 0x76, 0x22, 0x3a,
      0x31, 0x36, 0x36, 0x36, 0x32, 0x36, 0x33, 0x33, 0x33, 0x39, 0x7d, 0x5d};
  const size_t packet_byte_len = sizeof(packet);

  // Packet shifted of 1 bit and rule_id on first bit
  const uint8_t schc_expected_packet[] = {
      0xb0, 0x00, 0x77, 0x96, 0x80, 0x34, 0x08, 0xa0, 0x10, 0x00, 0x86, 0xdc,
      0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0x10, 0x00, 0x86, 0xdc, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x10, 0x68, 0x80, 0x0b, 0x19, 0x80, 0x34, 0x2e, 0x10,
      0xb4, 0x22, 0x91, 0x7b, 0x5c, 0x18, 0x07, 0x7f, 0x73, 0x31, 0x48, 0x91,
      0x60, 0xb7, 0x7f, 0xad, 0xbd, 0x91, 0x31, 0x37, 0x11, 0x1d, 0x11, 0x17,
      0x9b, 0x17, 0x91, 0x16, 0x11, 0x37, 0x11, 0x1d, 0x11, 0x18, 0x17, 0x98,
      0x11, 0x16, 0x11, 0x3b, 0x11, 0x1d, 0x1a, 0x9a, 0x17, 0x18, 0x3e, 0x96,
      0x3d, 0x91, 0x37, 0x11, 0x1d, 0x11, 0x18, 0x17, 0x98, 0x91, 0x16, 0x11,
      0x3b, 0x11, 0x1d, 0x1a, 0x1c, 0x17, 0x18, 0x3e, 0x96, 0x3d, 0x91, 0x37,
      0x11, 0x1d, 0x11, 0x18, 0x17, 0x9a, 0x91, 0x16, 0x11, 0x3b, 0x11, 0x1d,
      0x18, 0x9b, 0x1b, 0x1b, 0x19, 0x1b, 0x19, 0x99, 0x99, 0x9c, 0xbe, 0xae,
      0x80};
  const size_t schc_expected_byte_len = sizeof(schc_expected_packet);

  const uint8_t context[] = {
      // Context
      0, 2, 0, 6, 0, 53,

      // Rule Descriptor
      0x03, 0, 22, 0, 56, 0, 66, 0, 76, 0, 86, 0, 94, 0, 104, 0, 114, 0, 126, 0,
      136, 0, 146, 0, 156, 0, 164, 0, 172, 0, 182, 0, 192, 0, 200, 0, 208, 0,
      216, 0, 224, 0, 234, 0, 242, 0, 250,  // Rule for compression
      0x01, 1, 0,                           // Rule for no-compression

      // Rule Field Descriptor
      0x13, 0xcc, 0, 4, 0, 0, 64, 1, 0x01, 0x04,  // sid-ipv6-version
                                                  // bi/eq/ns
      0x13, 0xc9, 0, 8, 0, 0, 64, 1, 0x01, 0x05,  // sid-ipv6-trafficclass
                                                  // bi/eq/ns
      0x13, 0xc5, 0, 20, 0, 0, 0, 1, 0x01, 0x06,  // sid-ipv6-flow-label
                                                  // up/eq/ns
      0x13, 0xc8, 0, 16, 0, 0, 75, 0,             // sid-ipv6-payload-length
                                                  // bi/ig/vs
      0x13, 0xc7, 0, 8, 0, 0, 64, 1, 0x01, 0x09,  // sid-ipv6-nextheader
                                                  // bi/eq/ns
      0x13, 0xc6, 0, 8, 0, 0, 64, 1, 0x01, 0x0a,  // sid-ipv6-hop-limit
                                                  // bi/eq/ns
      0x13, 0xc1, 0, 128, 0, 0, 17, 0, 120, 1, 0x01,
      0x0b,  // sid-ipv6-sourceaddress
             // up/msb/lsb
      0x13, 0xc4, 0, 128, 0, 0, 90, 1, 0x01,
      0x0b,  // sid-ipv6-destinationaddress
             // bi/map/map

      0x13, 0xce, 0, 16, 0, 1, 0, 1, 0x01, 0x1b,  // sid-udp-app-port
                                                  // up/eq/ns
      0x13, 0xd1, 0, 16, 0, 1, 0, 1, 0x01, 0x1d,  // sid-udp-dev-port
                                                  // up/eq/ns
      0x13, 0xd2, 0, 16, 0, 1, 75, 0,             // sid-udp-length
                                                  // bi/ig/vs
      0x13, 0xd0, 0, 16, 0, 1, 75, 0,             // sid-udp-checksum
                                                  // bi/ig/vs

      0x13, 0xbf, 0, 2, 0, 0, 64, 1, 0x01, 0x1f,  // sid-coap-version
                                                  // bi/eq/ns
      0x13, 0xbe, 0, 2, 0, 0, 64, 1, 0x01, 0x20,  // sid-coap-type
                                                  // bi/eq/ns
      0x13, 0xbc, 0, 4, 0, 0, 75, 0,              // sid-coap-tkl
                                                  // bi/ig/vs
      0x13, 0x9f, 0, 8, 0, 0, 75, 0,              // sid-coap-code
                                                  // bi/ig/vs
      0x13, 0xa2, 0, 16, 0, 0, 75, 0,             // sid-coap-mid
                                                  // bi/ig/vs
      0x13, 0xbd, 0, 0, 0, 0, 75, 0,              // sid-coap-token
                                                  // bi/ig/vs

      0x14, 0x10, 0, 4, 0, 0, 0, 1, 0x01, 0x21,  // sid-option-delta
                                                 // up/eq/ns
      0x14, 0x12, 0, 4, 0, 0, 11, 0,             // sid-option-length
                                                 // up/ig/vs
      0x14, 0x14, 0, 0, 0, 0, 11, 0,             // sid-option-value
                                                 // up/ig/vs
      0x14, 0x15, 0, 8, 0, 0, 0, 1, 0x01, 0x22,  // sid-payload-marker
                                                 // up/eq/ns
      // Target Value
      0x06,              // IPv6 Version
      0x00,              // IPv6 Traffic class
      0x00, 0xef, 0x2d,  // IPv6 Flow Label
      0x11,              // IPv6 Next Header
      0x40,              // IPv6 Hop Limit
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // IPv6 Addr (SRC + DST)
      0xd1, 0x00,              // UDP App Port
      0x16, 0x33,              // UDP Dev Port
      0x02,  // CoAP Version (Change this value in order to trigger
             // NO-COMPRESSION)
      0x02,  // CoAP Type
      0x0c,  // CoAP Option Delta
      0xff   // CoAP Payload Maker
  };
  const size_t context_len = sizeof(context);

  uint8_t schc_packet[packet_byte_len + 1];
  memset(schc_packet, 0, packet_byte_len + 1);

  size_t schc_byte_len =
      compress(schc_packet, packet_byte_len + 1, DI_UP, packet, packet_byte_len,
               context, context_len);

  assert(schc_byte_len == schc_expected_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet, schc_byte_len) == 0);
}

void test_several_packets(void) {
  const uint8_t context[] = {
      // Context
      0, 2, 0, 6, 0, 59,

      // Rule Descriptor
      // 6
      0x00, 0, 25, 0, 62, 0, 72, 0, 82, 0, 92, 0, 100, 0, 110, 0, 120, 0, 130,
      0, 140, 0, 150, 0, 160, 0, 168, 0, 176, 0, 186, 0, 198, 0, 208, 0, 218, 0,
      226, 0, 246, 0x01, 0x00, 0x01, 0x0c, 0x01, 0x14, 0x01, 0x1e, 0x01, 0x2a,
      0x01, 0x36,  // Rule for compression
      // 59
      0x01, 1, 0,  // Rule for no-compression

      // Rule Field Descriptor
      // 62
      0x13, 0xcc, 0, 4, 0, 1, 64, 1, 0x01, 0x40,  // sid-ipv6-version
                                                  // bi/eq/ns
      // 72
      0x13, 0xc9, 0, 8, 0, 1, 64, 1, 0x01, 0x41,  // sid-ipv6-trafficclass
                                                  // bi/eq/ns
      // 82
      0x13, 0xc5, 0, 20, 0, 1, 0, 1, 0x01, 0x42,  // sid-ipv6-flow-label
                                                  // up/eq/ns
      // 92
      0x13, 0xc8, 0, 16, 0, 1, 75, 0,  // sid-ipv6-payload-length
                                       // bi/ig/vs
      // 100
      0x13, 0xc7, 0, 8, 0, 1, 64, 1, 0x01, 0x45,  // sid-ipv6-nextheader
                                                  // bi/eq/ns
      // 110
      0x13, 0xc6, 0, 8, 0, 1, 64, 1, 0x01, 0x46,  // sid-ipv6-hop-limit
                                                  // bi/eq/ns
      // 120
      0x13, 0xc1, 0, 128, 0, 1, 0, 1, 0x01, 0x47,  // sid-ipv6-sourceaddress
                                                   // up/eq/ns
      // 130
      0x13, 0xc4, 0, 128, 0, 1, 0, 1, 0x01,
      0x57,  // sid-ipv6-destinationaddress
             // up/eq/ns
      // 140
      0x13, 0xce, 0, 16, 0, 1, 0, 1, 0x01, 0x67,  // sid-udp-app-port
                                                  // up/eq/ns
      // 150
      0x13, 0xd1, 0, 16, 0, 1, 0, 1, 0x01, 0x69,  // sid-udp-dev-port
                                                  // up/eq/ns
      // 160
      0x13, 0xd2, 0, 16, 0, 1, 75, 0,  // sid-udp-length
                                       // bi/ig/vs
      // 168
      0x13, 0xd0, 0, 16, 0, 1, 75, 0,  // sid-udp-checksum
                                       // bi/ig/vs
      // 176
      0x13, 0xbf, 0, 2, 0, 1, 64, 1, 0x01, 0x6b,  // sid-coap-version
                                                  // bi/eq/ns
      // 186
      0x13, 0xbe, 0, 2, 0, 1, 90, 2, 0x01, 0x6b, 0x01, 0x41,  // sid-coap-type
                                                              // bi/map/map
      // 198
      0x13, 0xbc, 0, 4, 0, 1, 64, 1, 0x01, 0x6c,  // sid-coap-tkl
                                                  // bi/eq/ns
      // 208
      0x13, 0x9f, 0, 8, 0, 1, 64, 1, 0x01, 0x6d,  // sid-coap-code
                                                  // bi/eq/ns
      // 218
      0x13, 0xa2, 0, 16, 0, 1, 75, 0,  // sid-coap-mid
                                       // bi/ig/vs
      // 226
      0x13, 0xbd, 0, 0, 0, 1, 90, 6, 0x01, 0x72, 0x01, 0x74, 0x01, 0x76, 0x01,
      0x78, 0x01, 0x7a, 0x01, 0x7c,  // sid-coap-token
                                     // bi/map/map
      // 246
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x01, 0x40,  // sid-option-delta
                                                 // up/eq/ns
      // 256
      0x14, 0x12, 0, 4, 0, 1, 26, 2, 0x01, 0x6b, 0x01,
      0x6c,  // sid-option-length
             // up/map/map
      // 268
      0x14, 0x14, 0, 0, 0, 1, 11, 0,  // sid-option-value
                                      // up/ig/vs
      // 276
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x01, 0x40,  // sid-option-delta
                                                 // up/eq/ns

      // 286
      0x14, 0x12, 0, 4, 0, 1, 26, 2, 0x01, 0x6b, 0x01,
      0x6c,  // sid-option-length
             // up/map/map
      // 298
      0x14, 0x14, 0, 0, 0, 1, 26, 2, 0x01, 0x6f, 0x01,
      0x71,  // sid-option-value
             // up/map/map
      // 310
      0x14, 0x15, 0, 8, 0, 1, 0, 1, 0x01, 0x6e,  // sid-payload-marker
                                                 // up/eq/ns
      // Target Value
      // 320
      0x06,  // IPv6 Version + CoAP Option Delta
      // 321
      0x00,  // IPv6 Traffic class + CoAP Type
      // 322
      0x0f, 0xf8, 0x5f,  // IPv6 Flow Label
      // 325
      0x11,  // IPv6 Next Header
      // 326
      0x40,  // IPv6 Hop Limit
      // 327
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x03,  // IPv6 Addr (SRC + DST)
      // 343
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // IPv6 Addr (SRC + DST)
      // 359
      0x90, 0xa0,  // UDP App Port
      // 361
      0x16, 0x33,  // UDP Dev Port
      // 363
      0x01,  // CoAP Version + CoAP Type + CoAP Option Length
      // 364
      0x02,  // CoAP TKL + CoAP Option Length
      // 365
      0x45,  // CoAP Code
      // 366
      0xff,  // CoAP Payload Maker
      // 367
      0x2d, 0x16,  // CoAP Option Value
      // 369
      0x3c,  // CoAP Option Value
      // 370
      0x21, 0x50,  // CoAP Token
      // 372
      0x1f, 0x0a,  // CoAP Token
      // 374
      0xb7, 0x25,  // CoAP Token
      // 376
      0x37, 0x09,  // CoAP Token
      // 378
      0xd1, 0x59,  // CoAP Token
      // 380
      0x8d, 0x43  // CoAP Token
  };
  const size_t context_len = sizeof(context);

  size_t  schc_byte_len;
  uint8_t schc_packet[150];

  // Compress packet1
  const uint8_t packet1[] = {
      0x60, 0x0f, 0xf8, 0x5f, 0x00, 0x1d, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x90, 0xa0, 0x16, 0x33, 0x00, 0x1d, 0x13, 0xed,
      0x52, 0x45, 0x15, 0x52, 0x1f, 0x0a, 0x62, 0x01, 0x1e, 0x61, 0x3c, 0xff,
      0xfb, 0x40, 0x43, 0x6c, 0xcc, 0xcc, 0xcc, 0xcc, 0xcd};
  const size_t packet1_byte_len = sizeof(packet1);

  const uint8_t schc_expected_packet1[] = {
      0x00, 0x0e, 0x80, 0x0e, 0x89, 0xf6, 0x85, 0x54, 0x8f, 0xc4, 0x00,
      0x47, 0x9f, 0xb4, 0x04, 0x36, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xd0};
  const size_t schc_expected_packet1_byte_len = sizeof(schc_expected_packet1);

  memset(schc_packet, 0, 150);
  schc_byte_len = compress(schc_packet, 150, DI_UP, packet1, packet1_byte_len,
                           context, context_len);

  assert(schc_byte_len == schc_expected_packet1_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet1, schc_byte_len) == 0);

  // Compress packet2
  const uint8_t packet2[] = {
      0x60, 0x0f, 0xf8, 0x5f, 0x00, 0x21, 0x11, 0x40, 0x20, 0x01, 0x0d,
      0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x03, 0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x90, 0xa0, 0x16, 0x33,
      0x00, 0x21, 0x1a, 0xb3, 0x52, 0x45, 0x17, 0xd4, 0xd1, 0x59, 0x62,
      0x03, 0xb8, 0x62, 0x2d, 0x16, 0xff, 0xe8, 0x16, 0x44, 0x08, 0x40,
      0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};
  const size_t packet2_byte_len = sizeof(packet2);

  const uint8_t schc_expected_packet2[] = {
      0x00, 0x10, 0x80, 0x10, 0x8d, 0x59, 0x85, 0xf5, 0x27,
      0xc4, 0x00, 0xee, 0x2e, 0x81, 0x64, 0x40, 0x84, 0x04,
      0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const size_t schc_expected_packet2_byte_len = sizeof(schc_expected_packet2);

  memset(schc_packet, 0, 150);
  schc_byte_len = compress(schc_packet, 150, DI_UP, packet2, packet2_byte_len,
                           context, context_len);

  assert(schc_byte_len == schc_expected_packet2_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet2, schc_byte_len) == 0);
}

void test_coap_option_extended(void) {
  const uint8_t context[] = {
      // Context
      0, 2, 0, 6, 0, 93,

      // Rule Descriptor
      // 6
      0x00, 0, 42, 0, 96, 0, 106, 0, 116, 0, 126, 0, 134, 0, 144, 0, 154, 0,
      164, 0, 174, 0, 184, 0, 194, 0, 202, 0, 210, 0, 220, 0, 230, 0, 240, 0,
      250, 0x01, 0x06, 0x01, 0x0e, 0x01, 0x18, 0x01, 0x22, 0x01, 0x2a, 0x01,
      0x34, 0x01, 0x3e, 0x01, 0x48, 0x01, 0x52, 0x01, 0x5c, 0x01, 0x6a, 0x01,
      0x74,

      0x01, 0x7c, 0x01, 0x84, 0x01, 0x8e, 0x01, 0x98,

      0x01, 0xa2, 0x01, 0xac, 0x01, 0xb6, 0x01, 0xc0, 0x01, 0xc8, 0x01, 0xd2,
      0x01, 0xdc, 0x01, 0xe6, 0x01, 0xee,
      // Rule for compression
      // 93
      0x01, 1, 0,  // Rule for no-compression

      // Rule Field Descriptor
      // 96
      0x13, 0xcc, 0, 4, 0, 1, 64, 1, 0x01, 0xf8,  // sid-ipv6-version
                                                  // bi/eq/ns
      // 106
      0x13, 0xc9, 0, 8, 0, 1, 64, 1, 0x01, 0xf9,  // sid-ipv6-trafficclass
                                                  // bi/eq/ns
      // 116
      0x13, 0xc5, 0, 20, 0, 1, 0, 1, 0x01, 0xfa,  // sid-ipv6-flow-label
                                                  // up/eq/ns
      // 126
      0x13, 0xc8, 0, 16, 0, 1, 75, 0,  // sid-ipv6-payload-length
                                       // bi/ig/vs
      // 134
      0x13, 0xc7, 0, 8, 0, 1, 64, 1, 0x01, 0xfd,  // sid-ipv6-nextheader
                                                  // bi/eq/ns
      // 144
      0x13, 0xc6, 0, 8, 0, 1, 64, 1, 0x01, 0xfe,  // sid-ipv6-hop-limit
                                                  // bi/eq/ns
      // 154
      0x13, 0xc1, 0, 128, 0, 1, 0, 1, 0x01, 0xff,  // sid-ipv6-sourceaddress
                                                   // up/eq/ns
      // 164
      0x13, 0xc4, 0, 128, 0, 1, 0, 1, 0x02,
      0x0f,  // sid-ipv6-destinationaddress
             // up/eq/ns
      // 174
      0x13, 0xce, 0, 16, 0, 1, 0, 1, 0x02, 0x1f,  // sid-udp-app-port
                                                  // up/eq/ns
      // 184
      0x13, 0xd1, 0, 16, 0, 1, 0, 1, 0x02, 0x21,  // sid-udp-dev-port
                                                  // up/eq/ns
      // 194
      0x13, 0xd2, 0, 16, 0, 1, 75, 0,  // sid-udp-length
                                       // bi/ig/vs
      // 202
      0x13, 0xd0, 0, 16, 0, 1, 75, 0,  // sid-udp-checksum
                                       // bi/ig/vs

      // CoAP
      // 210
      0x13, 0xbf, 0, 2, 0, 1, 64, 1, 0x02, 0x23,  // sid-coap-version (0x01)
                                                  // bi/eq/ns
      // 220
      0x13, 0xbe, 0, 2, 0, 1, 0, 1, 0x01, 0xf9,  // sid-coap-type (0x00)
                                                 // up/eq/ns
      // 230
      0x13, 0xbc, 0, 4, 0, 1, 0, 1, 0x02, 0x24,  // sid-coap-tkl (0x08)
                                                 // up/eq/ns
      // 240
      0x13, 0x9f, 0, 8, 0, 1, 0, 1, 0x02, 0x25,  // sid-coap-code (0x02)
                                                 // up/eq/ns
      // 250
      0x13, 0xa2, 0, 16, 0, 1, 17, 0, 10, 1, 0x02, 0x25,  // sid-coap-mid (0x02,
                                                          // 0x12) up/msb/lsb
      // 262
      0x13, 0xbd, 0, 0, 0, 1, 11, 0,  // sid-coap-token
                                      // up/ig/vs

      // OPT 1
      // 270
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x02, 0x27,  // sid-option-delta (0x0b)
                                                 // up/eq/ns
      // 280
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x02, 0x25,  // sid-option-length (0x02)
                                                 // up/eq/ns
      // 290
      0x14, 0x14, 0, 0, 0, 1, 11, 0,  // sid-option-value
                                      // up/ig/vs

      // OPT 2
      // 298
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x02, 0x23,  // sid-option-delta (0x01)
                                                 // up/eq/ns
      // 308
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x02, 0x23,  // sid-option-length (0x01)
                                                 // up/eq/ns
      // 318
      0x14, 0x14, 0, 0, 0, 1, 0, 1, 0x02, 0x28,  // sid-option-value (0x28) ???
                                                 // len=8 up/eq/ns

      // OPT 3
      // 328
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x02, 0x29,  // sid-option-delta (0x03)
                                                 // up/eq/ns
      // 338
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x02, 0x29,  // sid-option-length (0x03)
                                                 // up/eq/ns
      // 348
      0x14, 0x14, 0, 0, 0, 1, 26, 3, 0x02, 0x2a, 0x02, 0x2d, 0x02,
      0x30,  // sid-option-value (??len=24) (0x62, 0x3d, 0x55 or 0xab, 0xcd,
             // 0xef or 0x77, 0x00, 0xff) up/map/map

      // OPT 4
      // 362
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x01, 0xf9,  // sid-option-delta (0x00)
                                                 // up/eq/ns
      // 372
      0x14, 0x12, 0, 4, 0, 1, 11, 0,  // sid-option-length
                                      // up/ig/vs
      // 380
      0x14, 0x14, 0, 0, 0, 1, 11, 0,  // sid-option-value
                                      // up/ig/vs

      // OPT 5
      // 388
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x01, 0xf9,  // sid-option-delta (0x00)
                                                 // up/eq/ns
      // 398
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x01, 0xf8,  // sid-option-length (0x06)
                                                 // up/eq/ns
      // 408
      0x14, 0x14, 0, 0, 0, 1, 0, 1, 0x02,
      0x33,  // sid-option-value (0x6c,0x74,0x3d,0x33,0x30,0x30)
             // up/eq/ns

      // OPT 6
      // 418
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x01, 0xf9,  // sid-option-delta (0x00)
                                                 // up/eq/ns
      // 428
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x02, 0x39,  // sid-option-length (0x0d)
                                                 // up/eq/ns
      // 438
      0x14, 0x13, 0, 0, 0, 1, 0, 1, 0x02, 0x25,  // sid-option-length-extended
                                                 // (0x02) (???len=8) up/eq/ns
      // 448
      0x14, 0x14, 0, 0, 0, 1, 11, 0,  // sid-option-value
                                      // up/ig/vs

      // OPT 7
      // 456
      0x14, 0x10, 0, 4, 0, 1, 0, 1, 0x02, 0x39,  // sid-option-delta (0x0d)
                                                 // up/eq/ns
      // 466
      0x14, 0x12, 0, 4, 0, 1, 0, 1, 0x02, 0x25,  // sid-option-length (0x02)
                                                 // up/eq/ns
      // 476
      0x14, 0x11, 0, 0, 0, 1, 0, 1, 0x02, 0x3a,  // sid-option-delta-extended
                                                 // (0x14) (???len=8) up/eq/ns
      // 486
      0x14, 0x14, 0, 0, 0, 1, 11, 0,  // sid-option-value
                                      // up/ig/vs

      // 494
      0x14, 0x15, 0, 8, 0, 1, 0, 1, 0x02, 0x3b,  // sid-payload-marker (0xff)
                                                 // up/eq/ns
      // Target Value
      // 504 (0x01, 0xf8)
      0x06,  // IPv6 Version

      // 505 (0x01, 0xf9)
      0x00,  // IPv6 Traffic class + CoAP Type + Option Del 4/5/6

      // 506 (0x01, 0xfa)
      0x0f, 0xf8, 0x5f,  // IPv6 Flow Label

      // 509 (0x01, 0xfd)
      0x11,  // IPv6 Next Header

      // 510 (0x01, 0xfe)
      0x40,  // IPv6 Hop Limit

      // 511 (0x01, 0xff)
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x03,  // IPv6 Addr SRC

      // 527 (0x02, 0x0f)
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // IPv6 Addr DST

      // 543 (0x02, 0x1f)
      0x90, 0xa0,  // UDP App Port

      // 545 (0x02, 0x21)
      0x16, 0x33,  // UDP Dev Port

      // CoAP
      // 547 (0x02, 0x23)
      0x01,  // CoAP Version + (Option DEL+LEN 2)

      // 548 (0x02, 0x24)
      0x08,  // CoAP TKL

      // 549 (0x02, 0x25)
      0x02,  // CoAP Code + Option LEN 1/7 + LEN-EXT 6
      0x12,  // following part of CoAP MID

      // 551 (0x02, 0x27)
      0x0b,  // Option DEL 1

      // 552  (0x02, 0x28)
      0x28,  // Option VAL 2

      // 553 (0x02, 0x29)
      0x03,  // Option DEL 3 + LEN 3

      // 554 (0x02, 0x2a)
      0x62, 0x3d, 0x55,  // Option VAL 3

      // 557 (0x02, 0x2d)
      0xab, 0xcd, 0xef,  // Option VAL 3

      // 560 (0x02, 0x30)
      0x77, 0x00, 0xff,  // Option VAL 3

      // 563 (0x02, 0x33)
      0x6c, 0x74, 0x3d, 0x33, 0x30, 0x30,  // Option VAL 5

      // 569 (0x02, 0x39)
      0x0d,  // Option LEN 6 + DEL 7

      // 570 (0x02, 0x3a)
      0x14,  // Option DEL-EXT 7

      // 571 (0x02, 0x3b)
      0xff  // Payload maker
  };
  const size_t context_len = sizeof(context);

  size_t  schc_byte_len;
  uint8_t schc_packet[150];

  // Compress packet1
  const uint8_t packet1[] = {
      0x60, 0x0f, 0xf8, 0x5f, 0x00, 0x1d, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x90, 0xa0, 0x16, 0x33, 0x00, 0x1d, 0x13, 0xed,
      0x48, 0x02, 0x84, 0x99, 0x74, 0xcd, 0xe8, 0xcb, 0x4e, 0x8c, 0x0d, 0xb7,
      0xb2, 0x72, 0x64, 0x11, 0x28, 0x33, 0x62, 0x3d, 0x55, 0x09, 0x6c, 0x77,
      0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31, 0x06, 0x6c, 0x74, 0x3d, 0x33,
      0x30, 0x30, 0x0d, 0x02, 0x65, 0x70, 0x3d, 0x38, 0x35, 0x62, 0x61, 0x39,
      0x62, 0x64, 0x61, 0x63, 0x30, 0x62, 0x65, 0xd2, 0x14, 0x07, 0x2b, 0xff,
      0x68, 0X33, 0X6c, 0X6c, 0X30, 0X0a};
  const size_t packet1_byte_len = sizeof(packet1);

  const uint8_t schc_expected_packet1[] = {
      0x00, 0x0e, 0x80, 0x0e, 0x89, 0xf6, 0xb3, 0xe8, 0x0e, 0x99, 0xbd, 0x19,
      0x69, 0xd1, 0x81, 0xb6, 0xfe, 0x20, 0xe4, 0xc8, 0x4f, 0xa4, 0x36, 0x3b,
      0xb6, 0x99, 0x36, 0x9e, 0x98, 0x97, 0x18, 0xfb, 0xc3, 0x2b, 0x81, 0xe9,
      0xc1, 0xab, 0x13, 0x09, 0xcb, 0x13, 0x23, 0x0b, 0x19, 0x83, 0x13, 0x2f,
      0x88, 0x03, 0x95, 0xb4, 0x19, 0xb6, 0x36, 0x18, 0x05, 0x00};
  const size_t schc_expected_packet1_byte_len = sizeof(schc_expected_packet1);

  memset(schc_packet, 0, 150);
  schc_byte_len = compress(schc_packet, 150, DI_UP, packet1, packet1_byte_len,
                           context, context_len);

  assert(schc_byte_len == schc_expected_packet1_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet1, schc_byte_len) == 0);

  // Compress packet2
  const uint8_t packet2[] = {
      0x60, 0x0f, 0xf8, 0x5f, 0x00, 0x1d, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x90, 0xa0, 0x16, 0x33, 0x00, 0x1d, 0x13, 0xed,
      0x48, 0x02, 0x84, 0x9f, 0x74, 0xcd, 0xe8, 0xcb, 0x4e, 0x8c, 0x0d, 0xb7,
      0xb2, 0x72, 0x64, 0x11, 0x28, 0x33, 0x77, 0x00, 0xff, 0x09, 0x6c, 0x77,
      0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31, 0x06, 0x6c, 0x74, 0x3d, 0x33,
      0x30, 0x30, 0x0d, 0x02, 0x65, 0x77, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45,
      0x62, 0x64, 0x61, 0x63, 0x30, 0x62, 0x65, 0xd2, 0x14, 0x07, 0x2b, 0xff,
      0x6f, 0x72, 0x61, 0x6e, 0x67, 0x65, 0x3c, 0x33};
  const size_t packet2_byte_len = sizeof(packet2);

  const uint8_t schc_expected_packet2[] = {
      0x00, 0x0e, 0x80, 0x0e, 0x89, 0xf6, 0xbf, 0xe8, 0x0e, 0x99, 0xbd, 0x19,
      0x69, 0xd1, 0x81, 0xb6, 0xfe, 0x20, 0xe4, 0xc9, 0x4f, 0xa4, 0x36, 0x3b,
      0xb6, 0x99, 0x36, 0x9e, 0x98, 0x97, 0x18, 0xfb, 0xc3, 0x2b, 0xbd, 0x5e,
      0x6f, 0x78, 0x09, 0x1a, 0x2b, 0x13, 0x23, 0x0b, 0x19, 0x83, 0x13, 0x2f,
      0x88, 0x03, 0x95, 0xb7, 0xb9, 0x30, 0xb7, 0x33, 0xb2, 0x9e, 0x19, 0x80};
  const size_t schc_expected_packet2_byte_len = sizeof(schc_expected_packet2);

  memset(schc_packet, 0, 150);
  schc_byte_len = compress(schc_packet, 150, DI_UP, packet2, packet2_byte_len,
                           context, context_len);

  assert(schc_byte_len == schc_expected_packet2_byte_len);
  assert(memcmp(schc_packet, schc_expected_packet2, schc_byte_len) == 0);
}

/* ********************************************************************** */

int main(void) {
  init_memory_pool();

  test_compression_microschc();
  test_no_compression();
  test_several_packets();
  test_coap_option_extended();

  destroy_memory_pool();

  printf("All tests passed !\n");

  return 0;
}
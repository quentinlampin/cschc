#include "core/rule_field_descriptor.h"
#include "protocols/headers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ********************************************************************** */

void test_unpacking_di_mo_cda(void) {
  direction_indicator_t              di;
  matching_operator_t                mo;
  compression_decompression_action_t cda;
  uint8_t                            packed_value;

  /**
   * @brief Unpack 0b10000000
   *
   * @details :
   * - di  = 00  (DI_UP)
   * - mo  = 00  (MO_EQUAL)
   * - cda = 000 (CDA_NOT_SENT)
   */
  packed_value = 0b10000000;
  unpack_di_mo_cda(&di, &mo, &cda, packed_value);
  assert(di == DI_UP);
  assert(mo == MO_EQUAL);
  assert(cda == CDA_NOT_SENT);

  /**
   * @brief Unpack 0b01001100
   *
   * @details :
   * - di  = 10  (DI_BI)
   * - mo  = 01  (MO_IGNORE)
   * - cda = 100 (CDA_COMPUTE)
   */
  packed_value = 0b01001100;
  unpack_di_mo_cda(&di, &mo, &cda, packed_value);
  assert(di == DI_BI);
  assert(mo == MO_IGNORE);
  assert(cda == CDA_COMPUTE);

  /**
   * @brief Unpack 0b00111000
   *
   * @details :
   * - di  = 01  (DI_DW)
   * - mo  = 11  (MO_MATCH_MAPPING)
   * - cda = 000 (CDA_NOT_SENT)
   */
  packed_value = 0b00111000;
  unpack_di_mo_cda(&di, &mo, &cda, packed_value);
  assert(di == DI_DW);
  assert(mo == MO_MATCH_MAPPING);
  assert(cda == CDA_NOT_SENT);
}

/* ********************************************************************** */

void test_get_rule_field_descriptor(void) {
  const uint8_t context[] = {
      // Context
      0, 2, 0, 6, 0, 33,  // 2 Rules

      // Rule Descriptor
      0, 0, 12, 0, 36, 0, 46, 0, 58, 0, 70, 0, 78, 0, 88, 0, 98, 0, 108, 0, 118,
      0, 128, 0, 138, 0, 146,  // First rule for compression
      1, 1, 0,                 // Second rule for no-compression

      // Rule Field Descriptor
      0x13, 0xcc, 0, 4, 0, 1, 64, 1, 0, 154,  // sid-ipv6-version ; bi/eq/ns
      0x13, 0xc9, 0, 8, 0, 1, 81, 0, 4, 1, 0,
      155,  // sid-ipv6-trafficclass ; bi/msb(4)/lsb
      0x13, 0xc5, 0, 20, 0, 1, 90, 2, 0, 155, 0,
      158,                             // sid-ipv6-flowlabel ; bi/map/map
      0x13, 0xc8, 0, 16, 0, 1, 76, 0,  // sid-ipv6-payload-length ; bi/ig/co
      0x13, 0xc7, 0, 8, 0, 1, 64, 1, 0, 161,  // sid-ipv6-nextheader ; bi/eq/ns
      0x13, 0xc6, 0, 8, 0, 1, 64, 1, 0, 162,  // sid-ipv6-hoplimit ; bi/eq/ns
      0x13, 0xc1, 0, 128, 0, 1, 64, 1, 0,
      163,  // sid-ipv6-sourceaddress ; bi/eq/ns
      0x13, 0xc4, 0, 128, 0, 1, 64, 1, 0,
      179,  // sid-ipv6-destinationaddress ; bi/eq/ns

      0x13, 0xce, 0, 16, 0, 1, 64, 1, 0, 195,  // sid-udp-app-port ; bi/eq/ns
      0x13, 0xd1, 0, 16, 0, 1, 64, 1, 0, 197,  // sid-udp-dev-port ; bi/eq/ns
      0x13, 0xd2, 0, 16, 0, 1, 76, 0,          // sid-udp-length ; bi/ig/co
      0x13, 0xd0, 0, 16, 0, 1, 76, 0,          // sid-udp-checksum ; bi/ig/co

      // Target Value
      0x06,              // sid-ipv6-version
      0x0f,              // sid-ipv6-trafficclass
      0x58, 0x0f,        // sid-ipv6-flowlabel(begin at sid-ipv6-trafficclass)
      0x09, 0xaa, 0xf2,  // sid-ipv6-flowlabel
      0x11,              // sid-ipv6-nextheader
      0x40,              // sid-ipv6-hoplimit
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x03,  // sid-ipv6-sourceaddress
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // sid-ipv6-destinationaddress
      0x90, 0xa0,              // sid-udp-app-port
      0x16, 0x33,              // sid-udp-dev-port
  };
  const size_t context_byte_len = sizeof(context);

  uint16_t                rule_descriptor_offset = 0x0006;
  rule_field_descriptor_t rule_field_descriptor;

  /**
   * @brief Get Rule Field Descriptor 0 (sid-ipv6-version)
   *
   * @details :
   * - SID                        = 0x13cc (SID_IPV6_VERSION)
   * - LEN                        = 4
   * - POS                        = 1
   * - DI                         = DI_BI
   * - MO                         = MO_EQUAL
   * - CDA                        = CDA_NOT_SENT
   * - msb_len                    = 0 (not a MSB/LSB)
   * - card_target_value          = 1
   * - first_target_value_offset  = 154
   */
  assert(get_rule_field_descriptor(&rule_field_descriptor, 0,
                                   rule_descriptor_offset, context,
                                   context_byte_len));
  assert(rule_field_descriptor.sid == SID_IPV6_VERSION);
  assert(rule_field_descriptor.len == 4);
  assert(rule_field_descriptor.pos == 1);
  assert(rule_field_descriptor.di == DI_BI);
  assert(rule_field_descriptor.mo == MO_EQUAL);
  assert(rule_field_descriptor.cda == CDA_NOT_SENT);
  assert(rule_field_descriptor.msb_len == 0);
  assert(rule_field_descriptor.card_target_value == 1);
  assert(rule_field_descriptor.first_target_value_offset == 154);

  /**
   * @brief Get Rule Field Descriptor 1 (sid-ipv6-trafficclass)
   *
   * @details :
   * - SID                        = 0x13c9 (SID_IPV6_TRAFFIC_CLASS)
   * - LEN                        = 8
   * - POS                        = 1
   * - DI                         = DI_BI
   * - MO                         = MO_MSB
   * - CDA                        = CDA_LSB
   * - msb_len                    = 4
   * - card_target_value          = 1
   * - first_target_value_offset  = 155
   */
  assert(get_rule_field_descriptor(&rule_field_descriptor, 1,
                                   rule_descriptor_offset, context,
                                   context_byte_len));
  assert(rule_field_descriptor.sid == SID_IPV6_TRAFFIC_CLASS);
  assert(rule_field_descriptor.len == 8);
  assert(rule_field_descriptor.pos == 1);
  assert(rule_field_descriptor.di == DI_BI);
  assert(rule_field_descriptor.mo == MO_MSB);
  assert(rule_field_descriptor.cda == CDA_LSB);
  assert(rule_field_descriptor.msb_len == 4);
  assert(rule_field_descriptor.card_target_value == 1);
  assert(rule_field_descriptor.first_target_value_offset == 155);

  /**
   * @brief Get Rule Field Descriptor 11 (sid-udp-checksum)
   *
   * @details :
   * - SID                        = 0x13d0 (SID_UDP_CHECKSUM)
   * - LEN                        = 16
   * - POS                        = 1
   * - DI                         = DI_BI
   * - MO                         = MO_IGNORE
   * - CDA                        = CDA_COMPUTE
   * - msb_len                    = 0
   * - card_target_value          = 0
   * - first_target_value_offset  = 0
   */
  assert(get_rule_field_descriptor(&rule_field_descriptor, 11,
                                   rule_descriptor_offset, context,
                                   context_byte_len));
  assert(rule_field_descriptor.sid == SID_UDP_CHECKSUM);
  assert(rule_field_descriptor.len == 16);
  assert(rule_field_descriptor.pos == 1);
  assert(rule_field_descriptor.di == DI_BI);
  assert(rule_field_descriptor.mo == MO_IGNORE);
  assert(rule_field_descriptor.cda == CDA_COMPUTE);
  assert(rule_field_descriptor.msb_len == 0);
  assert(rule_field_descriptor.card_target_value == 0);
  assert(rule_field_descriptor.first_target_value_offset == 0);

  /**
   * @brief Get Rule Field Descriptor 15
   *
   * @details As there are only 12 Rule Field Descriptors, getting the 16th one
   * is not possible.
   */
  assert(!get_rule_field_descriptor(&rule_field_descriptor, 15,
                                    rule_descriptor_offset, context,
                                    context_byte_len));
}

/* ********************************************************************** */

int main(void) {
  test_unpacking_di_mo_cda();
  test_get_rule_field_descriptor();

  printf("All tests passed!\n");

  return 0;
}
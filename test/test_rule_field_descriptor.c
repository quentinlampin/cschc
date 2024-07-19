#include "core/rule_field_descriptor.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ********************************************************************** */

void test_unpacking_dir_mo_cda(void) {
  direction_t                        dir;
  matching_operator_t                mo;
  compression_decompression_action_t cda;
  uint8_t                            packed_value;

  packed_value = 0b10000000;
  unpack_dir_mo_cda(&dir, &mo, &cda, packed_value);
  assert(dir == DIR_UP && mo == MO_EQUAL && cda == CDA_NOT_SENT);

  packed_value = 0b01001100;
  unpack_dir_mo_cda(&dir, &mo, &cda, packed_value);
  assert(dir == DIR_BI && mo == MO_IGNORE && cda == CDA_COMPUTE);

  packed_value = 0b00111000;
  unpack_dir_mo_cda(&dir, &mo, &cda, packed_value);
  assert(dir == DIR_DW && mo == MO_MATCH_MAPPING && cda == CDA_NOT_SENT);
}

/* ********************************************************************** */

void test_get_rule_field_desc_info(void) {
  const uint8_t ctx_example[] = {
      // Context
      0, 2, 0, 6, 0, 33,  // 2 Rules

      // RuleDesc
      0, 0, 12, 0, 36, 0, 46, 0, 58, 0, 70, 0, 78, 0, 88, 0, 98, 0, 108, 0, 118,
      0, 128, 0, 138, 0, 146,  // First rule for compression
      1, 1, 0,                 // Second rule for no-compression

      // RuleFieldDesc
      0x13, 0xcc, 0, 4, 0, 1, 64, 1, 0, 154,  // fid-ipv6-version ; bi/eq/ns
      0x13, 0xc9, 0, 8, 0, 1, 81, 0, 4, 1, 0,
      155,  // fid-ipv6-trafficclass ; bi/msb(4)/lsb
      0x13, 0xc5, 0, 20, 0, 1, 90, 2, 0, 155, 0,
      158,                             // fid-ipv6-flowlabel ; bi/map/map
      0x13, 0xc8, 0, 16, 0, 1, 76, 0,  // fid-ipv6-payload-length ; bi/ig/co
      0x13, 0xc7, 0, 8, 0, 1, 64, 1, 0, 161,  // fid-ipv6-nextheader ; bi/eq/ns
      0x13, 0xc6, 0, 8, 0, 1, 64, 1, 0, 162,  // fid-ipv6-hoplimit ; bi/eq/ns
      0x13, 0xc1, 0, 128, 0, 1, 64, 1, 0,
      163,  // fid-ipv6-sourceaddress ; bi/eq/ns
      0x13, 0xc4, 0, 128, 0, 1, 64, 1, 0,
      179,  // fid-ipv6-destinationaddress ; bi/eq/ns

      0x13, 0xce, 0, 16, 0, 1, 64, 1, 0, 195,  // fid-udp-app-port ; bi/eq/ns
      0x13, 0xd1, 0, 16, 0, 1, 64, 1, 0, 197,  // fid-udp-dev-port ; bi/eq/ns
      0x13, 0xd2, 0, 16, 0, 1, 76, 0,          // fid-udp-length ; bi/ig/co
      0x13, 0xd0, 0, 16, 0, 1, 76, 0,          // fid-udp-checksum ; bi/ig/co

      // TargetValue
      0x06,              // fid-ipv6-version
      0x0f,              // fid-ipv6-trafficclass
      0x58, 0x0f,        // fid-ipv6-flowlabel(begin at fid-ipv6-trafficclass)
      0x09, 0xaa, 0xf2,  // fid-ipv6-flowlabel
      0x11,              // fid-ipv6-nextheader
      0x40,              // fid-ipv6-hoplimit
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x03,  // fid-ipv6-sourceaddress
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20,  // fid-ipv6-destinationaddress
      0x90, 0xa0,              // fid-udp-app-port
      0x16, 0x33,              // fid-udp-dev-port
  };

  const size_t ctx_len = sizeof(ctx_example);

  rule_field_desc_t rule_field_desc;
  int               offset;

  offset = get_rule_field_desc_info(&rule_field_desc, 36, ctx_example, ctx_len);
  assert(offset == 43);
  assert(rule_field_desc.fid == 0x13cc);
  assert(rule_field_desc.len == 4);
  assert(rule_field_desc.pos == 1);
  assert(rule_field_desc.dir == DIR_BI);
  assert(rule_field_desc.mo == MO_EQUAL);
  assert(rule_field_desc.cda == CDA_NOT_SENT);
  assert(rule_field_desc.msb_len ==
         0);  // useless cause never use in this case but check if properly set
  // to 0 anyway
  assert(rule_field_desc.card_target_value == 1);

  offset = get_rule_field_desc_info(&rule_field_desc, 46, ctx_example, ctx_len);
  assert(offset == 55);
  assert(rule_field_desc.fid == 0x13c9);
  assert(rule_field_desc.len == 8);
  assert(rule_field_desc.pos == 1);
  assert(rule_field_desc.dir == DIR_BI);
  assert(rule_field_desc.mo == MO_MSB);
  assert(rule_field_desc.cda == CDA_LSB);
  assert(rule_field_desc.msb_len == 4);
  assert(rule_field_desc.card_target_value == 1);

  offset = get_rule_field_desc_info(&rule_field_desc, 70, ctx_example, ctx_len);
  assert(offset == 77);
  assert(rule_field_desc.fid == 0x13c8);
  assert(rule_field_desc.len == 16);
  assert(rule_field_desc.pos == 1);
  assert(rule_field_desc.dir == DIR_BI);
  assert(rule_field_desc.mo == MO_IGNORE);
  assert(rule_field_desc.cda == CDA_COMPUTE);
  assert(rule_field_desc.msb_len == 0);
  assert(rule_field_desc.card_target_value == 0);
}

/* ********************************************************************** */

int main(void) {
  test_unpacking_dir_mo_cda();
  test_get_rule_field_desc_info();

  printf("All tests passed!\n");

  return 0;
}
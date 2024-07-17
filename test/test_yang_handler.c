#include "parser/yang_handler.h"

#include <assert.h>
#include <stdio.h>

/* ********************************************************************** */

void test_get_fid_by_protocol_and_offset(void) {
  assert(get_fid_by_protocol_and_offset(PROTOCOL_IPV6, 0) ==
         5068);  // IPv6 Version

  assert(get_fid_by_protocol_and_offset(PROTOCOL_IPV6, 2) ==
         5061);  // IPv6 Flow Label

  assert(get_fid_by_protocol_and_offset(PROTOCOL_IPV6, 8) ==
         5062);  // IPv6 Hop Limit

  assert(get_fid_by_protocol_and_offset(PROTOCOL_UDP, 9) == 0);  // Nothing
}

/* ********************************************************************** */

void test_get_protocol_and_offset_by_fid(void) {
  protocol_t proto;
  int        offset;

  assert(get_protocol_and_offset_by_fid(&proto, &offset,
                                        0));  // Not in the mapping list

  assert(
      get_protocol_and_offset_by_fid(&proto, &offset, 5073));  // UDP Dev Port
  assert(proto == PROTOCOL_UDP);
  assert(offset == 2);

  assert(
      get_protocol_and_offset_by_fid(&proto, &offset, 5062));  // IPv6 Hop Limit
  assert(proto == PROTOCOL_IPV6);
  assert(offset == 8);
}

/* ********************************************************************** */

int main(void) {
  test_get_fid_by_protocol_and_offset();
  test_get_protocol_and_offset_by_fid();

  printf("All tests passed!\n");

  return 0;
}
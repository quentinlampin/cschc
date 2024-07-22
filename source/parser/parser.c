#include "parser/parser.h"

/* ********************************************************************** */

int parse_header(header_t* header, const stack_type_t stack,
                 const uint8_t* packet, const size_t packet_len) {
  int parsing_offset;
  int parsing_status;

  switch (stack) {
    case STACK_IPV6_UDP:
      parsing_offset =
          parse_ipv6_header(&header->ipv6_udp.ipv6_hdr, packet, packet_len);

      // Check if UDP is the next header
      if (header->ipv6_udp.ipv6_hdr.next_header[0] == 0x11) {
        parsing_offset = parse_udp_header(&header->ipv6_udp.udp_hdr, packet,
                                          parsing_offset, packet_len);
        parsing_status = 1;
      } else {
        parsing_status = 0;
      }
      break;

    default:
      parsing_status = 0;
      break;
  }

  return parsing_status;
}
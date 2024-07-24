#include "parser/protocol/udp.h"

#include <string.h>

/* ********************************************************************** */

int parse_udp_header(udp_hdr_t* udp_hdr, int* offset, const uint8_t* packet,
                     const size_t packet_byte_len) {
  if (packet_byte_len < *offset + UDP_HDR_BYTE_LENGTH) {
    return 0;
  }

  // Extract all fields
  memcpy(udp_hdr, packet + *offset, UDP_HDR_BYTE_LENGTH);
  *offset += UDP_HDR_BYTE_LENGTH;

  return 1;
}
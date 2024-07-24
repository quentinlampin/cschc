#include "parser/protocol/ipv6.h"

#include <string.h>

/* ********************************************************************** */

int parse_ipv6_header(ipv6_hdr_t* ipv6_hdr, int* offset, const uint8_t* packet,
                      const size_t packet_byte_len) {
  if (packet_byte_len < IPV6_HDR_BYTE_LENGTH || *offset != 0) {
    return 0;
  }

  // Extract version (4 bits), traffic class (8 bits), and flow label (20 bits)
  ipv6_hdr->version[0] = (packet[0] >> 4) & 0x0F;
  ipv6_hdr->traffic_class[0] =
      ((packet[0] & 0x0F) << 4) | ((packet[1] >> 4) & 0x0F);
  ipv6_hdr->flow_label[0] = packet[1] & 0x0F;
  ipv6_hdr->flow_label[1] = packet[2];
  ipv6_hdr->flow_label[2] = packet[3];

  // Extract other fields
  memcpy(ipv6_hdr->payload_length, packet + 4, IPV6_HDR_BYTE_LENGTH - 4);
  *offset += IPV6_HDR_BYTE_LENGTH;

  return 1;
}
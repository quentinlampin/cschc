#include "parser/protocol/ipv6.h"

#include <string.h>

/* ********************************************************************** */

int parse_ipv6_header(ipv6_hdr_t* ipv6_hdr, const uint8_t* packet,
                      const size_t packet_len) {
  if (packet_len < IPV6_HDR_BYTE_LENGTH) {
    return -1;
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

  return IPV6_HDR_BYTE_LENGTH;
}

/* ********************************************************************** */

uint8_t* get_ipv6_field(ipv6_hdr_t* ipv6_hdr, const uint16_t fid) {
  switch (fid) {
    case FID_IPV6_TRAFFIC_CLASS:
      return ipv6_hdr->traffic_class;

    case FID_IPV6_FLOW_LABEL:
      return ipv6_hdr->flow_label;

    case FID_IPV6_PAYLOAD_LENGTH:
      return ipv6_hdr->payload_length;

    case FID_IPV6_NEXT_HEADER:
      return ipv6_hdr->next_header;

    case FID_IPV6_HOP_LIMIT:
      return ipv6_hdr->hop_limit;

    case FID_IPV6_SRC_ADDRESS:
      return ipv6_hdr->source_address;

    case FID_IPV6_DST_ADDRESS:
      return ipv6_hdr->destination_address;

    default:  // FID_IPV6_VERSION
      return ipv6_hdr->version;
  }
}
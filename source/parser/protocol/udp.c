#include "parser/protocol/udp.h"

#include <string.h>

/* ********************************************************************** */

int parse_udp_header(udp_hdr_t* udp_hdr, const uint8_t* packet, int offset,
                     const size_t packet_len) {
  if (packet_len < offset + UDP_HDR_BYTE_LENGTH) {
    return -1;
  }

  // Extract all fields
  memcpy(udp_hdr, packet + offset, UDP_HDR_BYTE_LENGTH);

  return offset + UDP_HDR_BYTE_LENGTH;
}

/* ********************************************************************** */

uint8_t* get_udp_field(udp_hdr_t* udp_hdr, const uint16_t fid) {
  switch (fid) {
    case FID_UDP_DEV_PORT:
      return udp_hdr->dev_port;

    case FID_UDP_LENGTH:
      return udp_hdr->length;

    case FID_UDP_CHECKSUM:
      return udp_hdr->checksum;

    default:  // FID_UDP_APP_PORT
      return udp_hdr->app_port;
  }
}
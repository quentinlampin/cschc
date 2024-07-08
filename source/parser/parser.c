#include "parser/parser.h"

#include <string.h>

/* ********************************************************************** */

uint16_t network_to_host_short(uint16_t netshort) {
  return (netshort << 8) | (netshort >> 8);
}

/* ********************************************************************** */

uint32_t network_to_host_long(uint32_t netlong) {
  return ((netlong << 24) & 0xff000000) | ((netlong << 8) & 0x00ff0000) |
         ((netlong >> 8) & 0x0000ff00) | ((netlong >> 24) & 0x000000ff);
}

/* ********************************************************************** */

int parse_ipv6_header(ipv6_hdr_t* ipv6_hdr, const uint8_t* packet,
                      const size_t packet_len) {
  size_t ipv6_hdr_len;

  ipv6_hdr_len = sizeof(ipv6_hdr_t);
  if (packet_len < ipv6_hdr_len) {
    return -1;
  }

  memcpy(ipv6_hdr, packet, ipv6_hdr_len);

  return ipv6_hdr_len;
}

/* ********************************************************************** */

int parse_udp_header(udp_hdr_t* udp_hdr, const uint8_t* packet, int offset,
                     const size_t packet_len) {
  size_t udp_hdr_len;

  udp_hdr_len = sizeof(udp_hdr_t);
  if (packet_len < offset + udp_hdr_len) {
    return -1;
  }

  memcpy(udp_hdr, packet + offset, udp_hdr_len);

  return offset + udp_hdr_len;
}
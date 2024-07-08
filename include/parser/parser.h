#ifndef _PARSER_H_
#define _PARSER_H_

#include "parser/protocol/ipv6.h"
#include "parser/protocol/udp.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Converts properly a 16-bit value from network byte order (big-endian)
 * into host byte order (little-endian).
 *
 * @param netshort The 16-bit value in network byte order.
 * @return The 16-bit value in host byte order.
 */
uint16_t network_to_host_short(uint16_t netshort);

/**
 * @brief Converts properly a 32-bit value from network byte order (big-endian)
 * into host byte order (little-endian).
 *
 * @param netlong The 32-bit value in network byte order.
 * @return The 32-bit value in host byte order.
 */
uint32_t network_to_host_long(uint32_t netlong);

/**
 * @brief Parses a network packet as an IPv6 header.
 *
 * @param ipv6_hdr Pointer to the IPv6 header structure to fill.
 * @param packet The packet data.
 * @param packet_len The packet data length.
 * @return The offset of the next header in the packet, or -1 if an error
 * occurs.
 *
 * @details This function consideres the IPv6 header from the beginning of the
 * packet, i.e offset is 0.
 */
int parse_ipv6_header(ipv6_hdr_t* ipv6_hdr, const uint8_t* packet,
                      const size_t packet_len);

/**
 * @brief Parses a network packet as a UDP header.
 *
 * @param udp_hdr Pointer to the UDP header structure to fill.
 * @param packet The packet data.
 * @param offset The offset to the start of the UDP header in the packet.
 * @param packet_len The length of the packet data.
 * @return The offset of the next header in the packet, or -1 if an error
 * occurs.
 */
int parse_udp_header(udp_hdr_t* udp_hdr, const uint8_t* packet, int offset,
                     const size_t packet_len);

#endif  // _PARSER_H_
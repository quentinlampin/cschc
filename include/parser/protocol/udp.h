#ifndef _UDP_H_
#define _UDP_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief UDP IETF SCHC Yang model identifiers.
 */
#define UDP_PREFIX "fid-udp"
#define UDP_APP_PORT UDP_PREFIX "-app-port"
#define UDP_DEV_PORT UDP_PREFIX "-dev-port"
#define UDP_LENGTH UDP_PREFIX "-length"
#define UDP_CHECKSUM UDP_PREFIX "-checksum"

/**
 * @brief UDP byte lengths.
 */
#define UDP_APP_PORT_BYTE_LENGTH 2
#define UDP_DEV_PORT_BYTE_LENGTH 2
#define UDP_LENGTH_BYTE_LENGTH 2
#define UDP_CHECKSUM_BYTE_LENGTH 2
#define UDP_HDR_BYTE_LENGTH 8

/**
 * @brief UDP IETF SCHC Yang model FIDs.
 */
#define FID_UDP_APP_PORT 5070
#define FID_UDP_DEV_PORT 5073
#define FID_UDP_LENGTH 5074
#define FID_UDP_CHECKSUM 5072

/**
 * @brief UDP structure that stores UDP header fields.
 */
typedef struct {
  uint8_t app_port[UDP_APP_PORT_BYTE_LENGTH];
  uint8_t dev_port[UDP_DEV_PORT_BYTE_LENGTH];
  uint8_t length[UDP_LENGTH_BYTE_LENGTH];
  uint8_t checksum[UDP_CHECKSUM_BYTE_LENGTH];
} udp_hdr_t;

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

uint8_t* get_udp_field(udp_hdr_t* udp_hdr, const uint16_t fid);

#endif  // _UDP_H_

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
 * @param offset Pointer to the offset to the start of the UDP header in the
 * packet.
 * @param packet Pointer to the packet data.
 * @param packet_byte_len Byte length of the packet data.
 * @return The status code, 1 for success otherwise 0.
 */
int parse_udp_header(udp_hdr_t* udp_hdr, int* offset, const uint8_t* packet,
                     const size_t packet_byte_len);

#endif  // _UDP_H_

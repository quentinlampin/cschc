/**
 * @file udp.h
 * @author Corentin Banier and Quentin Lampin
 * @brief UDP implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

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
 * @brief UDP IETF SCHC Yang model SIDs.
 *
 * @details Not standardized yet.
 */
#define SID_UDP_APP_PORT 5070
#define SID_UDP_DEV_PORT 5073
#define SID_UDP_LENGTH 5074
#define SID_UDP_CHECKSUM 5072

/**
 * @brief Determines the UDP Checksum Value.
 *
 * @param checksum Pointer that store the computed checksum.
 * @param checksum_byte_len Byte length of the checksum.
 * @param packet Pointer to the packet data.
 * @param packet_byte_len Byte length of the packet.
 * @param is_ipv6 Flag indicating whether the packet is IPv6 (1) or IPv4 (0).
 *
 * @details This function calculates the UDP checksum for the given packet.
 * The checksum is computed according to the UDP specification, taking into
 * account the Pseudo-Header for only IPv6 yet. The result is stored in
 * the provided checksum buffer.
 *
 * @details To do : IPv4 case.
 */
void udp_checksum(uint8_t* checksum, const size_t checksum_byte_len,
                  const uint8_t* packet, const size_t packet_byte_len,
                  int is_ipv6);

#endif  // _UDP_H_

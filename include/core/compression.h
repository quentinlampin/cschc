/**
 * @file compression.h
 * @author Corentin Banier
 * @brief SCHC Compression implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @details See Section 7 SCHC RFC 8724.
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Compress a Packet using a SCHC Context.
 *
 * @param schc_packet Pointer to the SCHC Packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the schc_packet.
 * @param packet_direction Packet Direction Indicator.
 * @param packet Pointer to the packet that needs to be compressed.
 * @param packet_byte_len Byte length of the packet to compress.
 * @param context Pointer to the SCHC Context used to perform compression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the compressed SCHC packet.
 */
size_t compress(uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
                const direction_indicator_t packet_direction,
                const uint8_t* packet, const size_t packet_byte_len,
                const uint8_t* context, const size_t context_byte_len);

#endif  // _COMPRESSION_H_
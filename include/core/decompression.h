#ifndef _DECOMPRESSION_H_
#define _DECOMPRESSION_H_

#include "schc8724.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Decompress a SCHC Packet using a SCHC Context.
 *
 * @param packet Pointer to the Packet to fill.
 * @param packet_max_byte_len Maximum byte length of the packet.
 * @param packet_direction Packet Direction Indicator.
 * @param schc_packet Pointer to the SCHC Packet that needs to be decompressed.
 * @param schc_packet_byte_len Byte length of the schc_packet to decompress.
 * @param context Pointer to the SCHC Context used to perform decompression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the decompressed SCHC packet.
 */
size_t decompress(uint8_t *packet, const size_t packet_max_byte_len,
                  const direction_indicator_t packet_direction,
                  const uint8_t *schc_packet, const size_t schc_packet_byte_len,
                  const uint8_t *context, const size_t context_byte_len);

#endif  // _DECOMPRESSION_H_
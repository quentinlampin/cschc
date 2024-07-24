#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include "core/context.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Compress a packet using the specified rule and context.
 *
 * @param schc_packet Pointer to the SCHC packet to fill.
 * @param schc_packet_max_byte_len Maximum byte length of the SCHC packet.
 * @param packet Pointer to the packet to be compressed.
 * @param packet_byte_len Byte length of the packet to compress.
 * @param rule_desc Pointer to the Rule Descriptor used for compression.
 * @param context Pointer to the context used for compression.
 * @param context_byte_len Byte length of the context.
 * @return The final byte length of the compressed SCHC packet.
 */
size_t compress(uint8_t* schc_packet, const size_t schc_packet_max_byte_len,
                const uint8_t* packet, const size_t packet_byte_len,
                const rule_desc_t* rule_desc, const uint8_t* context,
                const size_t context_byte_len);

#endif  // _COMPRESSION_H_
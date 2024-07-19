#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include "core/context.h"

#include <stddef.h>
#include <stdint.h>

int compress(uint8_t* schc_packet, const size_t schc_packet_byte_len,
             const rule_desc_t rule_desc, const uint8_t* packet,
             const size_t packet_len, const uint8_t* context,
             const size_t context_len);

#endif  // _COMPRESSION_H_
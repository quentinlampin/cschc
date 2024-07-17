#ifndef _YANG_HANDLER_H_
#define _YANG_HANDLER_H_

#include "parser/parser.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Structure which map protocol fields to their FIDs.
 */
typedef struct {
  protocol_t protocol;
  uint8_t    byte_length;
  uint16_t   fid;
} field_fid_mapping_t;

/**
 * @brief Static array of field-to-FID mappings.
 */
static const field_fid_mapping_t field_fid_mappings[] = {
  // IPv6 mappings
    {PROTOCOL_IPV6, IPV6_VERSION_BYTE_LENGTH,        FID_IPV6_VERSION       },
    {PROTOCOL_IPV6, IPV6_TRAFFIC_CLASS_BYTE_LENGTH,  FID_IPV6_TRAFFIC_CLASS },
    {PROTOCOL_IPV6, IPV6_FLOW_LABEL_BYTE_LENGTH,     FID_IPV6_FLOW_LABEL    },
    {PROTOCOL_IPV6, IPV6_PAYLOAD_LENGTH_BYTE_LENGTH, FID_IPV6_PAYLOAD_LENGTH},
    {PROTOCOL_IPV6, IPV6_NEXT_HEADER_BYTE_LENGTH,    FID_IPV6_NEXT_HEADER   },
    {PROTOCOL_IPV6, IPV6_HOP_LIMIT_BYTE_LENGTH,      FID_IPV6_HOP_LIMIT     },
    {PROTOCOL_IPV6, IPV6_SRC_ADDRESS_BYTE_LENGTH,    FID_IPV6_SRC_ADDRESS   },
    {PROTOCOL_IPV6, IPV6_DST_ADDRESS_BYTE_LENGTH,    FID_IPV6_DST_ADDRESS   },

 // UDP mappings
    {PROTOCOL_UDP,  UDP_APP_PORT_BYTE_LENGTH,        FID_UDP_APP_PORT       },
    {PROTOCOL_UDP,  UDP_DEV_PORT_BYTE_LENGTH,        FID_UDP_DEV_PORT       },
    {PROTOCOL_UDP,  UDP_LENGTH_BYTE_LENGTH,          FID_UDP_LENGTH         },
    {PROTOCOL_UDP,  UDP_CHECKSUM_BYTE_LENGTH,        FID_UDP_CHECKSUM       },
};

#endif  // _YANG_HANDLER_H_
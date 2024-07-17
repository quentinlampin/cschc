#ifndef _YANG_HANDLER_H_
#define _YANG_HANDLER_H_

#include "parser/parser.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Enumeration of supported protocols.
 */
typedef enum {
  PROTOCOL_IPV6,
  PROTOCOL_UDP,
} protocol_t;

/**
 * @brief Structure which map protocol fields to their FIDs.
 */
typedef struct {
  protocol_t protocol;
  int        offset;
  uint8_t    byte_length;
  uint16_t   fid;
} field_fid_mapping_t;

/**
 * @brief Static array of field-to-FID mappings.
 */
static const field_fid_mapping_t field_fid_mappings[] = {
  // IPv6 mappings
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, version),             IPV6_VERSION_BYTE_LENGTH,
     FID_IPV6_VERSION                                                                                             },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, traffic_class),
     IPV6_TRAFFIC_CLASS_BYTE_LENGTH,                                                       FID_IPV6_TRAFFIC_CLASS },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, flow_label),
     IPV6_FLOW_LABEL_BYTE_LENGTH,                                                          FID_IPV6_FLOW_LABEL    },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, payload_length),
     IPV6_PAYLOAD_LENGTH_BYTE_LENGTH,                                                      FID_IPV6_PAYLOAD_LENGTH},
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, next_header),
     IPV6_NEXT_HEADER_BYTE_LENGTH,                                                         FID_IPV6_NEXT_HEADER   },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, hop_limit),           IPV6_HOP_LIMIT_BYTE_LENGTH,
     FID_IPV6_HOP_LIMIT                                                                                           },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, source_address),
     IPV6_SRC_ADDRESS_BYTE_LENGTH,                                                         FID_IPV6_SRC_ADDRESS   },
    {PROTOCOL_IPV6, offsetof(ipv6_hdr_t, destination_address),
     IPV6_DST_ADDRESS_BYTE_LENGTH,                                                         FID_IPV6_DST_ADDRESS   },

 // UDP mappings
    {PROTOCOL_UDP,  offsetof(udp_hdr_t,  app_port),            UDP_APP_PORT_BYTE_LENGTH,
     FID_UDP_APP_PORT                                                                                             },
    {PROTOCOL_UDP,  offsetof(udp_hdr_t,  dev_port),            UDP_DEV_PORT_BYTE_LENGTH,
     FID_UDP_DEV_PORT                                                                                             },
    {PROTOCOL_UDP,  offsetof(udp_hdr_t,  length),              UDP_LENGTH_BYTE_LENGTH,
     FID_UDP_LENGTH                                                                                               },
    {PROTOCOL_UDP,  offsetof(udp_hdr_t,  checksum),            UDP_CHECKSUM_BYTE_LENGTH,
     FID_UDP_CHECKSUM                                                                                             },
};

/**
 * @brief Get the FID by protocol and offset.
 *
 * @param protocol The protocol type.
 * @param offset The offset of the field within the protocol header.
 * @return The FID of the field, or 0 if no matching FID is found.
 */
uint16_t get_fid_by_protocol_and_offset(const protocol_t protocol,
                                        const int        offset);

/**
 * @brief Get the protocol and offset by FID.
 *
 * @param fid The Field Identifier (FID).
 * @param protocol Pointer to store the protocol type.
 * @param offset Pointer to store the offset of the field within the protocol
 * header.
 * @return 1 if success, 0 if no matching FID is found.
 */
int get_protocol_and_offset_by_fid(protocol_t* protocol, int* offset,
                                   const uint16_t fid);

#endif  // _YANG_HANDLER_H_
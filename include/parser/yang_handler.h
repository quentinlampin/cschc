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
  uint16_t   fid;
} field_fid_mapping_t;

/**
 * @brief Static array of field-to-FID mappings.
 */
static const field_fid_mapping_t field_fid_mappings[] = {
  // IPv6 mappings
    {PROTOCOL_IPV6, 0 /*offsetof(ipv6_hdr_t, version)*/,             FID_IPV6_VERSION  },
    {PROTOCOL_IPV6, 1 /*offsetof(ipv6_hdr_t, traffic_class)*/,
     FID_IPV6_TRAFFIC_CLASS                                                            },
    {PROTOCOL_IPV6, 2 /*offsetof(ipv6_hdr_t, flow_label)*/,
     FID_IPV6_FLOW_LABEL                                                               },
    {PROTOCOL_IPV6, 3 /*offsetof(ipv6_hdr_t, payload_length)*/,
     FID_IPV6_PAYLOAD_LENGTH                                                           },
    {PROTOCOL_IPV6, 4 /*offsetof(ipv6_hdr_t, next_header)*/,
     FID_IPV6_NEXT_HEADER                                                              },
    {PROTOCOL_IPV6, 5 /*offsetof(ipv6_hdr_t, hop_limit)*/,           FID_IPV6_HOP_LIMIT},
    {PROTOCOL_IPV6, 6 /*offsetof(ipv6_hdr_t, source_address)*/,
     FID_IPV6_SRC_ADDRESS                                                              },
    {PROTOCOL_IPV6, 7 /*offsetof(ipv6_hdr_t, destination_address)*/,
     FID_IPV6_DST_ADDRESS                                                              },

 // UDP mappings
    {PROTOCOL_UDP,  0 /*offsetof(udp_hdr_t,  app_port)*/,            FID_UDP_APP_PORT  },
    {PROTOCOL_UDP,  1 /*offsetof(udp_hdr_t,  dev_port)*/,            FID_UDP_DEV_PORT  },
    {PROTOCOL_UDP,  2 /*offsetof(udp_hdr_t,  length)*/,              FID_UDP_LENGTH    },
    {PROTOCOL_UDP,  3 /*offsetof(udp_hdr_t,  checksum)*/,            FID_UDP_CHECKSUM  },
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
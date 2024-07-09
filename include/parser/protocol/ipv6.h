#ifndef _IPV6_H_
#define _IPV6_H_

#include <stdint.h>

#define IPV6_FID "fid-ipv6"

#define IPV6_VERSION IPV6_FID "-version"
#define IPV6_TRAFFIC_CLASS IPV6_FID "-trafficclass"
#define IPV6_FLOW_LABEL IPV6_FID "-flowlabel"
#define IPV6_PAYLOAD_LENGTH IPV6_FID "-payload-length"
#define IPV6_NEXT_HEADER IPV6_FID "-nextheader"
#define IPV6_HOP_LIMIT IPV6_FID "-hoplimit"
// prefix and iid in yang model ?
#define IPV6_SRC_ADDRESS IPV6_FID "-sourceaddress"
#define IPV6_DST_ADDRESS IPV6_FID "-destinationaddress"

#define IPV6_VERSION_BYTE_LENGTH 1
#define IPV6_TRAFFIC_CLASS_BYTE_LENGTH 1
#define IPV6_FLOW_LABEL_BYTE_LENGTH 3
#define IPV6_PAYLOAD_LENGTH_BYTE_LENGTH 2
#define IPV6_NEXT_HEADER_BYTE_LENGTH 1
#define IPV6_HOP_LIMIT_BYTE_LENGTH 1
// prefix and iid in yang model ?
#define IPV6_SRC_ADDRESS_BYTE_LENGTH 16
#define IPV6_DST_ADDRESS_BYTE_LENGTH 16

#define IPV6_HDR_BYTE_LENGTH 40

/**
 * @brief IPv6 structure.
 *
 * @details This representation increases the length of the structure. Indeed,
 * the following struct is 41 bytes in length whereas an IPv6 header should be
 * 40 bytes. We use uint8_t in this way to simplify the code to perform the
 * (de)compression more easily.
 */
typedef struct {
  uint8_t version[IPV6_VERSION_BYTE_LENGTH];
  uint8_t traffic_class[IPV6_TRAFFIC_CLASS_BYTE_LENGTH];
  uint8_t flow_label[IPV6_FLOW_LABEL_BYTE_LENGTH];
  uint8_t payload_length[IPV6_PAYLOAD_LENGTH_BYTE_LENGTH];
  uint8_t next_header[IPV6_NEXT_HEADER_BYTE_LENGTH];
  uint8_t hop_limit[IPV6_HOP_LIMIT_BYTE_LENGTH];
  uint8_t source_address[IPV6_SRC_ADDRESS_BYTE_LENGTH];
  uint8_t destination_address[IPV6_DST_ADDRESS_BYTE_LENGTH];
} ipv6_hdr_t;

#endif  // _IPV6_H_
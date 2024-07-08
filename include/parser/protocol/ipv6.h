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

#define IPV6_VERSION_LENGTH 4
#define IPV6_TRAFFIC_CLASS_LENGTH 8
#define IPV6_FLOW_LABEL_LENGTH 20
#define IPV6_PAYLOAD_LENGTH_LENGTH 16
#define IPV6_NEXT_HEADER_LENGTH 8
#define IPV6_HOP_LIMIT_LENGTH 8
// prefix and iid in yang model ?
#define IPV6_SRC_ADDRESS_LENGTH 128
#define IPV6_DST_ADDRESS_LENGTH 128

/**
 * @brief IPv6 structure.
 *
 * @details Version, Traffic Class and Flow Label are packed into a uint32_t due
 * to memory alignment. Indeed, define a uint8_t for Version, then uint8_t for
 * Traffic Class and uint32_t for Flow Label Packed because this struct would
 * have generated more memory space, 44 bytes instead of 40 in the proposed
 * struct below.
 */
typedef struct {
  uint32_t ver_tc_fl;
  uint16_t payload_length;
  uint8_t  next_header;
  uint8_t  hop_limit;
  uint8_t  source_address[16];
  uint8_t  destination_address[16];
} ipv6_hdr_t;

/**
 * @brief Unpacks the version, traffic class, and flow label from a 32-bit IPv6
 * header field.
 *
 * @param version Pointer to store the unpacked version.
 * @param traffic_class Pointer to store the unpacked traffic class.
 * @param flow_label Pointer to store the unpacked flow label.
 * @param ver_tc_fl The 32-bit field containing the version, traffic class, and
 * flow label.
 */
void unpack_ver_tc_fl(uint8_t* version, uint8_t* traffic_class,
                      uint32_t* flow_label, const uint32_t ver_tc_fl);

#endif  // _IPV6_H_
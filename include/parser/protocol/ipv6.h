#ifndef _IPV6_H_
#define _IPV6_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief
 */
#define IPV6_PREFIX "fid-ipv6"
#define IPV6_VERSION IPV6_PREFIX "-version"
#define IPV6_TRAFFIC_CLASS IPV6_PREFIX "-trafficclass"
#define IPV6_FLOW_LABEL IPV6_PREFIX "-flowlabel"
#define IPV6_PAYLOAD_LENGTH IPV6_PREFIX "-payload-length"
#define IPV6_NEXT_HEADER IPV6_PREFIX "-nextheader"
#define IPV6_HOP_LIMIT IPV6_PREFIX "-hoplimit"
#define IPV6_SRC_ADDRESS IPV6_PREFIX "-sourceaddress"
#define IPV6_DST_ADDRESS IPV6_PREFIX "-destinationaddress"
// prefix and iid in yang model ?
// #define IPV6_APP_PREFIX IPV6_PREFIX "-appprefix"
// #define IPV6_APP_IID IPV6_PREFIX "-appiid"
// #define IPV6_DEV_PREFIX IPV6_PREFIX "-devprefix"
// #define IPV6_DEV_IID IPV6_PREFIX "-deviid"

/**
 * @brief IPv6 byte lengths.
 */
#define IPV6_VERSION_BYTE_LENGTH 1
#define IPV6_TRAFFIC_CLASS_BYTE_LENGTH 1
#define IPV6_FLOW_LABEL_BYTE_LENGTH 3
#define IPV6_PAYLOAD_LENGTH_BYTE_LENGTH 2
#define IPV6_NEXT_HEADER_BYTE_LENGTH 1
#define IPV6_HOP_LIMIT_BYTE_LENGTH 1
#define IPV6_SRC_ADDRESS_BYTE_LENGTH 16
#define IPV6_DST_ADDRESS_BYTE_LENGTH 16
// prefix and iid in yang model ?
// #define IPV6_APP_PREFIX_BYTE_LENGTH 8
// #define IPV6_APP_IID_BYTE_LENGTH 8
// #define IPV6_DEV_PREFIX_BYTE_LENGTH 8
// #define IPV6_DEV_IID_BYTE_LENGTH 8
#define IPV6_HDR_BYTE_LENGTH 40

/**
 * @brief IPv6 IETF SCHC Yang model FIDs.
 */
#define FID_IPV6_VERSION 5068
#define FID_IPV6_TRAFFIC_CLASS 5065
#define FID_IPV6_FLOW_LABEL 5061
#define FID_IPV6_PAYLOAD_LENGTH 5064
#define FID_IPV6_NEXT_HEADER 5063
#define FID_IPV6_HOP_LIMIT 5062
#define FID_IPV6_SRC_ADDRESS 5057
#define FID_IPV6_DST_ADDRESS 5060
// prefix and iid in yang model ?
// #define FID_IPV6_APP_PREFIX 5057
// #define FID_IPV6_APP_IID 5056
// #define FID_IPV6_DEV_PREFIX 5060
// #define FID_IPV6_DEV_IID 5059

/**
 * @brief IPv6 structure that stores IPv6 header fields.
 *
 * @details This representation increases the length of the structure.
 * Indeed, the following struct is 41 bytes in length whereas an IPv6 header
 * should be 40 bytes. We use uint8_t in this way to simplify the code to
 * perform the (de)compression more easily.
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

/**
 * @brief Parses a network packet as an IPv6 header.
 *
 * @param ipv6_hdr Pointer to the IPv6 header structure to fill.
 * @param offset Pointer to the offset to the start of the IPv6 header in the
 * packet.
 * @param packet Pointer to the packet data.
 * @param packet_byte_len Byte length of the packet data.
 * @return The status code, 1 for success otherwise 0.
 */
int parse_ipv6_header(ipv6_hdr_t* ipv6_hdr, int* offset, const uint8_t* packet,
                      const size_t packet_byte_len);

#endif  // _IPV6_H_
#ifndef _IPV6_H_
#define _IPV6_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief IPv6 IETF SCHC Yang model identifiers.
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
// Prefix and IID in Yang model?
// #define IPV6_APP_PREFIX IPV6_PREFIX "-appprefix"
// #define IPV6_APP_IID IPV6_PREFIX "-appiid"
// #define IPV6_DEV_PREFIX IPV6_PREFIX "-devprefix"
// #define IPV6_DEV_IID IPV6_PREFIX "-deviid"

/**
 * @brief IPv6 IETF SCHC Yang model SIDs.
 *
 * @details Not standardized yet.
 */
#define SID_IPV6_VERSION 5068
#define SID_IPV6_TRAFFIC_CLASS 5065
#define SID_IPV6_FLOW_LABEL 5061
#define SID_IPV6_PAYLOAD_LENGTH 5064
#define SID_IPV6_NEXT_HEADER 5063
#define SID_IPV6_HOP_LIMIT 5062
#define SID_IPV6_SRC_ADDRESS 5057
#define SID_IPV6_DST_ADDRESS 5060
// Prefix and IID in Yang model?
// #define SID_IPV6_APP_PREFIX 5057
// #define SID_IPV6_APP_IID 5056
// #define SID_IPV6_DEV_PREFIX 5060
// #define SID_IPV6_DEV_IID 5059

#endif  // _IPV6_H_
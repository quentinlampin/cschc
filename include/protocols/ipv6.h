/**
 * @file ipv6.h
 * @author Corentin Banier
 * @brief IPv6 implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

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

// Defined for this library (Not in current SCHC Yang Model)
#define IPV6_SRC_ADDRESS IPV6_PREFIX "-source-address"
#define IPV6_DST_ADDRESS IPV6_PREFIX "-destination-address"

// Prefix and IID defined in SCHC Yang model but not used in this implementation
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

// Defined for this library (Not in current SCHC Yang Model)
#define SID_IPV6_SRC_ADDRESS 5057
#define SID_IPV6_DST_ADDRESS 5060

// Prefix and IID defined in SCHC Yang model but not used in this implementation
// #define SID_IPV6_APP_PREFIX 5057
// #define SID_IPV6_APP_IID 5056
// #define SID_IPV6_DEV_PREFIX 5060
// #define SID_IPV6_DEV_IID 5059

#endif  // _IPV6_H_
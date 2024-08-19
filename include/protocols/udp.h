#ifndef _UDP_H_
#define _UDP_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief UDP IETF SCHC Yang model identifiers.
 */
#define UDP_PREFIX "fid-udp"
#define UDP_APP_PORT UDP_PREFIX "-app-port"
#define UDP_DEV_PORT UDP_PREFIX "-dev-port"
#define UDP_LENGTH UDP_PREFIX "-length"
#define UDP_CHECKSUM UDP_PREFIX "-checksum"

/**
 * @brief UDP IETF SCHC Yang model SIDs.
 *
 * @details Not standardized yet.
 */
#define SID_UDP_APP_PORT 5070
#define SID_UDP_DEV_PORT 5073
#define SID_UDP_LENGTH 5074
#define SID_UDP_CHECKSUM 5072

#endif  // _UDP_H_

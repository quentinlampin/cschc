#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>

#define UDP_FID "fid-udp"

#define UDP_APP_PORT UDP_FID "-app-port"
#define UDP_DEV_PORT UDP_FID "-dev-port"
#define UDP_LENGTH UDP_FID "-length"
#define UDP_CHECKSUM UDP_FID "-checksum"

#define UDP_APP_PORT_BYTE_LENGTH 2
#define UDP_DEV_PORT_BYTE_LENGTH 2
#define UDP_LENGTH_BYTE_LENGTH 2
#define UDP_CHECKSUM_BYTE_LENGTH 2

#define UDP_HDR_BYTE_LENGTH 8

/**
 * @brief UDP structure.
 */
typedef struct {
  uint8_t app_port[UDP_APP_PORT_BYTE_LENGTH];
  uint8_t dev_port[UDP_DEV_PORT_BYTE_LENGTH];
  uint8_t length[UDP_LENGTH_BYTE_LENGTH];
  uint8_t checksum[UDP_CHECKSUM_BYTE_LENGTH];
} udp_hdr_t;

#endif  // _UDP_H_
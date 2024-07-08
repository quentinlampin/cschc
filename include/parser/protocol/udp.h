#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>

#define UDP_FID "fid-udp"

#define UDP_APP_PORT UDP_FID "-app-port"
#define UDP_DEV_PORT UDP_FID "-dev-port"
#define UDP_LENGTH UDP_FID "-length"
#define UDP_CHECKSUM UDP_FID "-checksum"

/**
 * @brief UDP structure.
 */
typedef struct {
  uint16_t source_port;
  uint16_t destination_port;
  uint16_t length;
  uint16_t checksum;
} udp_hdr_t;

#endif  // _UDP_H_
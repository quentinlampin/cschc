#ifndef _PARSER_H_
#define _PARSER_H_

#include "parser/protocol/ipv6.h"
#include "parser/protocol/udp.h"

/**
 * @brief Enumeration of supported protocols.
 */
typedef enum {
  PROTOCOL_IPV6,
  PROTOCOL_UDP,
} protocol_t;

#endif  // _PARSER_H_
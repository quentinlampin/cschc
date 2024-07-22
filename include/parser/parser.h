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

/**
 * @brief Enumeration of supported stacks.
 */
typedef enum {
  STACK_IPV6_UDP,
} stack_type_t;

/**
 * @brief IPv6 + UDP header structure.
 */
typedef struct {
  ipv6_hdr_t ipv6_hdr;
  udp_hdr_t  udp_hdr;
} ipv6_udp_hdr_t;

/**
 * @brief Union of various header stacks supported.
 */
typedef union {
  ipv6_udp_hdr_t ipv6_udp;
} header_t;

/**
 * @brief Parses the header based on the specified stack type.
 *
 * @param header Pointer to the header structure to be parsed.
 * @param stack The type of stack to parse.
 * @param packet Pointer to the packet data.
 * @param packet_len Length of the packet data.
 * @return The status code, 1 for success otherwise 0.
 */
int parse_header(header_t* header, const stack_type_t stack,
                 const uint8_t* packet, const size_t packet_len);

#endif  // _PARSER_H_
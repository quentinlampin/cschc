#ifndef _PARSER_H_
#define _PARSER_H_

#include "parser/protocol/ipv6.h"
#include "parser/protocol/udp.h"

/**
 * @brief IPv6 + UDP header structure.
 */
typedef struct {
  ipv6_hdr_t ipv6_hdr;
  udp_hdr_t  udp_hdr;
  // coap ; not implemented yet
} header_t;

/**
 * @brief Parses the header.
 *
 * @param header Pointer to the header structure to be parsed.
 * @param offset Pointer to the parsing offset.
 * @param packet Pointer to the packet data.
 * @param packet_byte_len Byte length of the packet data.
 * @return The status code, 1 for success otherwise 0.
 */
int parse_header(header_t* header, int* offset, const uint8_t* packet,
                 const size_t packet_byte_len);

/**
 * @brief Get the next header field pointer thanks to the previous FID.
 *
 * @param field_ptr Pointer to the field.
 * @param header Pointer to the header content.
 * @param fid Field ID.
 * @return The status code, 1 for success otherwise 0.
 */
int get_next_header_field(uint8_t** field_ptr, header_t* header,
                          const uint16_t fid);

/**
 * @brief Get the fid from a field pointer.
 *
 * @param fid The FID which corresponds to the field_ptr.
 * @param field_ptr Pointer to the field.
 * @param header Pointer to the header content.
 * @return The status code, 1 for success otherwise 0.
 */
int get_fid_from_field_ptr(uint16_t* fid, uint8_t* field_ptr, header_t* header);

#endif  // _PARSER_H_
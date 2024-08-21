#include "headers.h"
#include "utils/binary.h"
#include "utils/memory.h"

#include <string.h>

/* ********************************************************************** */
/*                          UDP Header functions                          */
/* ********************************************************************** */

void udp_checksum(uint8_t* checksum, const size_t checksum_byte_len,
                  const uint8_t* packet, const size_t packet_byte_len,
                  int is_ipv6) {
  size_t   ip_addresses_byte_len;
  size_t   protocol_id_byte_len;
  size_t   udp_length_byte_len;
  size_t   pseudo_header_byte_len;
  uint16_t checksum_value;
  uint16_t udp_length_value;
  uint64_t sum;

  if (is_ipv6) {
    /**
     * @brief The Pseudo-Header for an IPv6 + UDP Stack is composed of the
     * following elements:
     * - IPv6 Source Address
     * - IPv6 Destination Address
     * - Protocol ID (on 16 bits)
     * - UDP Length
     * - UDP Header
     * - UDP Body
     *
     * @remark The only variable part is the UDP Body. Indeed, we cannot predict
     * the length of this part. However, we can deduce it from the UDP Length
     * Value.
     *
     * @details If the UDP Body Length is odd, then one 0x00 byte is added in
     * the calculation of the Pseudo-Header.
     */

    ip_addresses_byte_len = 32;
    protocol_id_byte_len  = 2;
    udp_length_byte_len   = 2;
    checksum_value        = 0;
    sum                   = 0;

    udp_length_value = merge_uint8_t(
        packet[44],
        packet[45]);  // 44 and 45 are the byte positions which represent the
                      // UDP Length in an IPv6 + UDP protocol stack

    pseudo_header_byte_len = ip_addresses_byte_len + protocol_id_byte_len +
                             udp_length_byte_len + udp_length_value;

    // Protocol ID
    sum += 0x0011;

    // UDP Length
    sum += udp_length_value;

    // IPv6 Source Address + IPv6 Destination Address + UDP Header
    // + UDP Body(-1)
    for (int index = 0; index < ip_addresses_byte_len + udp_length_value - 2;
         index += 2) {
      sum += merge_uint8_t(packet[8 + index], packet[8 + index + 1]);
    }

    // Last 16-bit of UDP Body
    if (pseudo_header_byte_len % 2 == 0) {
      sum += merge_uint8_t(packet[packet_byte_len - 2],
                           packet[packet_byte_len - 1]);
    } else {
      sum += merge_uint8_t(packet[packet_byte_len - 1], 0x00);
    }

    // Shrink uint64_t format to uint16_t
    while (sum >> 16) {
      sum = (sum & 0xffff) + (sum >> 16);
    }

    // Determine the 16-bit checksum value
    checksum_value = ~sum;

    if (checksum_value == 0x0000) {
      checksum_value = 0xffff;
    }

    split_uint16_t(checksum, checksum + 1, checksum_value);
  } else {  // IPv4 Stack
    // Not Implemented yet
    memset(checksum, 0xff, checksum_byte_len);
  }
}

/* ********************************************************************** */
/*                         CoAP Header functions                          */
/* ********************************************************************** */

void set_coap_option_variable_length(uint16_t*      variable_length,
                                     const uint8_t* field,
                                     const size_t   field_byte_len,
                                     const uint16_t sid) {
  if (sid == SID_COAP_OPTION_DELTA || sid == SID_COAP_OPTION_LENGTH) {
    *variable_length = (uint16_t) *field;
  } else if (sid == SID_COAP_OPTION_DELTA_EXTENDED ||
             sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
    switch (*variable_length) {
      case 13:
        *variable_length = (uint16_t) field[0] + 13;
        break;

      case 14:
        *variable_length = ((uint16_t) ((field[0] << 8) | field[1])) + 269;
        break;

      default:  // less than 13 or 15. 15 is reserved.
        *variable_length = 0;
        break;
    }
  } else {
    *variable_length = 0;
  }
}

/* ********************************************************************** */

size_t get_coap_option_bit_length(const uint16_t option, const uint16_t sid) {
  if (sid == SID_COAP_OPTION_DELTA_EXTENDED ||
      sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
    switch (option) {
      case 13:
        return 8;

      case 14:
        return 16;

      default:  // less than 13 or 15. 15 is reserved.
        return 0;
    }
  } else if (sid == SID_COAP_OPTION_VALUE) {
    return 8 * option;
  } else {  // Might not reach this statement
    return 0;
  }
}
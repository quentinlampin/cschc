#include "headers.h"
#include "utils/binary.h"
#include "utils/memory.h"

#include <string.h>

/* ********************************************************************** */
/*                          UDP Header functions                          */
/* ********************************************************************** */

/**
 * @brief Calculates the Checksum for a specific range of data.
 *
 * @param checksum Pointer to the checksum value.
 * @param data Pointer to the data.
 * @param data_byte_length Byte length of data.
 */
static void __calculate_checksum(uint32_t* checksum, const uint8_t* data,
                                 size_t data_byte_length);

/* ********************************************************************** */

void udp_checksum(uint8_t* checksum, const size_t checksum_byte_len,
                  const uint8_t* packet, const size_t packet_byte_len,
                  int is_ipv6) {
  size_t   ip_addresses_byte_len;
  uint16_t carry;
  uint16_t udp_length_value;
  uint32_t pseudo_header_checksum;
  uint32_t udp_packet_checksum;
  uint64_t checksum_value;

  if (is_ipv6) {
    /**
     * @brief The Pseudo-Header for an IPv6 + UDP stack is composed of the
     * following elements:
     * - IPv6 Source Address
     * - IPv6 Destination Address
     * - Protocol ID (32 bits)
     * - UDP Length (32 bits)
     */

    // 1. Initialize Pseudo-Header Checksum
    pseudo_header_checksum = 0;

    // 2. Add IPv6 Addresses Checksum
    ip_addresses_byte_len = 32;
    __calculate_checksum(&pseudo_header_checksum, packet + 8,
                         ip_addresses_byte_len);

    // 3. Determine UDP Length from the packet
    udp_length_value = merge_uint8_t(
        packet[44],   // 44 and 45 are the byte positions which represent the
        packet[45]);  // UDP Length in an IPv6 + UDP protocol stack

    // 4. UDP Length + Protocol ID:
    // 4.1. Allocate udp_length_protocol_id from the pool
    uint8_t* udp_length_protocol_id =
        (uint8_t*) pool_alloc(sizeof(uint8_t) * 8);  // 4 bytes for each
    memset(udp_length_protocol_id, 0x00, 8);

    // 4.2. Fill UDP Length
    split_uint16_t(udp_length_protocol_id + 2, udp_length_protocol_id + 3,
                   udp_length_value);

    // 4.3. Fill Protocol ID
    udp_length_protocol_id[7] = 0x11;

    // 4.4. Determine Checksum
    __calculate_checksum(&pseudo_header_checksum, udp_length_protocol_id, 8);

    // 4.5. Deallocate udp_length_protocol_id from the pool
    pool_dealloc(udp_length_protocol_id, sizeof(uint8_t) * 8);

    /**
     * @brief The second part needed for the UDP checksum is the UDP packet,
     * i.e., the UDP header and UDP payload.
     *
     * @remark The only variable part is the UDP payload. Indeed, we cannot
     * predict the length of this part. However, we can deduce it from the UDP
     * length value.
     *
     * @details If the UDP payload length is odd, then one 0x00 byte is added in
     * the calculation of the UDP packet checksum.
     */

    // 1. Initialize UDP Packet Checksum
    udp_packet_checksum = 0;
    __calculate_checksum(&udp_packet_checksum, packet + 40,
                         udp_length_value - 2);

    // 2. Handle the last 16-bit of the UDP payload
    if (udp_length_value % 2 == 0) {
      __calculate_checksum(&udp_packet_checksum, packet + (packet_byte_len - 2),
                           2);
    } else {
      udp_packet_checksum += merge_uint8_t(packet[packet_byte_len - 1], 0x00);
      carry               = udp_packet_checksum >> 16;
      udp_packet_checksum = (udp_packet_checksum + carry) & 0xffff;
    }

    // Sum both checksums
    checksum_value = pseudo_header_checksum + udp_packet_checksum;

    // Normalize to uint16_t checksum_value
    carry          = checksum_value >> 16;
    checksum_value = (checksum_value + carry) & 0xffff;
    checksum_value = ~checksum_value & 0xffff;

    split_uint16_t(checksum, checksum + 1, (uint16_t) checksum_value);
  } else {  // IPv4 Stack
    // Not implemented yet
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
        *variable_length = merge_uint8_t(field[0], field[1]) + 269;
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

/* ********************************************************************** */
/*                             Static function                            */
/* ********************************************************************** */

static void __calculate_checksum(uint32_t* checksum, const uint8_t* data,
                                 size_t data_byte_length) {
  uint16_t carry;

  for (size_t i = 0; i < data_byte_length; i += 2) {
    *checksum += merge_uint8_t(data[i], data[i + 1]);
    carry     = *checksum >> 16;
    *checksum = (*checksum + carry) & 0xffff;
  }
}
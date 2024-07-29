#include "parser/parser.h"

/* ********************************************************************** */

int parse_header(header_t* header, int* offset, const uint8_t* packet,
                 const size_t packet_byte_len) {
  int parsing_status;

  parsing_status =
      parse_ipv6_header(&header->ipv6_hdr, offset, packet, packet_byte_len);

  if (header->ipv6_hdr.next_header[0] == 0x11) {
    parsing_status =
        parse_udp_header(&header->udp_hdr, offset, packet, packet_byte_len);
  } else {
    return 0;
  }

  // parse_coap_header : to do !

  return parsing_status;
}

/* ********************************************************************** */

int get_next_header_field(uint8_t** field_ptr, header_t* header,
                          const uint16_t fid) {
  int status;

  status = 1;
  switch (fid) {
    case FID_IPV6_VERSION:
      *field_ptr = header->ipv6_hdr.traffic_class;
      break;

    case FID_IPV6_TRAFFIC_CLASS:
      *field_ptr = header->ipv6_hdr.flow_label;
      break;

    case FID_IPV6_FLOW_LABEL:
      *field_ptr = header->ipv6_hdr.payload_length;
      break;

    case FID_IPV6_PAYLOAD_LENGTH:
      *field_ptr = header->ipv6_hdr.next_header;
      break;

    case FID_IPV6_NEXT_HEADER:
      *field_ptr = header->ipv6_hdr.hop_limit;
      break;

    case FID_IPV6_HOP_LIMIT:
      *field_ptr = header->ipv6_hdr.source_address;
      break;

    case FID_IPV6_SRC_ADDRESS:
      *field_ptr = header->ipv6_hdr.destination_address;
      break;

    case FID_IPV6_DST_ADDRESS:
      *field_ptr = header->udp_hdr.app_port;
      break;

    case FID_UDP_APP_PORT:
      *field_ptr = header->udp_hdr.dev_port;
      break;

    case FID_UDP_DEV_PORT:
      *field_ptr = header->udp_hdr.length;
      break;

    case FID_UDP_LENGTH:
      *field_ptr = header->udp_hdr.checksum;
      break;

    case FID_UDP_CHECKSUM:
      *field_ptr = NULL;
      status     = 0;
      break;

      // CoAP cases : to do !

    default:
      status = 0;
      break;
  }

  return status;
}

/* ********************************************************************** */

int get_fid_from_field_ptr(uint16_t* fid, uint8_t* field_ptr,
                           header_t* header) {
  int status;

  status = 1;
  if (field_ptr == header->ipv6_hdr.version) {
    *fid = FID_IPV6_VERSION;
  } else if (field_ptr == header->ipv6_hdr.traffic_class) {
    *fid = FID_IPV6_TRAFFIC_CLASS;
  } else if (field_ptr == header->ipv6_hdr.flow_label) {
    *fid = FID_IPV6_FLOW_LABEL;
  } else if (field_ptr == header->ipv6_hdr.payload_length) {
    *fid = FID_IPV6_PAYLOAD_LENGTH;
  } else if (field_ptr == header->ipv6_hdr.next_header) {
    *fid = FID_IPV6_NEXT_HEADER;
  } else if (field_ptr == header->ipv6_hdr.hop_limit) {
    *fid = FID_IPV6_HOP_LIMIT;
  } else if (field_ptr == header->ipv6_hdr.source_address) {
    *fid = FID_IPV6_SRC_ADDRESS;
  } else if (field_ptr == header->ipv6_hdr.destination_address) {
    *fid = FID_IPV6_DST_ADDRESS;
  } else if (field_ptr == header->udp_hdr.app_port) {
    *fid = FID_UDP_APP_PORT;
  } else if (field_ptr == header->udp_hdr.dev_port) {
    *fid = FID_UDP_DEV_PORT;
  } else if (field_ptr == header->udp_hdr.length) {
    *fid = FID_UDP_LENGTH;
  } else if (field_ptr == header->udp_hdr.checksum) {
    *fid = FID_UDP_CHECKSUM;
  } else {
    *fid   = 0;
    status = 0;
  }

  return status;
}
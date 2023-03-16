#include "ipv6.h"
#include "cschc.h"

/**
 * @brief parse an IPv6 header and returns its header descriptor
 * 
 * @param[in] buffer IPv6 header, assumed to be right-padded
 * @return header_descriptor_t* 
 */
header_descriptor_t* parse_ipv6_header(buffer_t* buffer){

    buffer_t* buffer_version;
    buffer_t* buffer_traffic_class;
    buffer_t* buffer_flow_label;
    buffer_t* buffer_payload_length;
    buffer_t* buffer_next_header;
    buffer_t* buffer_hop_limit;
    buffer_t* buffer_source_address;
    buffer_t* buffer_destination_address;

    field_descriptor_t* fd_version;
    field_descriptor_t* fd_traffic_class;
    field_descriptor_t* fd_flow_label;
    field_descriptor_t* fd_payload_length;
    field_descriptor_t* fd_next_header;
    field_descriptor_t* fd_hop_limit;
    field_descriptor_t* fd_source_address;
    field_descriptor_t* fd_destination_address;

    header_descriptor_t* header_descriptor;

    buffer_version = buffer_extract(buffer, 0, 4, LEFT);
    buffer_traffic_class = buffer_extract(buffer, 4, 8, LEFT);
    buffer_flow_label = buffer_extract(buffer, 12, 20, LEFT);
    buffer_payload_length = buffer_extract(buffer, 32, 16, LEFT);
    buffer_next_header = buffer_extract(buffer, 48, 8, LEFT);
    buffer_hop_limit = buffer_extract(buffer, 56, 8, LEFT);
    buffer_source_address = buffer_extract(buffer, 64, 128, LEFT);
    buffer_destination_address = buffer_extract(buffer, 192, 128, LEFT);

    fd_version = field_descriptor_init(IPV6_VERSION, buffer_version);
    fd_traffic_class = field_descriptor_init(IPV6_TRAFFIC_CLASS, buffer_traffic_class);
    fd_flow_label = field_descriptor_init(IPV6_FLOW_LABEL, buffer_flow_label);
    fd_payload_length = field_descriptor_init(IPV6_PAYLOAD_LENGTH, buffer_payload_length);
    fd_next_header = field_descriptor_init(IPV6_NEXT_HEADER, buffer_next_header);
    fd_hop_limit = field_descriptor_init(IPV6_HOP_LIMIT, buffer_hop_limit);
    fd_source_address = field_descriptor_init(IPV6_SRC_ADDRESS, buffer_source_address);
    fd_destination_address = field_descriptor_init(IPV6_DST_ADDRESS, buffer_destination_address);

    header_descriptor = header_descriptor_init(IPV6_HEADER_ID, 8, 
        fd_version,
        fd_traffic_class,
        fd_flow_label,
        fd_payload_length,
        fd_next_header,
        fd_hop_limit,
        fd_source_address,
        fd_destination_address
    );
    
    return header_descriptor;
}
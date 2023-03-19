#include "udp.h"

header_descriptor_t* parse_udp_header(buffer_t* buffer){
    buffer_t* buffer_source_port;
    buffer_t* buffer_destination_port;
    buffer_t* buffer_length;
    buffer_t* buffer_checksum;

    field_descriptor_t* fd_source_port;
    field_descriptor_t* fd_destination_port;
    field_descriptor_t* fd_length;
    field_descriptor_t* fd_checksum;

    header_descriptor_t* header_descriptor;

    buffer_source_port = buffer_extract(buffer, 0, 16, LEFT);
    buffer_destination_port = buffer_extract(buffer, 16, 16, LEFT);
    buffer_length = buffer_extract(buffer, 32, 16, LEFT);
    buffer_checksum = buffer_extract(buffer, 48, 16, LEFT);

    fd_source_port = field_descriptor_init(UDP_SOURCE_PORT, buffer_source_port);
    fd_destination_port = field_descriptor_init(UDP_DESTINATION_PORT, buffer_destination_port);
    fd_length = field_descriptor_init(UDP_LENGTH, buffer_length);
    fd_checksum = field_descriptor_init(UDP_CHECKSUM, buffer_checksum);

    header_descriptor = header_descriptor_init(UDP_HEADER_ID, 4, 
        fd_source_port,
        fd_destination_port,
        fd_length,
        fd_checksum
    );

    return header_descriptor;
}
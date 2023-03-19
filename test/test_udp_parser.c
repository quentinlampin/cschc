#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "udp.h"

#define TEST_SUCCESS 0

int test_udp_parser(){
    /**
     * @brief testing the UDP parser on a UDP header
     * 
     * The packet is made of a UDP header with following fields:
     *  - id='Source Port'          length=4    position=0  value={0x23, 0x29}
     *  - id='Destination Port'     length=8    position=0  value={0x23, 0x2a}
     *  - id='Length'               length=20   position=0  value={0x00, 0x10}
     *  - id='Checksum'             length=16   position=0  value={0x2d, 0xa1}
     * 
     */
    const unsigned char buffer_content_udp_header[] = {
        0x23, 0x29, 0x23, 0x2a, 0x00, 0x10, 0x2d, 0xa1,
    };

    const unsigned char buffer_content_source_port[] = {0x23, 0x29};
    const unsigned char buffer_content_destination_port[] = {0x23, 0x2a};
    const unsigned char buffer_content_length[] = {0x00, 0x10};
    const unsigned char buffer_content_checksum[] = {0x2d, 0xa1};
    
    buffer_t* buffer_udp_header = buffer_init(buffer_content_udp_header, UDP_HEADER_LENGTH, RIGHT);

    header_descriptor_t* header_descriptor_udp = parse_udp_header(buffer_udp_header);

    buffer_t* buffer_source_port = header_descriptor_udp->field_descriptors[0]->buffer;
    buffer_t* buffer_destination_port = header_descriptor_udp->field_descriptors[1]->buffer;
    buffer_t* buffer_length = header_descriptor_udp->field_descriptors[2]->buffer;
    buffer_t* buffer_checksum = header_descriptor_udp->field_descriptors[3]->buffer;
    
    assert(memcmp(buffer_source_port->content, buffer_content_source_port, 2) == 0);
    assert(memcmp(buffer_destination_port->content, buffer_content_destination_port, 2) == 0);
    assert(memcmp(buffer_length->content, buffer_content_length, 2) == 0);
    assert(memcmp(buffer_checksum->content, buffer_content_checksum, 2) == 0);
    
    buffer_deinit(buffer_udp_header);
    header_descriptor_deinit(header_descriptor_udp);
    return TEST_SUCCESS;
}

int main(){
    assert(test_udp_parser() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ipv6.h"

#define TEST_SUCCESS 0


int test_ipv6_parser(){
    /**
     * @brief testing the IPv6 parser on an IPv6 header
     * 
     * The packet is made of an IPv6 header with following fields:
     *  - id='Version'              length=4    position=0  value={0x06}
     *  - id='Traffic Class'        length=8    position=0  value={0x00}
     *  - id='Flow Label'           length=20   position=0  value={0x00, 0x00, 0x00}
     *  - id='Payload Length'       length=16   position=0  value={0x10}
     *  - id='Next Header'          length=8    position=0  value={0x11}
     *  - id='Hop Limit'            length=8    position=0  value={0x40}
     *  - id='Source Address'       length=128  position=0  value={0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1}
     *  - id='Destination Address'  length=128  position=0  value={0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2}
     * 
     */
    const unsigned char buffer_content_ipv6_header[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x40, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2, 0x23, 0x29, 0x23, 0x2a, 0x00, 0x10, 0x2d, 0xa1,
        0x64, 0x65, 0x61, 0x64, 0x62, 0x65, 0x65, 0x66
    };

    const unsigned char buffer_content_version[] = {0x06};
    const unsigned char buffer_content_traffic_class[] = {0x00};
    const unsigned char buffer_content_flow_label[] = {0x00, 0x00, 0x00};
    const unsigned char buffer_content_payload_length[] = {0x00, 0x10};
    const unsigned char buffer_content_next_header[] = {0x11};
    const unsigned char buffer_content_hop_limit[] = {0x40};
    const unsigned char buffer_content_source_address[] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1};
    const unsigned char buffer_content_destination_address[] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2};

    buffer_t* buffer_ipv6_header = buffer_init(buffer_content_ipv6_header, IPV6_HEADER_LENGTH, RIGHT);

    header_descriptor_t* header_descriptor_ipv6 = parse_ipv6_header(buffer_ipv6_header);

    buffer_t* buffer_version = header_descriptor_ipv6->field_descriptors[0]->buffer;
    buffer_t* buffer_traffic_class = header_descriptor_ipv6->field_descriptors[1]->buffer;
    buffer_t* buffer_flow_label = header_descriptor_ipv6->field_descriptors[2]->buffer;
    buffer_t* buffer_payload_length = header_descriptor_ipv6->field_descriptors[3]->buffer;
    buffer_t* buffer_next_header = header_descriptor_ipv6->field_descriptors[4]->buffer;
    buffer_t* buffer_hop_limit = header_descriptor_ipv6->field_descriptors[5]->buffer;
    buffer_t* buffer_source_address = header_descriptor_ipv6->field_descriptors[6]->buffer;
    buffer_t* buffer_destination_address = header_descriptor_ipv6->field_descriptors[7]->buffer;

    assert(memcmp(buffer_version->content, buffer_content_version, 1) == 0);
    assert(memcmp(buffer_traffic_class->content, buffer_content_traffic_class, 1) == 0);
    assert(memcmp(buffer_flow_label->content, buffer_content_flow_label, 3) == 0);
    assert(memcmp(buffer_payload_length->content, buffer_content_payload_length, 2) == 0);
    assert(memcmp(buffer_next_header->content, buffer_content_next_header, 1) == 0);
    assert(memcmp(buffer_hop_limit->content, buffer_content_hop_limit, 1) == 0);
    assert(memcmp(buffer_source_address->content, buffer_content_source_address, 16) == 0);
    assert(memcmp(buffer_destination_address->content, buffer_content_destination_address, 16) == 0);

    buffer_deinit(buffer_ipv6_header);
    header_descriptor_deinit(header_descriptor_ipv6);
    return TEST_SUCCESS;
}

int main(){
    assert(test_ipv6_parser() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
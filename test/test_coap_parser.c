#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "coap.h"

#define TEST_SUCCESS 0

int test_coap_parser(){
    /**
     * @brief testing the CoAP parser on a CoAP header
     * 
     The packet is made of a CoAP header with following fields:
        - id='Version'                length=2    position=0  value={0x01}
        - id='Type'                   length=2    position=0  value={0x00}
        - id='Token Length'           length=4    position=0  value={0x08}
        - id='Code'                   length=8    position=0  value={0x02}
        - id='message ID'             length=16   position=0  value={0x84, 0x99}
        - id='Token'                  length=64   position=0  value={0x74, 0xcd, 0xe8, 0xcb, 0x4e, 0x8c, 0x0d, 0xb7}

        - id='Option Delta'           length=4    position=0  value={0x0B}
        - id='Option Length'          length=4    position=0  value={0x02}
        - id='Option Value'           length=16   position=0  value={0x72, 0x64}

        - id='Option Delta'           length=4    position=1  value={0x01}
        - id='Option Length'          length=4    position=1  value={0x01}
        - id='Option Value'           length=8    position=1  value={0x28}

        - id='Option Delta'           length=4    position=2  value={0x03}
        - id='Option Length'          length=4    position=2  value={0x03}
        - id='Option Value'           length=24   position=2  value={0x62, 0x3d, 0x55}

        - id='Option Delta'           length=4    position=3  value={0x00}
        - id='Option Length'          length=4    position=3  value={0x09}
        - id='Option Value'           length=72   position=3  value={0x6c, 0x77, 0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31}

        - id='Option Delta'           length=4    position=4  value={0x00}
        - id='Option Length'          length=4    position=4  value={0x06}
        - id='Option Value'           length=48   position=4  value={0x6c, 0x74, 0x3d, 0x33, 0x30, 0x30}

        - id='Option Delta'           length=4    position=5  value={0x00}
        - id='Option Length'          length=4    position=5  value={0x0d}
        - id='Option Length Extended' length=8    position=0  value={0x02}
        - id='Option Value'           length=120  position=5  value={0x65, 0x70, 0x3d, 0x38, 0x35, 0x62, 0x61, 0x39, 0x62, 0x64, 0x61, 0x63, 0x30, 0x62, 0x65}
        
        - id='Option Delta'           length=4    position=6  value={0x0c}
        - id='Option Length'          length=4    position=6  value={0x01}
        - id='Option Value'           length=8    position=6  value={0x0d}

        - id='Option Delta'           length=4    position=7  value={0x0d}
        - id='Option Length'          length=4    position=7  value={0x02}
        - id='Option Delta Extended'  length=8    position=0  value={0x14}
        - id='Option Value'           length=8    position=7  value={0x07, 0x2b}

        - id='Payload Marker'          length=8    position=0  value={0xff}

     */
    const unsigned char buffer_content_coap_header[] = { 
        0x48, 0x02, 0x84, 0x99, 0x74, 0xcd, 0xe8, 0xcb, 0x4e, 0x8c, 0x0d, 0xb7, 0xb2, 0x72, 0x64, 0x11,
        0x28, 0x33, 0x62, 0x3d, 0x55, 0x09, 0x6c, 0x77, 0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31, 0x06,
        0x6c, 0x74, 0x3d, 0x33, 0x30, 0x30, 0x0d, 0x02, 0x65, 0x70, 0x3d, 0x38, 0x35, 0x62, 0x61, 0x39,
        0x62, 0x64, 0x61, 0x63, 0x30, 0x62, 0x65, 0xc1, 0x0d, 0xd2, 0x14, 0x07, 0x2b, 0xff
    };

    const unsigned char buffer_content_coap_version[] = {0x01};
    const unsigned char buffer_content_coap_type[] = {0x00};
    const unsigned char buffer_content_coap_token_length[] = {0x08};
    const unsigned char buffer_content_coap_code[] = {0x02};
    const unsigned char buffer_content_coap_message_id[] = {0x84, 0x99};
    const unsigned char buffer_content_coap_token[] = {0x74, 0xcd, 0xe8, 0xcb, 0x4e, 0x8c, 0x0d, 0xb7};

    const unsigned char buffer_content_coap_option_delta_1[] = {0x0b};
    const unsigned char buffer_content_coap_option_length_1[] = {0x02};
    const unsigned char buffer_content_coap_option_value_1[] = {0x72, 0x64};

    const unsigned char buffer_content_coap_option_delta_2[] = {0x01};
    const unsigned char buffer_content_coap_option_length_2[] = {0x01};
    const unsigned char buffer_content_coap_option_value_2[] = {0x28};

    const unsigned char buffer_content_coap_option_delta_3[] = {0x03};
    const unsigned char buffer_content_coap_option_length_3[] = {0x03};
    const unsigned char buffer_content_coap_option_value_3[] = {0x62, 0x3d, 0x55};

    const unsigned char buffer_content_coap_option_delta_4[] = {0x00};
    const unsigned char buffer_content_coap_option_length_4[] = {0x09};
    const unsigned char buffer_content_coap_option_value_4[] = {0x6c, 0x77, 0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31};

    const unsigned char buffer_content_coap_option_delta_5[] = {0x00};
    const unsigned char buffer_content_coap_option_length_5[] = {0x06};
    const unsigned char buffer_content_coap_option_value_5[] = {0x6c, 0x74, 0x3d, 0x33, 0x30, 0x30};

    const unsigned char buffer_content_coap_option_delta_6[] = {0x00};
    const unsigned char buffer_content_coap_option_length_6[] = {0x0d};
    const unsigned char buffer_content_coap_option_length_extended_6[] = {0x02};
    const unsigned char buffer_content_coap_option_value_6[] = {0x65, 0x70, 0x3d, 0x38, 0x35, 0x62, 0x61, 0x39, 0x62, 0x64, 0x61, 0x63, 0x30, 0x62, 0x65};

    const unsigned char buffer_content_coap_option_delta_7[] = {0x0c};
    const unsigned char buffer_content_coap_option_length_7[] = {0x01};
    const unsigned char buffer_content_coap_option_value_7[] = {0x0d};

    const unsigned char buffer_content_coap_option_delta_8[] = {0x0d};
    const unsigned char buffer_content_coap_option_length_8[] = {0x02};
    const unsigned char buffer_content_coap_option_length_extended_8[] = {0x14};
    const unsigned char buffer_content_coap_option_value_8[] = {0x07, 0x2b};

    const unsigned char buffer_content_coap_payload_marker[] = {0xff};
    
    buffer_t* buffer_coap_header = buffer_init(buffer_content_coap_header, 496, RIGHT);
    header_descriptor_t* header_descriptor_coap = parse_coap_header(buffer_coap_header);

    buffer_t* buffer_coap_version = header_descriptor_coap->field_descriptors[0]->buffer;
    assert(memcmp(buffer_coap_version->content, buffer_content_coap_version, 1) == 0);


    buffer_t* buffer_coap_type = header_descriptor_coap->field_descriptors[1]->buffer;
    assert(memcmp(buffer_coap_type->content, buffer_content_coap_type, 1) == 0);

    buffer_t* buffer_coap_token_length = header_descriptor_coap->field_descriptors[2]->buffer;
    assert(memcmp(buffer_coap_token_length->content, buffer_content_coap_token_length, 1) == 0);


    buffer_t* buffer_coap_code = header_descriptor_coap->field_descriptors[3]->buffer;
    assert(memcmp(buffer_coap_code->content, buffer_content_coap_code, 1) == 0);

    buffer_t* buffer_coap_message_id = header_descriptor_coap->field_descriptors[4]->buffer;
    assert(memcmp(buffer_coap_message_id->content, buffer_content_coap_message_id, 2) == 0);


    buffer_t* buffer_coap_token = header_descriptor_coap->field_descriptors[5]->buffer;
    assert(memcmp(buffer_coap_token->content, buffer_content_coap_token, 8) == 0);


    buffer_t* buffer_coap_option_delta_1 = header_descriptor_coap->field_descriptors[6]->buffer;
    assert(memcmp(buffer_coap_option_delta_1->content, buffer_content_coap_option_delta_1, 1) == 0);


    buffer_t* buffer_coap_option_length_1 = header_descriptor_coap->field_descriptors[7]->buffer;
    assert(memcmp(buffer_coap_option_length_1->content, buffer_content_coap_option_length_1, 1) == 0);

    buffer_t* buffer_coap_option_value_1 = header_descriptor_coap->field_descriptors[8]->buffer;
    assert(memcmp(buffer_coap_option_value_1->content, buffer_content_coap_option_value_1, 2) == 0);
    
    buffer_t* buffer_coap_option_delta_2 = header_descriptor_coap->field_descriptors[9]->buffer;
    assert(memcmp(buffer_coap_option_delta_2->content, buffer_content_coap_option_delta_2, 1) == 0);

    buffer_t* buffer_coap_option_length_2 = header_descriptor_coap->field_descriptors[10]->buffer;
    assert(memcmp(buffer_coap_option_length_2->content, buffer_content_coap_option_length_2, 1) == 0);

    buffer_t* buffer_coap_option_value_2 = header_descriptor_coap->field_descriptors[11]->buffer;
    assert(memcmp(buffer_coap_option_value_2->content, buffer_content_coap_option_value_2, 1) == 0);

    buffer_t* buffer_coap_option_delta_3 = header_descriptor_coap->field_descriptors[12]->buffer;
    assert(memcmp(buffer_coap_option_delta_3->content, buffer_content_coap_option_delta_3, 1) == 0);

    buffer_t* buffer_coap_option_length_3 = header_descriptor_coap->field_descriptors[13]->buffer;
    assert(memcmp(buffer_coap_option_length_3->content, buffer_content_coap_option_length_3, 1) == 0);

    buffer_t* buffer_coap_option_value_3 = header_descriptor_coap->field_descriptors[14]->buffer;
    assert(memcmp(buffer_coap_option_value_3->content, buffer_content_coap_option_value_3, 3) == 0);

    buffer_t* buffer_coap_option_delta_4 = header_descriptor_coap->field_descriptors[15]->buffer;
    assert(memcmp(buffer_coap_option_delta_4->content, buffer_content_coap_option_delta_4, 1) == 0);

    buffer_t* buffer_coap_option_length_4 = header_descriptor_coap->field_descriptors[16]->buffer;
    assert(memcmp(buffer_coap_option_length_4->content, buffer_content_coap_option_length_4, 1) == 0);

    buffer_t* buffer_coap_option_value_4 = header_descriptor_coap->field_descriptors[17]->buffer;
    assert(memcmp(buffer_coap_option_value_4->content, buffer_content_coap_option_value_4, 9) == 0);

    buffer_t* buffer_coap_option_delta_5 = header_descriptor_coap->field_descriptors[18]->buffer;
    assert(memcmp(buffer_coap_option_delta_5->content, buffer_content_coap_option_delta_5, 1) == 0);

    buffer_t* buffer_coap_option_length_5 = header_descriptor_coap->field_descriptors[19]->buffer;
    assert(memcmp(buffer_coap_option_length_5->content, buffer_content_coap_option_length_5, 1) == 0);

    buffer_t* buffer_coap_option_value_5 = header_descriptor_coap->field_descriptors[20]->buffer;
    assert(memcmp(buffer_coap_option_value_5->content, buffer_content_coap_option_value_5, 6) == 0);


    buffer_t* buffer_coap_option_delta_6 = header_descriptor_coap->field_descriptors[21]->buffer;
    assert(memcmp(buffer_coap_option_delta_6->content, buffer_content_coap_option_delta_6, 1) == 0);

    buffer_t* buffer_coap_option_length_6 = header_descriptor_coap->field_descriptors[22]->buffer;
    assert(memcmp(buffer_coap_option_length_6->content, buffer_content_coap_option_length_6, 1) == 0);

    buffer_t* buffer_coap_option_length_extended_6 = header_descriptor_coap->field_descriptors[23]->buffer;
    assert(memcmp(buffer_coap_option_length_extended_6->content, buffer_content_coap_option_length_extended_6, 1) == 0);

    buffer_t* buffer_coap_option_value_6 = header_descriptor_coap->field_descriptors[24]->buffer;
    assert(memcmp(buffer_coap_option_value_6->content, buffer_content_coap_option_value_6, 15) == 0);

    buffer_t* buffer_coap_option_delta_7 = header_descriptor_coap->field_descriptors[25]->buffer;
    assert(memcmp(buffer_coap_option_delta_7->content, buffer_content_coap_option_delta_7, 1) == 0);

    buffer_t* buffer_coap_option_length_7 = header_descriptor_coap->field_descriptors[26]->buffer;
    assert(memcmp(buffer_coap_option_length_7->content, buffer_content_coap_option_length_7, 1) == 0);

    buffer_t* buffer_coap_option_value_7 = header_descriptor_coap->field_descriptors[27]->buffer;
    assert(memcmp(buffer_coap_option_value_7->content, buffer_content_coap_option_value_7, 1) == 0);

    buffer_t* buffer_coap_option_delta_8 = header_descriptor_coap->field_descriptors[28]->buffer;
    assert(memcmp(buffer_coap_option_delta_8->content, buffer_content_coap_option_delta_8, 1) == 0);

    buffer_t* buffer_coap_option_length_8 = header_descriptor_coap->field_descriptors[29]->buffer;
    assert(memcmp(buffer_coap_option_length_8->content, buffer_content_coap_option_length_8, 1) == 0);

    buffer_t* buffer_coap_option_length_extended_8 = header_descriptor_coap->field_descriptors[30]->buffer;
    assert(memcmp(buffer_coap_option_length_extended_8->content, buffer_content_coap_option_length_extended_8, 1) == 0);

    buffer_t* buffer_coap_option_value_8 = header_descriptor_coap->field_descriptors[31]->buffer;
    assert(memcmp(buffer_coap_option_value_8->content, buffer_content_coap_option_value_8, 2) == 0);

    buffer_t* buffer_coap_payload_marker = header_descriptor_coap->field_descriptors[32]->buffer;
    assert(memcmp(buffer_coap_payload_marker->content, buffer_content_coap_payload_marker, 1) == 0);

    buffer_deinit(buffer_coap_header);
    header_descriptor_deinit(header_descriptor_coap);
    return TEST_SUCCESS;
}

int main(){
    assert(test_coap_parser() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
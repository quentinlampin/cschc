#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cschc.h"

#define TEST_SUCCESS 0


int test_header_descriptor_init_deinit(){
    uint8_t length_1 = 32;
    const unsigned char buffer_content_1[] = {0x01, 0x02, 0x03, 0x04};
    buffer_t* buffer_1 = buffer_init(buffer_content_1, length_1, LEFT);
    const char field_id_1[] = "field_1";
    field_descriptor_t* field_descriptor_1 = field_descriptor_init(field_id_1, buffer_1);

    uint8_t length_2 = 24;
    const unsigned char buffer_content_2[] = {0x01, 0x02, 0x03};
    buffer_t* buffer_2 = buffer_init(buffer_content_2, length_2, LEFT);
    const char field_id_2[] = "field_2";
    field_descriptor_t* field_descriptor_2 = field_descriptor_init(field_id_2, buffer_2);

    const char header_id[] = "protocol_1";
    header_descriptor_t* header_descriptor = header_descriptor_init(header_id, 2, field_descriptor_1, field_descriptor_2);

    assert(strcmp(header_descriptor->id, header_id) == 0);
    assert(header_descriptor->length == 56);
    assert(header_descriptor->field_descriptors_count == 2);

    assert(memcmp(header_descriptor->field_descriptors[0]->buffer->content, buffer_content_1, 4) == 0);
    assert(memcmp(header_descriptor->field_descriptors[1]->buffer->content, buffer_content_2, 3) == 0);

    header_descriptor_deinit(header_descriptor);
    return TEST_SUCCESS;
}

int test_header_descriptor_concat(){
    uint8_t length_1 = 32;
    const unsigned char buffer_content_1[] = {0x01, 0x02, 0x03, 0x04};
    buffer_t* buffer_1 = buffer_init(buffer_content_1, length_1, LEFT);
    const char field_id_1[] = "field_1";
    field_descriptor_t* field_descriptor_1 = field_descriptor_init(field_id_1, buffer_1);

    uint8_t length_2 = 24;
    const unsigned char buffer_content_2[] = {0x01, 0x02, 0x03};
    buffer_t* buffer_2 = buffer_init(buffer_content_2, length_2, LEFT);
    const char field_id_2[] = "field_2";
    field_descriptor_t* field_descriptor_2 = field_descriptor_init(field_id_2, buffer_2);

    const char header_id_1[] = "protocol_1_1";
    header_descriptor_t* header_descriptor_1 = header_descriptor_init(header_id_1, 2, field_descriptor_1, field_descriptor_2);

    uint8_t length_3 = 8;
    const unsigned char buffer_content_3[] = {0x01};
    buffer_t* buffer_3 = buffer_init(buffer_content_3, length_3, LEFT);
    const char field_id_3[] = "field_3";
    field_descriptor_t* field_descriptor_3 = field_descriptor_init(field_id_3, buffer_3);

    uint8_t length_4 = 16;
    const unsigned char buffer_content_4[] = {0x01, 0x02};
    buffer_t* buffer_4 = buffer_init(buffer_content_4, length_4, LEFT);
    const char field_id_4[] = "field_4";
    field_descriptor_t* field_descriptor_4 = field_descriptor_init(field_id_4, buffer_4);

    const char header_id_2[] = "protocol_1_2";
    header_descriptor_t* header_descriptor_2 = header_descriptor_init(header_id_2, 2, field_descriptor_3, field_descriptor_4);

    const char header_id[] = "protocol_1";
    header_descriptor_t* header_descriptor = header_descriptor_concat(header_id, 2, header_descriptor_1, header_descriptor_2);

    assert(strcmp(header_descriptor->id, header_id) == 0);
    assert(header_descriptor->length == 80);
    assert(header_descriptor->field_descriptors_count == 4);

    assert(memcmp(header_descriptor->field_descriptors[0]->buffer->content, buffer_content_1, 4) == 0);
    assert(memcmp(header_descriptor->field_descriptors[1]->buffer->content, buffer_content_2, 3) == 0);
    assert(memcmp(header_descriptor->field_descriptors[2]->buffer->content, buffer_content_3, 1) == 0);
    assert(memcmp(header_descriptor->field_descriptors[3]->buffer->content, buffer_content_4, 2) == 0);

    header_descriptor_deinit(header_descriptor);
    return TEST_SUCCESS;

}

int main(){
    assert(test_header_descriptor_init_deinit() == TEST_SUCCESS);
    assert(test_header_descriptor_concat() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
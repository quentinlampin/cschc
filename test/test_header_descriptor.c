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

    // header_descriptor_deinit(header_descriptor);
    return TEST_SUCCESS;
}

int main(){
    assert(test_header_descriptor_init_deinit() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
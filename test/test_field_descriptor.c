#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cschc.h"

#define TEST_SUCCESS 0


int test_field_descriptor_init_deinit(){
    uint8_t length = 32;
    const unsigned char buffer_content[] = {0x01, 0x02, 0x03, 0x04};
    buffer_t* buffer = buffer_init(buffer_content, length, LEFT);
    
    assert(buffer != NULL);
    assert(buffer->length == length);
    assert(memcmp(buffer->content, buffer_content, buffer->byte_length) == 0);
    
    const char field_id[] = "field_1";
    field_descriptor_t* field_descriptor = field_descriptor_init(field_id, buffer);

    assert(field_descriptor != NULL);
    assert(memcmp(field_descriptor->buffer->content, buffer_content, 4) == 0);
    assert(strcmp(field_descriptor->id, field_id) == 0);
    return TEST_SUCCESS;
}

int main(){
    assert(test_field_descriptor_init_deinit() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
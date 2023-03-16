#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cschc.h"

#define TEST_SUCCESS 0

int test_buffer_malloc_dealloc(){
    uint8_t length = 32;
    const unsigned char buffer_content[] = {0x01, 0x02, 0x03, 0x04};
    buffer_t* buffer = buffer_init(buffer_content, length, LEFT);
    
    assert(buffer != NULL);
    assert(buffer->length == length);
    assert(memcmp(buffer->content, buffer_content, buffer->byte_length) == 0);
    buffer_deinit(buffer);

    return TEST_SUCCESS;
}

int test_buffer_shift(){
    const unsigned char buffer_content_1[] = {0xf0, 0xf0};
    const unsigned char buffer_content_2[] = {0x0f};
    const unsigned char buffer_content_3[] = {0xff, 0x0f};
    const unsigned char buffer_content_4[] = {0xf0,0x10};
    const unsigned char buffer_content_5[] = {0xf0,0xff};
    
    const unsigned char shifted_buffer_content_1[] = {0x0f, 0x0f, 0x00};
    const unsigned char shifted_buffer_content_2[] = {0x0f, 0x0f, 0x00, 0x00};
    const unsigned char shifted_buffer_content_3[] = {0xf0};
    const unsigned char shifted_buffer_content_4[] = {0x0f, 0xf0};
    const unsigned char shifted_buffer_content_5[] = {0xf0,0x10};
    const unsigned char shifted_buffer_content_6[] = {0xf0,0x10,0x00};
    const unsigned char shifted_buffer_content_7[] = {0xf0,0xc0};

    
    buffer_t* buffer_1 = buffer_init(buffer_content_1, 16, LEFT);
    assert(memcmp(buffer_1->content, buffer_content_1, sizeof(buffer_content_1)) == 0);

    buffer_t* shifted_buffer;
    
    shifted_buffer = buffer_shift(buffer_1, 0);
    assert(shifted_buffer->length == buffer_1->length);
    assert(shifted_buffer->byte_length == buffer_1->byte_length);
    assert(shifted_buffer->padding == LEFT);
    assert(shifted_buffer->padding_length == 0);
    assert(memcmp(shifted_buffer->content,buffer_content_1, 2) == 0);
    buffer_deinit(shifted_buffer);
    
    shifted_buffer = buffer_shift(buffer_1, -4);
    assert(shifted_buffer->length == buffer_1->length+4);
    assert(shifted_buffer->byte_length == buffer_1->byte_length+1);
    assert(shifted_buffer->padding == LEFT);
    assert(shifted_buffer->padding_length == 4);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_1, 3) == 0);
    buffer_deinit(shifted_buffer);

    shifted_buffer = buffer_shift(buffer_1, -12);
    assert(shifted_buffer->length == buffer_1->length+12);
    assert(shifted_buffer->byte_length == buffer_1->byte_length+2);
    assert(shifted_buffer->padding == LEFT);
    assert(shifted_buffer->padding_length == 4);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_2, 4) == 0);
    buffer_deinit(shifted_buffer);
    buffer_deinit(buffer_1);

    buffer_t* buffer_2 = buffer_init(buffer_content_2, 4, LEFT);

    shifted_buffer = buffer_shift(buffer_2, -4);
    assert(shifted_buffer->length == buffer_2->length+4);
    assert(shifted_buffer->byte_length == buffer_2->byte_length);
    assert(shifted_buffer->padding == LEFT);
    assert(shifted_buffer->padding_length == 0);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_3, 1) == 0);
    buffer_deinit(shifted_buffer);
    buffer_deinit(buffer_2);

    buffer_t* buffer_3 = buffer_init(buffer_content_3, 16, LEFT);

    shifted_buffer = buffer_shift(buffer_3, 4);
    assert(shifted_buffer->length == buffer_3->length-4);
    assert(shifted_buffer->byte_length == buffer_3->byte_length);
    assert(shifted_buffer->padding == LEFT);
    assert(shifted_buffer->padding_length == 4);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_4, 2) == 0);
    buffer_deinit(shifted_buffer);
    buffer_deinit(buffer_3);

    buffer_t* buffer_4 = buffer_init(buffer_content_4, 12, RIGHT);

    shifted_buffer = buffer_shift(buffer_4, -4);
    assert(shifted_buffer->length == buffer_4->length+4);
    assert(shifted_buffer->byte_length == buffer_4->byte_length);
    assert(shifted_buffer->padding == RIGHT);
    assert(shifted_buffer->padding_length == 0);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_5, 2) == 0);
    buffer_deinit(shifted_buffer);

    shifted_buffer = buffer_shift(buffer_4, -12);
    assert(shifted_buffer->length == buffer_4->length+12);
    assert(shifted_buffer->byte_length == buffer_4->byte_length+1);
    assert(shifted_buffer->padding == RIGHT);
    assert(shifted_buffer->padding_length == 0);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_6, 3) == 0);
    buffer_deinit(shifted_buffer);
    buffer_deinit(buffer_4);

    buffer_t* buffer_5 = buffer_init(buffer_content_5, 16, RIGHT);
    shifted_buffer = buffer_shift(buffer_5, 6);
    assert(shifted_buffer->length == buffer_5->length-6);
    assert(shifted_buffer->byte_length == buffer_5->byte_length);
    assert(shifted_buffer->padding == RIGHT);
    assert(shifted_buffer->padding_length == 6);
    assert(memcmp(shifted_buffer->content,shifted_buffer_content_7, 2) == 0);
    buffer_deinit(shifted_buffer);
    buffer_deinit(buffer_5);

    return TEST_SUCCESS;
}

int test_buffer_pad(){
    buffer_t* buffer_padded;

    const unsigned char buffer_content_1[] = {0xf0, 0xf0};
    buffer_t* buffer_1 = buffer_init(buffer_content_1, 16, LEFT);
    const unsigned char buffer_padded_content_1[] = {0xf0, 0xf0};
    
    buffer_padded = buffer_pad(buffer_1, LEFT);
    assert(buffer_padded->padding == LEFT);
    assert(buffer_padded->length == 16);
    assert(buffer_padded->byte_length == 2);
    assert(memcmp(buffer_padded->content, buffer_padded_content_1, 2) == 0);
    buffer_deinit(buffer_padded);

    buffer_padded = buffer_pad(buffer_1, RIGHT);
    assert(buffer_padded->padding == RIGHT);
    assert(buffer_padded->length == 16);
    assert(buffer_padded->byte_length == 2);
    assert(memcmp(buffer_padded->content, buffer_padded_content_1, 2) == 0);
    buffer_deinit(buffer_padded);
    buffer_deinit(buffer_1);

    buffer_t* buffer_2 = buffer_init(buffer_content_1, 12, RIGHT);
    const unsigned char buffer_padded_content_2[] = {0x0f, 0x0f};
    buffer_padded = buffer_pad(buffer_2, LEFT);
    assert(buffer_padded->padding == LEFT);
    assert(buffer_padded->length == 12);
    assert(buffer_padded->byte_length == 2);
    assert(memcmp(buffer_padded->content, buffer_padded_content_2, 2) == 0);
    buffer_deinit(buffer_padded);
    buffer_deinit(buffer_2);

    const unsigned char buffer_content_2[] = {0x0f, 0xff};
    buffer_t* buffer_3 = buffer_init(buffer_content_2, 12, LEFT);
    const unsigned char buffer_padded_content_3[] = {0xff, 0xf0};
    buffer_padded = buffer_pad(buffer_3, RIGHT);
    assert(buffer_padded->padding == RIGHT);
    assert(buffer_padded->length == 12);
    assert(buffer_padded->byte_length == 2);
    assert(memcmp(buffer_padded->content, buffer_padded_content_3, 2) == 0);
    buffer_deinit(buffer_padded);
    buffer_deinit(buffer_3);

    return TEST_SUCCESS;
}

int test_buffer_concat(){
    /* simple case: no actual padding (8bits aligned) */
    const unsigned char buffer_content_1_1[] = {0xf0, 0xf0};
    const unsigned char buffer_content_1_2[] = {0xff, 0x0f};
    const unsigned char buffer_content_expected_1[] = {0xf0, 0xf0, 0xff, 0x0f};
    buffer_t* buffer_1_1 = buffer_init(buffer_content_1_1, 16, LEFT);
    buffer_t* buffer_1_2 = buffer_init(buffer_content_1_2, 16, LEFT);
    buffer_t* buffer_1_concat = buffer_concat(buffer_1_1, buffer_1_2);
    assert(buffer_1_concat->byte_length == 4);
    assert(buffer_1_concat->length == 32);
    assert(buffer_1_concat->padding == LEFT);
    assert(memcmp(buffer_1_concat->content, buffer_content_expected_1, 4) == 0);

    buffer_deinit(buffer_1_1);
    buffer_deinit(buffer_1_2);
    buffer_deinit(buffer_1_concat);

    return TEST_SUCCESS;
}

int test_buffer_equal(){
    const unsigned char buffer_content_1[] = {0xf0, 0xff, 0xe0};
    buffer_t* buffer_1 = buffer_init(buffer_content_1, 24, RIGHT);
    buffer_t* buffer_2 = buffer_init(buffer_content_1, 24, RIGHT);
    assert(buffer_equal(buffer_1, buffer_2, 24) == true);

    const unsigned char buffer_content_3[] = {0xf0, 0xff, 0xef};
    buffer_t* buffer_3 = buffer_init(buffer_content_3, 24, RIGHT);
    assert(buffer_equal(buffer_1, buffer_3, 20) == true);
    assert(buffer_equal(buffer_1, buffer_3, 24) == false);

    const unsigned char buffer_content_4[] = {0x0f, 0xff, 0xef};
    buffer_t* buffer_4 = buffer_init(buffer_content_4, 24, LEFT);
    const unsigned char buffer_content_5[] = {0x0f, 0xff, 0xef};
    buffer_t* buffer_5 = buffer_init(buffer_content_5, 24, LEFT);
    assert(buffer_equal(buffer_4, buffer_5, 24));

    const unsigned char buffer_content_6[] = {0x0f, 0xff, 0xef};
    buffer_t* buffer_6 = buffer_init(buffer_content_6, 24, LEFT);
    const unsigned char buffer_content_7[] = {0x0f, 0xff, 0xee};
    buffer_t* buffer_7 = buffer_init(buffer_content_7, 24, LEFT);
    assert(buffer_equal(buffer_6, buffer_7, 24) == false);
    assert(buffer_equal(buffer_6, buffer_7, 23) == true);

    buffer_deinit(buffer_1);
    buffer_deinit(buffer_2);
    buffer_deinit(buffer_3);
    buffer_deinit(buffer_4);
    buffer_deinit(buffer_5);
    buffer_deinit(buffer_6);
    buffer_deinit(buffer_7);


    return TEST_SUCCESS;
}

int test_buffer_extract(){
    const unsigned char buffer_content_1[] = {0xf0, 0xff, 0xe0};
    buffer_t* buffer_1 = buffer_init(buffer_content_1, 24, RIGHT);
    buffer_t* buffer_extract_1 = buffer_extract(buffer_1, 4, 16, RIGHT);
    const unsigned char buffer_content_expected_1[] = {0x0f, 0xfe};
    assert(memcmp(buffer_extract_1->content, buffer_content_expected_1, 2) == 0);

    const unsigned char buffer_content_2[] = {0xf0, 0xff, 0xe0};
    buffer_t* buffer_2 = buffer_init(buffer_content_2, 24, LEFT);
    buffer_t* buffer_extract_2 = buffer_extract(buffer_2, 4, 16, RIGHT);
    const unsigned char buffer_content_expected_2[] = {0x0f, 0xfe, 0x00};
    assert(memcmp(buffer_extract_2->content, buffer_content_expected_2, 2) == 0);

    const unsigned char buffer_content_3[] = {0x0f, 0xff, 0xe0};
    buffer_t* buffer_3 = buffer_init(buffer_content_3, 20, LEFT);
    buffer_t* buffer_extract_3 = buffer_extract(buffer_3, 0, 16, RIGHT);
    const unsigned char buffer_content_expected_3[] = {0xff, 0xfe};
    assert(memcmp(buffer_extract_3->content, buffer_content_expected_3, 2) == 0);

    const unsigned char buffer_content_4[] = {0x0f, 0xff, 0xe0};
    buffer_t* buffer_4 = buffer_init(buffer_content_4, 20, LEFT);
    buffer_t* buffer_extract_4 = buffer_extract(buffer_4, 0, 20, RIGHT);
    const unsigned char buffer_content_expected_4[] = {0xff, 0xfe, 0x00};
    assert(memcmp(buffer_extract_4->content, buffer_content_expected_4, 3) == 0);

    const unsigned char buffer_content_5[] = {0x0f, 0xff, 0xe0};
    buffer_t* buffer_5 = buffer_init(buffer_content_5, 20, LEFT);
    buffer_t* buffer_extract_5 = buffer_extract(buffer_5, 0, 20, RIGHT);
    const unsigned char buffer_content_expected_5[] = {0xff, 0xfe, 0x00};
    assert(memcmp(buffer_extract_5->content, buffer_content_expected_5 , 3) == 0);

    const unsigned char buffer_content_6[] = {0xff, 0xfe, 0x00};
    buffer_t* buffer_6 = buffer_init(buffer_content_6, 20, RIGHT);
    buffer_t* buffer_extract_6 = buffer_extract(buffer_6, 0, 20, LEFT);
    const unsigned char buffer_content_expected_6[] = {0x0f, 0xff, 0xe0};
    assert(memcmp(buffer_extract_6->content, buffer_content_expected_6, 3) == 0);

    buffer_deinit(buffer_1);
    buffer_deinit(buffer_2);
    buffer_deinit(buffer_3);
    buffer_deinit(buffer_4);
    buffer_deinit(buffer_5);
    buffer_deinit(buffer_6);

    buffer_deinit(buffer_extract_1);
    buffer_deinit(buffer_extract_2);
    buffer_deinit(buffer_extract_3);
    buffer_deinit(buffer_extract_4);
    buffer_deinit(buffer_extract_5);
    buffer_deinit(buffer_extract_6);

    return TEST_SUCCESS;
}

int main() {
    assert(test_buffer_malloc_dealloc() == TEST_SUCCESS);
    assert(test_buffer_shift() == TEST_SUCCESS);
    assert(test_buffer_pad() == TEST_SUCCESS);
    assert(test_buffer_concat() == TEST_SUCCESS);
    assert(test_buffer_equal() == TEST_SUCCESS);
    assert(test_buffer_extract() == TEST_SUCCESS);
    return TEST_SUCCESS;
}
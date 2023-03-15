/**
 * @file buffer.c
 * @author Quentin Lampin (quentin.lampin@orange.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-08
 * 
 * @copyright Orange Copyright (c) 2023
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cschc.h"

/**
 * @brief returns the number of octets (bytes) necessary to store bit_length bits
 * 
 * @param[in] bit_length numbers of bits
 * @return uint16_t number of bytes
 */
uint16_t _byte_length(uint16_t bit_length){
    uint16_t byte_length;
    
    byte_length = (bit_length%8==0)? bit_length/8 : 1 + bit_length/8;
    return byte_length;
}
/**
 * @brief shifts bytes of \p in and outputs them in \p out .
 * 
 * @attention \p out must be initialized, malloc'ed  and its size matches this of \p in .
 *  
 * @param[out] out output.
 * @param[in] in  bytes to shift.
 * @param size size in bits of \p in . 
 * @param shift shift value, negative for left shift, positive for right shift.
 */
void _shift(unsigned char *out, unsigned char *in, int size, int shift){
    int left_shift;
    int right_shift;
    int cursor;

    if(shift < 0){
        left_shift = -shift%8;
        right_shift = (8-shift)%8;
        for(cursor=0; cursor < size-1; cursor++){
            out[cursor] = ((in[cursor] << left_shift) & 0xff) | (in[cursor+1] >> right_shift);
        }
        out[size-1] = (in[cursor] << left_shift) & 0xff;
    }else if(shift == 0){
        memcpy(out, in, size);
    }else if(shift > 0){
        right_shift = shift%8;
        left_shift = (8+shift)%8;
        out[0] = in[0] >> right_shift;
        for(cursor = 1; cursor < size; cursor++){
            out[cursor] = ((in[cursor-1] << left_shift) & 0xff) | (in[cursor] >> right_shift);
        }
    }
}

/**
 * @brief returns a malloc'ed buffer of \p length bits.
 * 
 * @param length length of buffer in bits.
 * @return buffer_t* 
 */
buffer_t* _buffer_malloc(uint8_t length){
    buffer_t* buffer;
    
    buffer = (buffer_t*) malloc(sizeof(buffer_t));
    buffer->length = length;
    buffer->byte_length = _byte_length(length);
    buffer->padding_length = 8*buffer->byte_length - buffer->length;
    buffer->content = (unsigned char*) malloc(buffer->byte_length);
    memset(buffer->content, 0x00, buffer->byte_length*sizeof(unsigned char));

    return buffer;
}

/**
 * @brief returns a malloc'ed buffer of \p length bits with \p content and \p padding.
 * 
 * @param content content
 * @param length  in bits
 * @param padding left, right
 * @return buffer_t* 
 */
buffer_t* buffer_init(const unsigned char* content, uint16_t length, padding_t padding){
    buffer_t* buffer;

    buffer = _buffer_malloc(length);
    buffer->padding = padding;
    memcpy(buffer->content, content, buffer->byte_length);

    return buffer;
}

/**
 * @brief free memory used by \p buffer
 * 
 * @param buffer 
 */
void buffer_deinit(buffer_t* buffer){
    free(buffer->content);
    free(buffer);
}

/**
 * @brief shift \p buffer by \p shift bits returned as a new buffer
 * 
 * @param[in] buffer buffer to shift
 * @param shift negative value for left shift, positive value for right shift
 * @return buffer_t* 
 */
buffer_t* buffer_shift(buffer_t* buffer, int shift){
    buffer_t* new_buffer;
    int new_length;
    int left_shift;
    int right_shift;
    int mask;
    
    new_length = buffer->length - shift ? buffer->length - shift : 0;

    new_buffer = _buffer_malloc(new_length);
    new_buffer->padding = buffer->padding;
        
    if(shift <= 0){
        /* left shift */
        switch (buffer->padding){
        case LEFT:
            left_shift = -shift%8;
            right_shift = (8-shift)%8;
            if(left_shift > buffer->padding_length){
                new_buffer->content[0] = (buffer->content[0] >> right_shift);
                _shift(new_buffer->content+1, buffer->content, buffer->byte_length, shift);
            }else{
                _shift(new_buffer->content, buffer->content, buffer->byte_length, shift);
            }
            break;
        case RIGHT:
            memcpy(new_buffer->content, buffer->content, buffer->byte_length);
            break;
        } 
    }else{
        /* right shift*/
        switch (buffer->padding){
        case LEFT:
            _shift(new_buffer->content, buffer->content, buffer->byte_length, shift%8);
            break;
        case RIGHT:
            mask = (0xff << new_buffer->padding_length) & 0xff;
            memcpy(new_buffer->content, buffer->content, new_buffer->byte_length-1);
            new_buffer->content[new_buffer->byte_length-1] = buffer->content[new_buffer->byte_length-1] & mask;
            break;
        }
    }
    
    return new_buffer;
}

/**
 * @brief change the padding of \p buffer as new buffer
 * 
 * @param[in] buffer 
 * @param padding 
 * @return buffer_t* 
 */
buffer_t* buffer_pad(buffer_t* buffer, padding_t padding){
    buffer_t* new_buffer;
    int shift;
    
    if(padding == buffer->padding || buffer->padding_length%8 == 0){
        new_buffer = buffer_init(buffer->content, buffer->length, padding);
    }else{
        new_buffer = _buffer_malloc(buffer->length);
        shift = padding == LEFT ? buffer->padding_length: -buffer->padding_length;
        _shift(new_buffer->content, buffer->content, buffer->byte_length, shift);
        new_buffer->padding = padding;
    }
    return new_buffer;
}

/**
 * @brief concatenated \p buffer_1 and \p buffer_2, returned as new buffer
 * 
 * @param[in] buffer_1 
 * @param[in] buffer_2 
 * @return buffer_t* 
 */
buffer_t* buffer_concat(buffer_t* buffer_1, buffer_t* buffer_2){
    buffer_t* new_buffer;
    int new_length;
    int buffer_1_shift;
    int buffer_2_offset;
    int buffer_2_shift;
    int cursor;
    unsigned char saved;

    new_length = buffer_1->length + buffer_2->length;
    new_buffer = _buffer_malloc(new_length);
    new_buffer->padding = buffer_1->padding;

    switch (new_buffer->padding){
    case LEFT:
        buffer_1_shift = new_buffer->padding_length - buffer_1->padding_length;
        _shift(new_buffer->content, buffer_1->content, buffer_1->byte_length, buffer_1_shift);
        break;
    case RIGHT:
        buffer_1_shift = 0;
        memcpy(new_buffer->content, buffer_1->content, buffer_1->byte_length);
        break;
    }

    cursor = (new_buffer->padding_length + buffer_1->length)/8;
    buffer_2_offset = (new_buffer->padding_length + buffer_1->length)%8;        
    buffer_2_shift = buffer_2_offset - buffer_2->padding_length;

    if(buffer_2_offset == 0){
        _shift(new_buffer->content+cursor, buffer_2->content, buffer_2->byte_length, buffer_2_shift);
    }else{
        saved = new_buffer->content[cursor];
        _shift(new_buffer->content+cursor, buffer_2->content, buffer_2->byte_length, buffer_2_shift);
        new_buffer->content[cursor] |= saved;
    }
    return new_buffer;
}

/**
 * @brief returns true if \p buffer_1 and \p buffer_2 first \p length bits are equal. 
 * If \p length is 0, all bits are considered.
 * 
 * @param[in] buffer_1 
 * @param[in] buffer_2 
 * @param length in bits. If 0 (zero), compare all bits.
 * @return true 
 * @return false 
 */
bool buffer_equal(buffer_t* buffer_1, buffer_t* buffer_2, int length){
    buffer_t* buffer_1_padded;
    buffer_t* buffer_2_padded;

    int byte_length;
    uint8_t mask;

    buffer_1_padded = buffer_pad(buffer_1, RIGHT);
    buffer_2_padded = buffer_pad(buffer_2, RIGHT);
    
    if(length == 0){
        return (buffer_1_padded->length == buffer_2_padded->length) 
                && memcmp(buffer_1_padded->content, buffer_2_padded->content, buffer_1->byte_length);
    }

    if(length > buffer_1->length || length > buffer_2->length){
        return false;
    }

    byte_length = _byte_length(length);
    mask = ~(0xff >> (8 - length%8));

    if(memcmp(buffer_1_padded->content, buffer_2_padded->content, byte_length-1) != 0){
        return false;
    }
    return (buffer_1_padded->content[byte_length-1] & mask) == (buffer_2_padded->content[byte_length-1] & mask);
}

/**
 * @brief extracts \p bit_length bits from the \p bit_start 's bit of the buffer and returns the extract as a new buffer
 * 
 * @param[in] buffer buffer that contains bits to extract
 * @param bit_start index of the first bit to extract
 * @param bit_length number of bits to extract
 * @param padding padding of the output buffer
 * @return buffer_t* 
 */
buffer_t* buffer_extract(buffer_t* buffer, uint16_t bit_start, uint16_t bit_length, padding_t padding){
    buffer_t* buffer_extracted;
    unsigned char* buffer_temp;
    uint16_t bit_start_absolute;
    uint16_t byte_start;
    uint16_t byte_end;
    uint16_t area_of_interest_byte_length;
    int shift;

    buffer_extracted = _buffer_malloc(bit_length);

    bit_start_absolute = (buffer->padding == LEFT)? bit_start+buffer->padding_length : bit_start;
    byte_start = bit_start_absolute/8;
    byte_end = (bit_start_absolute + bit_length - 1)/8;

    area_of_interest_byte_length = byte_end - byte_start +1;
    buffer_temp = (unsigned char*)malloc(area_of_interest_byte_length*sizeof(char));
    shift = (padding == LEFT)? buffer_extracted->padding_length - bit_start_absolute % 8 : - bit_start_absolute % 8;
    _shift(buffer_temp, buffer->content + byte_start, area_of_interest_byte_length, shift);
    memcpy(buffer_extracted->content, buffer_temp, buffer_extracted->byte_length);
    free(buffer_temp);
    
    return buffer_extracted;
}
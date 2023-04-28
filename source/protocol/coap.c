#include "coap.h"

header_descriptor_t* _parse_options(buffer_t* buffer){
    header_descriptor_t* header_options;
    header_descriptor_t* header_option;
    header_descriptor_t* header_extended;

    field_descriptor_t* fd_option_delta;
    field_descriptor_t* fd_option_length;
    field_descriptor_t* fd_option_delta_extended;
    field_descriptor_t* fd_option_length_extended;
    field_descriptor_t* fd_option_value;
    field_descriptor_t* fd_payload_marker;

    buffer_t* buffer_option_delta;
    buffer_t* buffer_option_length;
    buffer_t* buffer_option_delta_extended;
    buffer_t* buffer_option_length_extended;
    buffer_t* buffer_option_value;
    buffer_t* buffer_payload_marker;

    uint16_t option_offset;
    uint16_t option_value_length;
    uint16_t cursor;

    header_options = header_descriptor_init(COAP_HEADER_ID, 0);
    cursor = 0;

    while(cursor < buffer->length && buffer->content[cursor/8] != COAP_PAYLOAD_MARKER_VALUE){
        option_offset = 0;
        option_value_length = 0;

        buffer_option_delta = buffer_extract(buffer, cursor, 4, LEFT);
        buffer_option_length = buffer_extract(buffer, cursor + 4, 4, LEFT);

        fd_option_delta = field_descriptor_init(COAP_OPTION_DELTA, buffer_option_delta);
        fd_option_length = field_descriptor_init(COAP_OPTION_LENGTH, buffer_option_length);

        option_value_length += 8 * (uint8_t)buffer_option_length->content[0];

        header_option = header_descriptor_init(COAP_HEADER_ID, 2, fd_option_delta, fd_option_length);
        option_offset += 8;

        switch(buffer_option_delta->content[0]){
            case COAP_OPTION_DELTA_EXTENDED_8BITS:
            buffer_option_delta_extended = buffer_extract(buffer, cursor + option_offset, 8, LEFT);
            fd_option_delta_extended = field_descriptor_init(COAP_OPTION_DELTA_EXTENDED, buffer_option_delta_extended);
            header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_option_delta_extended);
            header_option = header_descriptor_concat(COAP_HEADER_ID, 2, header_option, header_extended);
            break;
            case COAP_OPTION_DELTA_EXTENDED_16BITS:
            buffer_option_delta_extended = buffer_extract(buffer, cursor + option_offset, 16, LEFT);
            fd_option_delta_extended = field_descriptor_init(COAP_OPTION_DELTA_EXTENDED, buffer_option_delta_extended);
            header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_option_delta_extended);
            header_option = header_descriptor_concat(COAP_HEADER_ID, 2, header_option, header_extended);
            break;
        }
        option_offset = header_option->length;

        switch(buffer_option_length->content[0]){
            case COAP_OPTION_LENGTH_EXTENDED_8BITS:
            buffer_option_length_extended = buffer_extract(buffer, cursor + option_offset, 8, LEFT);
            option_value_length += 8 * (uint8_t)buffer_option_length_extended->content[0];
            fd_option_length_extended = field_descriptor_init(COAP_OPTION_DELTA_EXTENDED, buffer_option_length_extended);
            header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_option_length_extended);
            header_option = header_descriptor_concat(COAP_HEADER_ID, 2, header_option, header_extended);
            option_offset += 8;
            break;
            case COAP_OPTION_LENGTH_EXTENDED_16BITS:
            buffer_option_delta_extended = buffer_extract(buffer, cursor + option_offset, 16, LEFT);
            option_value_length += 8 * *((uint16_t*)buffer_option_length_extended->content);
            fd_option_delta_extended = field_descriptor_init(COAP_OPTION_LENGTH_EXTENDED, buffer_option_delta_extended);
            header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_option_length_extended);
            header_option = header_descriptor_concat(COAP_HEADER_ID, 2, header_option, header_extended);
            option_offset += 16;
            break;
        }
        buffer_option_value = buffer_extract(buffer, cursor + option_offset, option_value_length, LEFT);
        fd_option_value = field_descriptor_init(COAP_OPTION_VALUE, buffer_option_value);
        header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_option_value);
        header_option = header_descriptor_concat(COAP_HEADER_ID, 2, header_option, header_extended);

        header_options = header_descriptor_concat(COAP_HEADER_ID, 2, header_options, header_option);
        option_offset += option_value_length;
        cursor += option_offset;
    }
    if(cursor < buffer->length && buffer->content[cursor/8] == COAP_PAYLOAD_MARKER_VALUE){
        buffer_payload_marker = buffer_extract(buffer, cursor, 8, LEFT);
        fd_payload_marker = field_descriptor_init(COAP_PAYLOAD_MARKER, buffer_payload_marker);
        header_extended = header_descriptor_init(COAP_HEADER_ID, 1, fd_payload_marker);
        header_options = header_descriptor_concat(COAP_HEADER_ID, 2, header_options, header_extended);
    }

    return header_options;
}

/**
 * @brief parse a CoAP header and returns its header descriptor
 * 
 * @param[in] buffer CoAP header, assumed to be right-padded
 * @return header_descriptor_t* 
 */
header_descriptor_t* parse_coap_header(buffer_t* buffer){

    buffer_t* buffer_version;
    buffer_t* buffer_type;
    buffer_t* buffer_token_length;
    buffer_t* buffer_code;
    buffer_t* buffer_message_id;
    buffer_t* buffer_token;
    buffer_t* buffer_options;
    
    field_descriptor_t* fd_version;
    field_descriptor_t* fd_type;
    field_descriptor_t* fd_token_length;
    field_descriptor_t* fd_code;
    field_descriptor_t* fd_message_id;
    field_descriptor_t* fd_token;
    

    header_descriptor_t* header_options;
    header_descriptor_t* header_descriptor;
    header_descriptor_t* header_descriptor_part;


    uint16_t token_length;

    buffer_version = buffer_extract(buffer, 0, 2, LEFT);
    buffer_type = buffer_extract(buffer, 2, 2, LEFT);
    buffer_token_length = buffer_extract(buffer, 4, 4, LEFT);
    buffer_code = buffer_extract(buffer, 8, 8, LEFT);
    buffer_message_id = buffer_extract(buffer, 16, 16, LEFT);

    fd_version = field_descriptor_init(COAP_VERSION, buffer_version);
    fd_type = field_descriptor_init(COAP_TYPE, buffer_type);
    fd_token_length = field_descriptor_init(COAP_TOKEN_LENGTH, buffer_token_length);
    fd_code = field_descriptor_init(COAP_CODE, buffer_code);
    fd_message_id = field_descriptor_init(COAP_MESSAGE_ID, buffer_message_id);

    header_descriptor = header_descriptor_init(COAP_HEADER_ID, 5, 
        fd_version,
        fd_type,
        fd_token_length,
        fd_code,
        fd_message_id
    );

    token_length = 8*(uint8_t)buffer_token_length->content[0];

    if(token_length > 0){
        buffer_token = buffer_extract(buffer, header_descriptor->length, token_length, LEFT);
        fd_token = field_descriptor_init(COAP_TOKEN, buffer_token);
        header_descriptor_part = header_descriptor_init(COAP_HEADER_ID, 1, fd_token);
        header_descriptor = header_descriptor_concat(COAP_HEADER_ID, 2, header_descriptor, header_descriptor_part);
    }

    buffer_options = buffer_extract(buffer, header_descriptor->length, 0, LEFT);
    header_options = _parse_options(buffer_options);
    buffer_deinit(buffer_options);

    header_descriptor = header_descriptor_concat(COAP_HEADER_ID, 2, header_descriptor, header_options);


    return header_descriptor;
}



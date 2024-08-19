#include "headers.h"

/* ********************************************************************** */
/*                         CoAP Header functions                          */
/* ********************************************************************** */

void set_coap_option_variable_length(uint16_t*      variable_length,
                                     const uint8_t* field,
                                     const size_t   field_byte_len,
                                     const uint16_t sid) {
  if (sid == SID_COAP_OPTION_DELTA || sid == SID_COAP_OPTION_LENGTH) {
    *variable_length = (uint16_t) *field;
  } else if (sid == SID_COAP_OPTION_DELTA_EXTENDED ||
             sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
    switch (*variable_length) {
      case 13:
        *variable_length = (uint16_t) field[0] + 13;
        break;

      case 14:
        *variable_length = ((uint16_t) ((field[0] << 8) | field[1])) + 269;
        break;

      default:  // less than 13 or 15. 15 is reserved.
        *variable_length = 0;
        break;
    }
  } else {
    *variable_length = 0;
  }
}

/* ********************************************************************** */

size_t get_coap_option_bit_length(const uint16_t option, const uint16_t sid) {
  if (sid == SID_COAP_OPTION_DELTA_EXTENDED ||
      sid == SID_COAP_OPTION_LENGTH_EXTENDED) {
    switch (option) {
      case 13:
        return 8;

      case 14:
        return 16;

      default:  // less than 13 or 15. 15 is reserved.
        return 0;
    }
  } else if (sid == SID_COAP_OPTION_VALUE) {
    return 8 * option;
  } else {  // Might not reach this statement
    return 0;
  }
}
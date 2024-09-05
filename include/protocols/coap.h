/**
 * @file coap.h
 * @author Corentin Banier and Quentin Lampin
 * @brief CoAP implementation in CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _COAP_H_
#define _COAP_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief CoAP IETF SCHC Yang model identifiers.
 */
#define COAP_PREFIX "fid-coap"
#define COAP_VERSION COAP_PREFIX "-version"
#define COAP_TYPE COAP_PREFIX "-type"
#define COAP_TOKEN_LENGTH COAP_PREFIX "-tkl"
#define COAP_CODE COAP_PREFIX "-code"
#define COAP_MESSAGE_ID COAP_PREFIX "-mid"
#define COAP_TOKEN COAP_PREFIX "-token"

// Defined for this library (Not in current SCHC Yang Model)
#define COAP_OPTION_DELTA COAP_PREFIX "-option-delta"
#define COAP_OPTION_LENGTH COAP_PREFIX "-option-length"
#define COAP_OPTION_VALUE COAP_PREFIX "-option-value"
#define COAP_PAYLOAD_MARKER COAP_PREFIX "-payload-marker"

/**
 * @brief CoAP IETF SCHC Yang model SIDs.
 *
 * @details Not standardized yet.
 */
#define SID_COAP_VERSION 5055
#define SID_COAP_TYPE 5054
#define SID_COAP_TKL 5052
#define SID_COAP_CODE 5023
#define SID_COAP_MESSAGE_ID 5026
#define SID_COAP_TOKEN 5053

// Defined for this library (Not in current SCHC Yang Model)
#define SID_COAP_OPTION_DELTA 5136
#define SID_COAP_OPTION_DELTA_EXTENDED 5137
#define SID_COAP_OPTION_LENGTH 5138
#define SID_COAP_OPTION_LENGTH_EXTENDED 5139
#define SID_COAP_OPTION_VALUE 5140
#define SID_COAP_PAYLOAD_MARKER 5141

/**
 * @brief Sets the CoAP Variable-Length.
 *
 * @details See Section 3 of CoAP RFC 7252.
 *
 * @param variable_length Pointer to the Variable-Length value to fill.
 * @param field Pointer to the CoAP Option (Option Delta or Option Length).
 * @param field_byte_len Byte length of the Field.
 * @param sid SID of the CoAP Option.
 */
void set_coap_option_variable_length(uint16_t*      variable_length,
                                     const uint8_t* field,
                                     const size_t   field_byte_len,
                                     const uint16_t sid);

/**
 * @brief Gets the CoAP Option Extended bit length.
 *
 * @details See Section 3.1 of CoAP RFC 7252.
 *
 * @param option The CoAP Option (Option Delta or Option Length).
 * @param sid SID corresponding to the CoAP Option.
 * @return The bit length of the CoAP Option Extended (Delta or Length).
 */
size_t get_coap_option_bit_length(const uint16_t option, const uint16_t sid);

#endif  // _COAP_H_
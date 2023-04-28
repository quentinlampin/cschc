/**
 * @brief  COAP header parser for cSCHC
 * @author Quentin Lampin (quentin.lampin@orange.com)
 * @brief Parser for the CoAP protocol header as defined in RFC7252 [1].
 *
 *        Note: The case of CoAP in the context of SCHC is a odd one.
 *        Compressing `Option Values` requires **interpreting** CoAP fields,
 *        specifically option deltas to identify options types.
 *
 *        This implementation strays away from SCHC philosophy: instead of
 *        interpreting fields values, i.e. provide absolute option types and
 *        option values, it only exposes the `raw` fields: option deltas and
 *        option lengths. The rationale for this choice is that options inside
 *        a CoAP packet are not expected to vary too much for a device, therefore
 *        rendering the need to access to option semantic useless.
 * 
 *       [1] "RFC7252: The Constrained Application Protocol (CoAP)", Z. Shelby et al.
 * 
 * @copyright Copyright (c) Orange 2023
 * 
 */

#include "cschc.h"

#define COAP_HEADER_ID                 "CoAP"
#define COAP_BASE_FIELD_COUNT          6

#define   COAP_VERSION                 COAP_HEADER_ID"Version"
#define   COAP_TYPE                    COAP_HEADER_ID"Type"
#define   COAP_TOKEN_LENGTH            COAP_HEADER_ID"Token Length"
#define   COAP_CODE                    COAP_HEADER_ID"Code"
#define   COAP_MESSAGE_ID              COAP_HEADER_ID"Message ID"
#define   COAP_TOKEN                   COAP_HEADER_ID"Token"
#define   COAP_PAYLOAD_MARKER          COAP_HEADER_ID"Payload Marker"
#define   COAP_OPTION_DELTA            COAP_HEADER_ID"Option Delta"
#define   COAP_OPTION_LENGTH           COAP_HEADER_ID"Option Length"
#define   COAP_OPTION_DELTA_EXTENDED   COAP_HEADER_ID"Option Delta Extended"
#define   COAP_OPTION_LENGTH_EXTENDED  COAP_HEADER_ID"Option Length Extended"
#define   COAP_OPTION_VALUE            COAP_HEADER_ID"Option Value"

#define   COAP_OPTION_DELTA_EXTENDED_8BITS      0x0d
#define   COAP_OPTION_DELTA_EXTENDED_16BITS     0x0e
#define   COAP_OPTION_LENGTH_EXTENDED_8BITS     0x0d
#define   COAP_OPTION_LENGTH_EXTENDED_16BITS    0x0e

#define   COAP_PAYLOAD_MARKER_VALUE             0xff


header_descriptor_t* parse_coap_header(buffer_t* buffer);
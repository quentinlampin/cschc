/**
 * @brief  UDP header parser for cSCHC
 * @author Quentin Lampin (quentin.lampin@orange.com)
 * @brief Parser for the UDP protocol header as defined in RFC768 [1].
 * 
 *        [1] "RFC768: User Datagram Protocol", J. Postel
 * @version 0.1
 * @date 2023-03-20
 * 
 * @copyright Copyright (c) Orange 2023
 * 
 */
#include "cschc.h"

#define UDP_HEADER_ID           "UDP"
#define UDP_SOURCE_PORT         UDP_HEADER_ID"Source Port"
#define UDP_DESTINATION_PORT    UDP_HEADER_ID"Destination Port"
#define UDP_LENGTH              UDP_HEADER_ID"Length"
#define UDP_CHECKSUM            UDP_HEADER_ID"Checksum"

#define UDP_HEADER_LENGTH 64

header_descriptor_t* parse_udp_header(buffer_t* buffer);
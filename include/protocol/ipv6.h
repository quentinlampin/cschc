/**
 * @brief IPv6 header parser

Parser for the IPv6 protocol header as defined in RFC8200 [1].

Note 1: Hop by Hop Options, Routing header parsing is not implemented yet.
Note 2: Fragment header parsing is not implemented as fragmentation and reassembly
        are handled by SCHC-RF.
Note 3: Authentication and Encapsulating Security payload parsing is not implemented yet.

[1] "RFC8200: Internet Protocol, Version 6 (IPv6) Specification", S. Deering et al.
 * 
 */

#define IPV6_HEADER_ID "IPv6"
#define IPV6_VERSION            IPV6_HEADER_ID":Version"
#define IPV6_TRAFFIC_CLASS      IPV6_HEADER_ID":Traffic Class"
#define IPV6_FLOW_LABEL         IPV6_HEADER_ID":Flow Label"
#define IPV6_PAYLOAD_LENGTH     IPV6_HEADER_ID":Version"
#define IPV6_NEXT_HEADER        IPV6_HEADER_ID":Next Header"
#define IPV6_HOP_LIMIT          IPV6_HEADER_ID":Hop Limit"
#define IPV6_SRC_ADDRESS        IPV6_HEADER_ID":Source Address"
#define IPV6_DST_ADDRESS        IPV6_HEADER_ID":Destination Address"

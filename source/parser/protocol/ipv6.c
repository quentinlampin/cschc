#include "parser/protocol/ipv6.h"

/* ********************************************************************** */

void unpack_ver_tc_fl(uint8_t* version, uint8_t* traffic_class,
                      uint32_t* flow_label, const uint32_t ver_tc_fl) {
  *version =
      (ver_tc_fl >> (IPV6_TRAFFIC_CLASS_LENGTH + IPV6_FLOW_LABEL_LENGTH)) &
      0x0f;
  *traffic_class = (ver_tc_fl >> (IPV6_FLOW_LABEL_LENGTH)) & 0xff;
  *flow_label    = ver_tc_fl & 0xfffff;
}
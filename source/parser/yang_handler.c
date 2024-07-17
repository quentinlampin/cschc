#include "parser/yang_handler.h"

/* ********************************************************************** */

uint16_t get_fid_by_protocol_and_offset(const protocol_t protocol,
                                        const int        offset) {
  for (int i = 0; i < sizeof(field_fid_mappings); i++) {
    if (field_fid_mappings[i].protocol == protocol &&
        field_fid_mappings[i].offset == offset) {
      return field_fid_mappings[i].fid;
    }
  }
  return 0;
}

/* ********************************************************************** */

int get_protocol_and_offset_by_fid(protocol_t* protocol, int* offset,
                                   const uint16_t fid) {
  for (int i = 0; i < sizeof(field_fid_mappings); i++) {
    if (field_fid_mappings[i].fid == fid) {
      *protocol = field_fid_mappings[i].protocol;
      *offset   = field_fid_mappings[i].offset;
      return 1;
    }
  }
  return 0;
}
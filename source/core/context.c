#include "context.h"
#include "utils/memory.h"

/* ********************************************************************** */

int get_cardinal_compute_entries(const rule_descriptor_t *rule_descriptor,
                                 const uint8_t           *context,
                                 const size_t             context_byte_len) {
  int                      card_compute_entries;
  int                      index_rule_field_descriptor;
  rule_field_descriptor_t *rule_field_descriptor;

  // Init
  card_compute_entries        = 0;
  index_rule_field_descriptor = 0;

  // Allocate rule_field_descriptor from the pool
  rule_field_descriptor =
      (rule_field_descriptor_t *) pool_alloc(sizeof(rule_field_descriptor_t));

  while (index_rule_field_descriptor <
         rule_descriptor->card_rule_field_descriptor) {
    // Get Rule Field Descriptor
    get_rule_field_descriptor(
        rule_field_descriptor, index_rule_field_descriptor,
        rule_descriptor->offset, context, context_byte_len);

    // Check if the Rule Field Descriptor CDA is CDA_COMPUTE
    if (rule_field_descriptor->cda == CDA_COMPUTE) {
      card_compute_entries++;
    }

    // Move to the next Rule Field Descriptor index
    index_rule_field_descriptor++;
  }

  // Deallocate rule_field_descriptor from the pool
  pool_dealloc(rule_field_descriptor, sizeof(rule_field_descriptor_t));

  return card_compute_entries;
}
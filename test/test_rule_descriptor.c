#include "core/rule_descriptor.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ********************************************************************** */

void test_get_rule_descriptor(void) {
  const uint8_t context[] = {
      // Context
      0, 4, 0, 10, 0, 21, 0, 30, 0, 35,

      // Rule Descriptor
      0, 0, 4, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff,                                         // Rule Descriptor 0
      1, 2, 3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // Rule Descriptor 1
      2, 0, 1, 0xff, 0xff,                          // Rule Descriptor 2
      3, 1, 0,                                      // Rule Descriptor 3

      // Rule Field Descriptor
      // ...
  };
  const size_t context_byte_len = sizeof(context);

  rule_descriptor_t rule_descriptor;

  /**
   * @brief Get Rule Descriptor 0
   *
   * @details :
   * - Offset                     = 10
   * - ID                         = 0
   * - Nature                     = 0 (NATURE_COMPRESSION)
   * - Card Rule Field Descriptor = 4
   */
  assert(get_rule_descriptor(&rule_descriptor, 0, context, context_byte_len));
  assert(rule_descriptor.offset == 10);
  assert(rule_descriptor.id == 0);
  assert(rule_descriptor.nature == NATURE_COMPRESSION);
  assert(rule_descriptor.card_rule_field_descriptor == 4);

  /**
   * @brief Get Rule Descriptor 3
   *
   * @details :
   * - Offset                     = 35
   * - ID                         = 3
   * - Nature                     = 1 (NATURE_NO_COMPRESSION)
   * - Card Rule Field Descriptor = 0
   */
  assert(get_rule_descriptor(&rule_descriptor, 3, context, context_byte_len));
  assert(rule_descriptor.offset == 35);
  assert(rule_descriptor.id == 3);
  assert(rule_descriptor.nature == NATURE_NO_COMPRESSION);
  assert(rule_descriptor.card_rule_field_descriptor == 0);

  /**
   * @brief Get Rule Descriptor 1
   *
   * @details :
   * - Offset                     = 21
   * - ID                         = 1
   * - Nature                     = 2 (NATURE_FRAGMENTATION)
   * - Card Rule Field Descriptor = 3
   */
  assert(get_rule_descriptor(&rule_descriptor, 1, context, context_byte_len));
  assert(rule_descriptor.offset == 21);
  assert(rule_descriptor.id == 1);
  assert(rule_descriptor.nature == NATURE_FRAGMENTATION);
  assert(rule_descriptor.card_rule_field_descriptor == 3);

  /**
   * @brief Get Rule Descriptor 2
   *
   * @details :
   * - Offset                     = 30
   * - ID                         = 2
   * - Nature                     = 0 (NATURE_COMPRESSION)
   * - Card Rule Field Descriptor = 0
   */
  assert(get_rule_descriptor(&rule_descriptor, 2, context, context_byte_len));
  assert(rule_descriptor.offset == 30);
  assert(rule_descriptor.id == 2);
  assert(rule_descriptor.nature == NATURE_COMPRESSION);
  assert(rule_descriptor.card_rule_field_descriptor == 1);

  /**
   * @brief Get Rule Descriptor 9
   *
   * @details As there are only 4 Rule Descriptors, getting the 10th one is not
   * possible.
   */
  assert(!get_rule_descriptor(&rule_descriptor, 9, context, context_byte_len));
}

/* ********************************************************************** */

int main(void) {
  test_get_rule_descriptor();

  printf("All tests passed!\n");

  return 0;
}
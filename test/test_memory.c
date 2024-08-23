#include "core/rule_field_descriptor.h"
#include "utils/memory.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ********************************************************************** */

void test_alloc_dealloc(void) {
  /**
   * @brief Test if the pool allocation does not generate a NULL pointer.
   * Additionally, verify the pool->used size in parallel with allocation.
   */

  size_t last_size = 0;
  assert(pool->used == last_size);

  int *data1 = (int *) pool_alloc(sizeof(int) * 10);
  assert(data1 != NULL);
  assert(pool->used == sizeof(int) * 10 + last_size);
  last_size += sizeof(int) * 10;

  char *data2 = (char *) pool_alloc(sizeof(char) * 20);
  assert(data2 != NULL);
  assert(pool->used == sizeof(char) * 20 + last_size);
  last_size += sizeof(char) * 20;

  uint8_t *data3 = (uint8_t *) pool_alloc(sizeof(uint8_t) * 100);
  assert(data3 != NULL);
  assert(pool->used == sizeof(uint8_t) * 100 + last_size);
  last_size += sizeof(uint8_t) * 100;

  rule_field_descriptor_t *data4 =
      (rule_field_descriptor_t *) pool_alloc(sizeof(rule_field_descriptor_t));
  assert(data4 != NULL);
  assert(pool->used == sizeof(rule_field_descriptor_t) + last_size);
  last_size += sizeof(rule_field_descriptor_t);

  uint8_t *data5 = (uint8_t *) pool_alloc(sizeof(uint8_t) * 300);
  assert(data5 != NULL);
  assert(pool->used == sizeof(uint8_t) * 300 + last_size);
  last_size += sizeof(uint8_t) * 300;

  pool_dealloc(data5, sizeof(uint8_t) * 300);
  assert(pool->used == last_size - sizeof(uint8_t) * 300);
  last_size -= sizeof(uint8_t) * 300;

  pool_dealloc(data4, sizeof(rule_field_descriptor_t));
  assert(pool->used == last_size - sizeof(rule_field_descriptor_t));
  last_size -= sizeof(rule_field_descriptor_t);

  pool_dealloc(data3, sizeof(uint8_t) * 100);
  assert(pool->used == last_size - sizeof(uint8_t) * 100);
  last_size -= sizeof(uint8_t) * 100;

  pool_dealloc(data2, sizeof(char) * 20);
  assert(pool->used == last_size - sizeof(char) * 20);
  last_size -= sizeof(char) * 20;

  pool_dealloc(data1, sizeof(int) * 10);
  assert(pool->used == last_size - sizeof(int) * 10);
  last_size -= sizeof(int) * 10;

  assert(pool->used == 0);  // Empty
}

/* ********************************************************************** */

void test_segmentation_fault(void) {
  /**
   * @brief Try to allocate more space than it's possible.
   *
   * @details POOL_SIZE is the maximum size of the pool->memory.
   *
   * - First we allocate all the space for the pointer data.
   * - Second trying to allocate one more byte. As it's not possible, the
   * pointer is NULL.
   */
  char *data = (char *) pool_alloc(sizeof(char) * POOL_SIZE);
  assert(data != NULL);
  assert(pool->used == sizeof(char) * POOL_SIZE);

  char *seg_fault = (char *) pool_alloc(sizeof(char) * 1);
  assert(seg_fault == NULL);
  assert(pool->used != sizeof(char) + sizeof(char) * POOL_SIZE);
  assert(pool->used == sizeof(char) * POOL_SIZE);
}

/* ********************************************************************** */

int main(void) {
  init_memory_pool();

  test_alloc_dealloc();
  test_segmentation_fault();

  destroy_memory_pool();

  printf("All tests passed!\n");

  return 0;
}
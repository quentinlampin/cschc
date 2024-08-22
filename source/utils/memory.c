#include "utils/memory.h"

#include <stdlib.h>
#include <string.h>

/* ********************************************************************** */

memory_pool_t *pool = NULL;

/* ********************************************************************** */

memory_pool_t *create_memory_pool(void) {
  memory_pool_t *_pool = (memory_pool_t *) malloc(sizeof(memory_pool_t));
  if (_pool == NULL) {
    return NULL;
  }

  _pool->memory = (uint8_t *) malloc(POOL_SIZE);
  if (_pool->memory == NULL) {
    free(_pool);
    return NULL;
  }

  // Init
  _pool->used = 0;
  _pool->size = POOL_SIZE;
  memset(_pool->memory, 0x00, POOL_SIZE);

  return _pool;
}

/* ********************************************************************** */

void destroy_memory_pool(void) {
  if (pool) {
    free(pool->memory);
    free(pool);
  }
}

/* ********************************************************************** */

void init_memory_pool(void) { pool = create_memory_pool(); }

/* ********************************************************************** */

void *pool_alloc(const size_t size) {
  void *ptr;

  if (!pool) {
    return NULL;
  }

  if (pool->used + size > pool->size) {
    return NULL;
  }

  ptr = pool->memory + pool->used;
  pool->used += size;

  return ptr;
}

/* ********************************************************************** */

void pool_dealloc(void *ptr, const size_t size) {
  if (ptr == NULL || pool == NULL) {
    return;
  }

  pool->used -= size;
  ptr = NULL;
  memset(pool->memory + pool->used, 0x00, size);
}
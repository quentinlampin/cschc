#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include <stdint.h>

#define POOL_SIZE (1024 * 1024)  // 1MB

/**
 * @brief Struct which defines a memory pool.
 */
typedef struct {
  uint8_t *memory;  // Dynamically allocated space
  size_t   used;    // Current memory used
  size_t   size;    // Total amount of memory allocated
} memory_pool_t;

/**
 * @brief Creates a memory pool object.
 *
 * @return A pointer to a dynamically allocated memory_pool_t of size POOL_SIZE.
 */
memory_pool_t *create_memory_pool(void);

/**
 * @brief Pointer to track the memory_pool_t.
 */
extern memory_pool_t *pool;

/**
 * @brief Frees the pool.
 */
void destroy_memory_pool(void);

/**
 * @brief Initializes the pool.
 */
void init_memory_pool(void);

/**
 * @brief Allocates an object from the pool according to its size.
 *
 * @param size The size of the object to allocate.
 * @return Pointer to the allocated object.
 */
void *pool_alloc(size_t size);

/**
 * @brief Deallocates a pointer given its size.
 *
 * @param ptr Pointer to deallocate.
 * @param size The size of the pointer.
 */
void pool_dealloc(void *ptr, size_t size);

#endif  // _MEMORY_H_
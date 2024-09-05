/**
 * @file memory.h
 * @author Corentin Banier
 * @brief Memory Pool implementation for CSCHC.
 * @version 1.0
 * @date 2024-08-26
 *
 * @copyright Copyright (c) Orange 2024. This project is released under the MIT
 * License.
 *
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include <stdint.h>

#define POOL_SIZE (1024 * 1024)  // 1MB

/**
 * @brief Struct that defines a memory pool.
 *
 * @details This memory pool implementation is not fragmentation-friendly.
 * Indeed, no realignment is performed. Therefore, allocation/deallocation from
 * the pool must be done in the correct order to prevent data fragmentation. As
 * the pool is mainly used internally, we should avoid this issue.
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
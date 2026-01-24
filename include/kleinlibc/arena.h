#ifndef KLEINLIBC_ARENA_H
#define KLEINLIBC_ARENA_H

#include <stdint.h>
#include <stdlib.h>

typedef struct 
{
    uint8_t *buffer;
    size_t size;
    size_t alignment;
    size_t offset;
} arena_t;

/**
 * @brief Initialize the arena
 * 
 * @param handle Pointer to the arena handle
 * @param buffer Pointer to a statically allocated array
 * @param buffer_size Size of buffer in bytes
 */
void arena_init(arena_t *handle, void *buffer, size_t buffer_size);

/**
 * @brief Explicitly set the alignment to use in the arena
 * 
 * @note The maximum platform-specific alignment is used by default
 * 
 * @param handle Pointer to the arena handle
 * @param bytes Desired byte alignment (must be a power of 2)
 * @return true If alignment succesfully configured
 * @return false If alignment is not a power of 2
 */
bool arena_set_alignment(arena_t *handle, size_t bytes); // alignment must be a power of 2

/**
 * @brief Allocated a number of bytes
 * 
 * @param handle Pointer to the arena handle
 * @param size Number of bytes to allocate
 * @return void* Pointer to the allocated memory. NULL if memory could not be allocated.
 */
void *arena_alloc(arena_t *handle, size_t size);

/**
 * @brief Free all memory from the arena, allowing it to be reused immediately
 * 
 * @param handle Pointer to the arena handle
 */
void arena_free_all(arena_t *handle);

/**
 * @brief Get the number of remaining bytes available in the buffer (after subtracting the required padding)
 * 
 * @param handle Pointer to the arena handle
 * @return size_t Number of bytes available for allocation
 */
size_t arena_available(arena_t *handle);

#endif
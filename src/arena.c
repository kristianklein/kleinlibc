#include <stddef.h>
#include <stdalign.h>
#include <stdbool.h>
#include "kleinlibc/arena.h"

#define PLATFORM_ALIGNMENT __alignof__(max_align_t)
#define ALIGN_UP(ptr, align) \
    (((uintptr_t)(ptr) + ((align) - 1)) & ~((uintptr_t)((align) - 1)))
#define IS_POWER_OF_TWO(n) (((n) != 0) && (((n) & ((n) - 1)) == 0))

void arena_init(arena_t *handle, void *buffer, size_t buffer_size)
{
    handle->buffer = (uint8_t*)buffer;
    handle->size = buffer_size;
    handle->offset = 0U;
    handle->alignment = PLATFORM_ALIGNMENT;
}

bool arena_set_alignment(arena_t *handle, size_t bytes)
{
    if (!IS_POWER_OF_TWO(bytes))
    {
        return false;
    }

    handle->alignment = bytes;
    return true;
}

void *arena_alloc(arena_t *handle, size_t size)
{
    // Ensure alignment
    uintptr_t cursor = (uintptr_t)(handle->buffer + handle->offset);
    uintptr_t aligned_cursor = ALIGN_UP(cursor, handle->alignment);
    size_t aligned_offset = (aligned_cursor - (uintptr_t)handle->buffer);

    // Check if enough space available
    if (size > handle->size || aligned_offset + size > handle->size)
    {
        // Not enough memory!
        return (void*)0;
    }

    // Bump offset
    handle->offset = aligned_offset + size;

    // Return pointer
    return (void*)(handle->buffer + aligned_offset);
}

void arena_free_all(arena_t *handle)
{
    handle->offset = 0U;
}

size_t arena_available(arena_t *handle)
{
    uintptr_t cursor = (uintptr_t)(handle->buffer + handle->offset);
    uintptr_t aligned_cursor = ALIGN_UP(cursor, handle->alignment);
    size_t aligned_offset = (aligned_cursor - (uintptr_t)handle->buffer);

    return handle->size - aligned_offset;
}
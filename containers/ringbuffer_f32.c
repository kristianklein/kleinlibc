#include "containers/ringbuffer_f32.h"

void ringbuffer_f32_create(ringbuffer_f32_t* handle, float* buffer, uint32_t length)
{
    handle->buffer = buffer;
    handle->capacity = length;
    handle->size = 0;
    handle->head = 0;
}

bool ringbuffer_f32_is_full(ringbuffer_f32_t* handle)
{
    return handle->size == handle->capacity;
}

void ringbuffer_f32_add(ringbuffer_f32_t* handle, float element)
{
    handle->buffer[handle->head] = element;
    handle->head = (handle->head + 1) % handle->capacity;
    
    if (handle->size < handle->capacity)
    {
        handle->size++;
    }
}

int16_t ringbuffer_f32_get(ringbuffer_f32_t* handle, uint32_t index)
{
    if (index >= handle->capacity)
    {
        return 0;
    }

    uint32_t local_index;

    if (index + 1 > handle->head) // handle wrap around
    {
        local_index = handle->head + handle->capacity - index - 1;
    }
    else
    {
        local_index = handle->head - index - 1;
    }

    return handle->buffer[local_index];
}
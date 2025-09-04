/*
Buffer intended to be used in filtering application, e.g. to store previous input samples.

The buffer is should work with signed 16 bit integers (covers ADCs up to 15-bit) and provide
the following functions:
- create (user supplies a statically allocated buffer and its length, which should be equal to the amount of filter taps)
- add (add an element, e.g. a sample, to the buffer. When full, the oldest element is overwritten)
- get (get element from any index. Index 0 is the newest element)
- is_full (to let the user know that enough samples have been pushed, to avoid start-up issues)
*/

#include "containers/ringbuffer_i16.h"

void ringbuffer_i16_create(ringbuffer_i16_t* handle, int16_t* buffer, uint32_t length)
{
    handle->buffer = buffer;
    handle->capacity = length;
    handle->size = 0;
    handle->head = 0;
}

bool ringbuffer_i16_is_full(ringbuffer_i16_t* handle)
{
    return handle->size == handle->capacity;
}

void ringbuffer_i16_add(ringbuffer_i16_t* handle, int16_t element)
{
    handle->buffer[handle->head] = element;
    handle->head = (handle->head + 1) % handle->capacity;
    handle->size++;
}

int16_t ringbuffer_i16_get(ringbuffer_i16_t* handle, uint32_t index)
{
    if (index >= handle->capacity)
    {
        return INT16_MIN;
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
#include <string.h>
#include <stdatomic.h>
#include "kleinlibc/containers/queue.h"
#include "kleinlibc/utils/sim_printf.h"

void queue_create(queue_t* handle, void* buffer, size_t length, size_t element_size)
{
    handle->buffer = buffer;
    handle->capacity = length;
    handle->element_size = element_size;
    atomic_init(&handle->size, 0);
    handle->head = 0;
    handle->tail = 0;
}

bool queue_push(queue_t* handle, const void* element)
{    
    if (queue_is_full(handle))
    {
        return false;
    }

    memcpy(handle->buffer + (handle->tail * handle->element_size), element, handle->element_size);
    handle->tail = (handle->tail + 1) % handle->capacity;
    atomic_fetch_add(&handle->size, 1);
    
    return true;
}


bool queue_pop(queue_t* handle, void* destination)
{
    if (queue_is_empty(handle))
    {
        return false;
    }
    
    memcpy(destination, handle->buffer + (handle->head * handle->element_size), handle->element_size);
    handle->head = (handle->head + 1) % handle->capacity;
    atomic_fetch_sub(&handle->size, 1);
    
    return true;
}

bool queue_peek(queue_t* handle, void* destination)
{
    if (queue_is_empty(handle))
    {
        return false;
    }

    memcpy(destination, handle->buffer + (handle->head * handle->element_size), handle->element_size);
    return true;
}
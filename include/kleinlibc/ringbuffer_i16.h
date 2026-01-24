#ifndef KLEINLIB_RINGBUFFER_I16_H
#define KLEINLIB_RINGBUFFER_I16_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct 
{
    int16_t* buffer;
    size_t size, capacity, head;
} ringbuffer_i16_t;

void ringbuffer_i16_create(ringbuffer_i16_t* handle, int16_t* buffer, size_t length);
void ringbuffer_i16_add(ringbuffer_i16_t* handle, int16_t element);
int16_t ringbuffer_i16_get(ringbuffer_i16_t* handle, size_t index);
static inline bool ringbuffer_i16_is_full(ringbuffer_i16_t* handle) {return handle->size == handle->capacity;}

#endif
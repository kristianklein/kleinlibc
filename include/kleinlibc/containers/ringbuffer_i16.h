#ifndef KLEINLIB_RINGBUFFER_I16_H
#define KLEINLIB_RINGBUFFER_I16_H

#include <stdint.h>
#include <stdbool.h>

typedef struct ringbuffer_i16_struct ringbuffer_i16_t;

void ringbuffer_i16_create(ringbuffer_i16_t* handle, int16_t* buffer, uint32_t length);
bool ringbuffer_i16_is_full(ringbuffer_i16_t* handle);
void ringbuffer_i16_add(ringbuffer_i16_t* handle, int16_t element);
int16_t ringbuffer_i16_get(ringbuffer_i16_t* handle, uint32_t index);

struct ringbuffer_i16_struct
{
    int16_t* buffer;
    uint32_t size, capacity, head;
};

#endif
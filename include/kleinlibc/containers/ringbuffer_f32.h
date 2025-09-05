#ifndef KLEINLIB_RINGBUFFER_F32_H
#define KLEINLIB_RINGBUFFER_F32_H

#include <stdint.h>
#include <stdbool.h>

typedef struct ringbuffer_f32_struct ringbuffer_f32_t;

void ringbuffer_f32_create(ringbuffer_f32_t* handle, float* buffer, uint32_t length);
bool ringbuffer_f32_is_full(ringbuffer_f32_t* handle);
void ringbuffer_f32_add(ringbuffer_f32_t* handle, float element);
int16_t ringbuffer_f32_get(ringbuffer_f32_t* handle, uint32_t index);

struct ringbuffer_f32_struct
{
    float* buffer;
    uint32_t size, capacity, head;
};

#endif
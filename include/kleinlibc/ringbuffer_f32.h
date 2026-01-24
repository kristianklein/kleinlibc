#ifndef KLEINLIB_RINGBUFFER_F32_H
#define KLEINLIB_RINGBUFFER_F32_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float* buffer;
    size_t size, capacity, head;
} ringbuffer_f32_t;

void ringbuffer_f32_create(ringbuffer_f32_t* handle, float* buffer, size_t length);
void ringbuffer_f32_add(ringbuffer_f32_t* handle, float element);
int16_t ringbuffer_f32_get(ringbuffer_f32_t* handle, size_t index);
static inline bool ringbuffer_f32_is_full(ringbuffer_f32_t* handle) {return handle->size == handle->capacity;}

#endif
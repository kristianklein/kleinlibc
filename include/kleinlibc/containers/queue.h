#ifndef KLEINLIBC_QUEUE_H
#define KLEINLIBC_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    void *buffer;
    size_t capacity;
    size_t element_size;
    size_t size;
    size_t head;
    size_t tail;
} queue_t;

void queue_create(queue_t* handle, void* buffer, size_t length, size_t element_size);
bool queue_push(queue_t* handle, const void* element);
bool queue_pop(queue_t* handle, void* destination);
bool queue_peek(queue_t* handle, void* destination);
static inline bool queue_is_empty(queue_t* handle) {return 0 == handle->size;}
static inline bool queue_is_full(queue_t* handle) {return handle->capacity == handle->size;}
static inline size_t queue_size(queue_t* handle) {return handle->size;}

#endif
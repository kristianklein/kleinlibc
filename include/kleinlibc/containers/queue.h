#ifndef KLEINLIBC_QUEUE_H
#define KLEINLIBC_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct queue_struct queue_t;

void queue_create(queue_t* handle, void* buffer, uint32_t length, uint32_t element_size);
bool queue_is_empty(queue_t* handle);
bool queue_push(queue_t* handle, void* element);
bool queue_pop(queue_t* handle, void* destination);
bool queue_is_full(queue_t* handle);
uint32_t queue_size(queue_t* handle);
bool queue_peek(queue_t* handle, void* destination);

struct queue_struct
{
    void *buffer;
    uint32_t capacity;
    uint32_t element_size;
    uint32_t size;
    uint32_t head;
    uint32_t tail;
};

#endif
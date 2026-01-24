#ifndef KLEINLIBC_SEMAPHORE_H
#define KLEINLIBC_SEMAPHORE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct semaphore_struct semaphore_t;

void semaphore_init(semaphore_t* sem, uint32_t initial_value, uint32_t max_value);
bool semaphore_take(semaphore_t* sem);
void semaphore_give(semaphore_t* sem);

struct semaphore_struct
{
    volatile uint32_t value;
    uint32_t max;
};


#endif

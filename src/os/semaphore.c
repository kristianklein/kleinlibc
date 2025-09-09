#include "kleinlibc/os/semaphore.h"

void semaphore_init(semaphore_t* sem, uint32_t initial_value, uint32_t max_value)
{
    atomic_init(&sem->value, initial_value);
    sem->max = max_value;
}

bool semaphore_take(semaphore_t* sem)
{
    uint32_t current_value = atomic_load(&sem->value);
    if (current_value > 0)
    {
        if (atomic_compare_exchange_weak(&sem->value, &current_value, current_value - 1))
        {
            return true;
        }
    }
    
    return false;
}

void semaphore_give(semaphore_t* sem)
{
    uint32_t current_value = atomic_load(&sem->value);
    if (current_value < sem->max)
    {
        atomic_compare_exchange_weak(&sem->value, &current_value, current_value + 1);        
    }
}
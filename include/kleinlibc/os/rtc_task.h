#ifndef KLEINLIBC_RTC_TASK_H
#define KLEINLIBC_RTC_TASK_H

#include <stdint.h>
#include "kleinlibc/os/semaphore.h"

typedef void (*rtc_task_init_func)(void*);
typedef void (*rtc_task_run_func)();

typedef struct {
    rtc_task_init_func init;
    rtc_task_run_func run;
    void* args;
    uint32_t period_ms;
    uint32_t next_run_ms;
    semaphore_t* signal;
} rtc_task_t;


#endif

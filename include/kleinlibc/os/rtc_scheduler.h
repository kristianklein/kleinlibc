#ifndef KLEINLIBC_RTC_SCHEDULER_H
#define KLEINLIBC_RTC_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "kleinlibc/os/rtc_task.h"

typedef uint32_t (*get_tick_ms_func)();

void rtc_scheduler_init(rtc_task_t* task_buffer[], uint32_t length, get_tick_ms_func get_tick_ms);

// The task will ALWAYS run on the set period (unless 0). If both period and signal is specified, the task will ALSO run when the signal semaphore is set.
bool rtc_scheduler_add_task(rtc_task_t* task, rtc_task_init_func init_func, rtc_task_run_func run_func, void* args, uint32_t period_ms, semaphore_t* event_signal);
void rtc_scheduler_run(); // never returns

#endif

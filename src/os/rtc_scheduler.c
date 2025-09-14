#include "kleinlibc/os/rtc_scheduler.h"

static rtc_task_t** tasks;
static uint32_t max_tasks;
static uint32_t task_count;
static bool tasks_initialized;
static get_tick_ms_func get_tick;

void rtc_scheduler_init(rtc_task_t* task_buffer[], uint32_t length, get_tick_ms_func get_tick_ms)
{
    tasks = task_buffer;
    max_tasks = length;
    task_count = 0;
    tasks_initialized = false;
    get_tick = get_tick_ms;
}

bool rtc_scheduler_add_task(rtc_task_t* task, rtc_task_init_func init_func, rtc_task_run_func run_func, void* args, uint32_t period_ms, semaphore_t* event_signal)
{
    if (task_count >= max_tasks) return false;

    task->init = init_func;
    task->run = run_func;
    task->args = args;
    task->period_ms = period_ms;
    task->next_run_ms = 0;
    task->signal = event_signal;
    tasks[task_count++] = task;
    
    return true;
}

void rtc_scheduler_run()
{
	uint32_t start_time = get_tick();
    // Initialize tasks
    for (uint32_t i = 0; i < task_count; i++)
    {
        rtc_task_t* task = tasks[i];
        task->init(task->args);
        task->next_run_ms = start_time; // all tasks start simultaneously
    }

    while(1)
    {
        for (uint32_t i = 0; i < task_count; i++)
        {
            rtc_task_t* task = tasks[i];

            if (task->period_ms != 0 && task->next_run_ms <= get_tick())
            {
            	task->next_run_ms += task->period_ms;
            	task->run();
            }
            else if (task->signal && semaphore_take(task->signal))
            {
            	// do not update next_run_ms, period should be phase-locked
            	task->run();
            }
        }   

        // TODO: Put MCU to sleep if no tasks are running
    }
}

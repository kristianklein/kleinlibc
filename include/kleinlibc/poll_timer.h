#ifndef KLEINLIBC_POLL_TIMER_H
#define KLEINLIBC_POLL_TIMER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct poll_timer poll_timer_t;

typedef enum
{
    POLL_TIMER_SINGLE_SHOT,
    POLL_TIMER_CONTINUOUS
} poll_timer_mode;

void poll_timer_init(poll_timer_t *handle, uint32_t (*get_milliseconds)(), poll_timer_mode mode, uint32_t timeout_ms);
uint32_t poll_timer_elapsed(poll_timer_t *handle);
void poll_timer_start(poll_timer_t *handle);
void poll_timer_stop(poll_timer_t *handle);
void poll_timer_reset(poll_timer_t *handle);
bool poll_timer_expired(poll_timer_t *handle);

struct poll_timer
{  
    uint32_t timeout;
    poll_timer_mode mode;
    uint32_t (*get_ms)();
    uint32_t start_time;
    uint32_t accumulated_time;
    uint32_t accumulated_interval_time;
    bool is_running;
};


#endif
#include "kleinlibc/os/poll_timer.h"

void poll_timer_init(poll_timer_t *handle, uint32_t (*get_milliseconds)(), poll_timer_mode mode, uint32_t timeout)
{
    handle->get_ms = get_milliseconds;
    handle->mode = mode;
    handle->timeout = timeout;
    handle->accumulated_time = 0;
    handle->accumulated_interval_time = 0;
    handle->is_running = false;
}

uint32_t poll_timer_elapsed(poll_timer_t *handle)
{
    uint32_t now = handle->get_ms();
    uint32_t elapsed = handle->accumulated_time;

    if (handle->is_running) {
        elapsed += (now - handle->start_time);
    }

    if (POLL_TIMER_CONTINUOUS == handle->mode) {
        elapsed += handle->accumulated_interval_time;
    }

    return elapsed;
}

void poll_timer_start(poll_timer_t *handle)
{
    if (handle->is_running) return;

    handle->is_running = true;
    handle->start_time = handle->get_ms();
}

void poll_timer_stop(poll_timer_t *handle)
{
    if (!handle->is_running) return;
    
    handle->is_running = false;
    uint32_t now = handle->get_ms();
    
    if (POLL_TIMER_SINGLE_SHOT == handle->mode)
    {
        handle->accumulated_time += now - handle->start_time;
    }
    else
    {
        handle->accumulated_interval_time += now - handle->start_time;
    }
}

void poll_timer_reset(poll_timer_t *handle)
{
    handle->is_running = false;
    handle->accumulated_time = 0;
    handle->accumulated_interval_time = 0;
}

bool poll_timer_expired(poll_timer_t *handle)
{
    if (handle->timeout == 0) return false;

    if (POLL_TIMER_SINGLE_SHOT == handle->mode)
    {
        return poll_timer_elapsed(handle) >= handle->timeout;
    }
    
    uint32_t now = handle->get_ms();
    
    if (handle->is_running)
    {
        uint32_t total_interval_time = now - handle->start_time + handle->accumulated_interval_time;

        if (total_interval_time >= handle->timeout)
        {
            uint32_t residual_time;

            if (handle->accumulated_interval_time >= handle->timeout)
            {
                handle->accumulated_interval_time -= handle->timeout;
                handle->accumulated_time += handle->timeout;
                residual_time = total_interval_time - handle->timeout;
            }
            else
            {
                handle->accumulated_time += handle->accumulated_interval_time;
                residual_time = total_interval_time - handle->accumulated_interval_time - handle->timeout;
                handle->accumulated_interval_time = 0;
            }

            handle->start_time = now - residual_time;
            
            return true;
        }
    }
    else
    {
        if (handle->accumulated_interval_time >= handle->timeout)
        {
            handle->accumulated_interval_time -= handle->timeout;
            handle->accumulated_time += handle->timeout;

            return true;
        }
    }

    return false;
}

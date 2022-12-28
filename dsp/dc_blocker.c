#include "dc_blocker.h"

void dc_blocker_init(dc_blocker_t *handle, float blocking_factor)
{
    handle->blocking_factor = blocking_factor;
    handle->gain = (1 + blocking_factor) / 2;
    handle->prev_input = 0;
    handle->prev_output = 0;
}

float dc_blocker_process(dc_blocker_t *handle, float current_input)
{
    float output = handle->gain * (current_input - handle->prev_input) +
                   handle->blocking_factor * handle->prev_output;

    handle->prev_input = current_input;
    handle->prev_output = output;
    return output;
}
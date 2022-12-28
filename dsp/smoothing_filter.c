#include "smoothing_filter.h"

void smoothing_filter_init(smoothing_filter_t *handle, float smoothing_factor, float initial_output)
{
    handle->smoothing_factor = smoothing_factor;
    handle->prev_output = initial_output;
}

float smoothing_filter_process(smoothing_filter_t *handle, float current_input)
{
    float output = current_input * handle->smoothing_factor +
                   handle->prev_output * (1 - handle->smoothing_factor);

    handle->prev_output = output;
    return output;
}
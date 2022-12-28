#ifndef KLEINLIBC_SMOOTHING_FILTER_H
#define KLEINLIBC_SMOOTHING_FILTER_H

typedef struct
{
    float smoothing_factor;
    float prev_output;
} smoothing_filter_t;

/**
 * @brief Initialize smoothing filter.
 *
 * @param handle Pointer to an instance of `smoothing_filter_t`.
 * @param smoothing_factor A floating point number between 0 and 1 that determines how much smoothing to apply.
 *  Smaller values apply more smoothing.
 * @param initial_output The initial output of the filter. If the approximate DC value of the input is known,
 *  the initial value of the filter can be set equal to this value to reduce settling time at startup.
 *  Otherwise, this value can simply be set to 0.
 */
void smoothing_filter_init(smoothing_filter_t *handle, float smoothing_factor, float initial_output);

/**
 * @brief Process the next input value.
 *
 * @param handle Pointer to an instance of `smoothing_filter_t`.
 * @param current_input The input value to process.
 * @return float The filtered output value.
 */
float smoothing_filter_process(smoothing_filter_t *handle, float current_input);

#endif
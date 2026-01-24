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
 */
void smoothing_filter_init(smoothing_filter_t *handle, float smoothing_factor);

/**
 * @brief Process the next input value.
 *
 * @param handle Pointer to an instance of `smoothing_filter_t`.
 * @param current_input The input value to process.
 * @return float The filtered output value.
 */
float smoothing_filter_process(smoothing_filter_t *handle, float current_input);

#endif
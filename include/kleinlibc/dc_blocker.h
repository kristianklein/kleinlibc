#ifndef KLEINLIBC_DC_BLOCKER_H
#define KLEINLIBC_DC_BLOCKER_H

typedef struct
{
    float blocking_factor;
    float gain;
    float prev_input;
    float prev_output;
} dc_blocker_t;

/**
 * @brief Initialize the DC blocker.
 * 
 * @param handle Pointer to an instance of `dc_blocker_t`.
 * @param blocking_factor A floating point number between 0 and 1 that
 *  determines how close to DC the cutoff frequency should be. A value close to (but less than)
 *  1 will result in a cutoff frequency very close to DC but with a slow response.
 *  Setting the blocking factor lower will result in a faster response but will attenuate more
 *  low frequency content. A value between 0.9 and 1 is reasonably in most cases.
 */
void dc_blocker_init(dc_blocker_t *handle, float blocking_factor);

/**
 * @brief Process the next input value.
 * 
 * @param handle Pointer to an instance of `dc_blocker_t`.
 * @param current_input The input value to process.
 * @return float The filtered output value.
 */
float dc_blocker_process(dc_blocker_t *handle, float current_input);

#endif
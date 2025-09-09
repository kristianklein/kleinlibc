#include "kleinlibc/dsp/fir_i16.h"

static const uint8_t NUM_SHIFTS = 15;

void fir_i16_init(fir_i16_t* handle, ringbuffer_i16_t* sample_buffer, const float* coeffs, int16_t* coeff_buffer, uint16_t num_taps)
{
    handle->sample_buffer = sample_buffer;
    handle->coeff_buffer = coeff_buffer;
    handle->taps = num_taps;

    for (uint32_t i = 0; i < handle->taps; i++)
    {
        // Zero-initialize sample buffer
        ringbuffer_i16_add(handle->sample_buffer, 0);

        // Scale coefficients to Q15 (multiply, no bitshift on float) and round to nearest int
        float scaled = coeffs[i] * INT16_MAX;
        handle->coeff_buffer[i] = scaled >= 0 ? (int16_t)(scaled + 0.5) : (int16_t)(scaled - 0.5);
    }
}

int16_t fir_i16_process(fir_i16_t* handle, int16_t input)
{
    ringbuffer_i16_add(handle->sample_buffer, input);

    int64_t y = 0;
    for (uint16_t i = 0; i < handle->taps; i++)
    {
        y += (int32_t)handle->coeff_buffer[i] * ringbuffer_i16_get(handle->sample_buffer, i);
    }

    return y >> NUM_SHIFTS;
}

void fir_i16_reset(fir_i16_t* handle)
{
    for (uint32_t i = 0; i < handle->taps; i++)
    {
        // Zero out sample buffer
        ringbuffer_i16_add(handle->sample_buffer, 0);
    }
}

void fir_i16_process_block(fir_i16_t* handle, const int16_t* input_block, int16_t* output_block, uint32_t num_samples)
{
    for (uint32_t i = 0; i < num_samples; i++)
    {
        output_block[i] = fir_i16_process(handle, input_block[i]);
    }
}
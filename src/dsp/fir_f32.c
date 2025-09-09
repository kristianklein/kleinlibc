#include "kleinlibc/dsp/fir_f32.h"

void fir_f32_init(fir_f32_t* handle, ringbuffer_f32_t* sample_buffer, const float* coeffs, uint16_t num_taps)
{
    handle->sample_buffer = sample_buffer;
    handle->coeff_buffer = coeffs;
    handle->taps = num_taps;

    for (uint32_t i = 0; i < handle->taps; i++)
    {
        // Zero-initialize sample buffer
        ringbuffer_f32_add(handle->sample_buffer, 0);
    }
}

int16_t fir_f32_process(fir_f32_t* handle, float input)
{
    ringbuffer_f32_add(handle->sample_buffer, input);

    double y = 0;
    for (uint16_t i = 0; i < handle->taps; i++)
    {
        y += handle->coeff_buffer[i] * ringbuffer_f32_get(handle->sample_buffer, i);
    }

    return y;
}

void fir_f32_reset(fir_f32_t* handle)
{
    for (uint32_t i = 0; i < handle->taps; i++)
    {
        // Zero out sample buffer
        ringbuffer_f32_add(handle->sample_buffer, 0);
    }
}

void fir_f32_process_block(fir_f32_t* handle, const float* input_block, float* output_block, uint32_t num_samples)
{
    for (uint32_t i = 0; i < num_samples; i++)
    {
        output_block[i] = fir_f32_process(handle, input_block[i]);
    }
}
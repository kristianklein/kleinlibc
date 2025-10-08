#ifndef KLEINLIB_FIR_F32_H
#define KLEINLIB_FIR_F32_H

#include "kleinlibc/containers/ringbuffer_f32.h"

typedef struct {
    ringbuffer_f32_t* sample_buffer;
    const float* coeff_buffer;
    uint16_t taps;
} fir_f32_t;

void fir_f32_init(fir_f32_t* handle, ringbuffer_f32_t* sample_buffer, const float* coeffs, uint16_t num_taps);
int16_t fir_f32_process(fir_f32_t* handle, float input);
void fir_f32_process_block(fir_f32_t* handle, const float * const input_block, float * const output_block, size_t num_samples);
void fir_f32_reset(fir_f32_t* handle);


#endif
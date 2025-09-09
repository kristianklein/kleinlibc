#ifndef KLEINLIB_FIR_F32_H
#define KLEINLIB_FIR_F32_H

#include "kleinlibc/containers/ringbuffer_f32.h"

typedef struct fir_f32_struct fir_f32_t;

void fir_f32_init(fir_f32_t* handle, ringbuffer_f32_t* sample_buffer, const float* coeffs, uint16_t num_taps);
int16_t fir_f32_process(fir_f32_t* handle, float input);
void fir_f32_reset(fir_f32_t* handle);
void fir_f32_process_block(fir_f32_t* handle, const float* input_block, float* output_block, uint32_t num_samples);

struct fir_f32_struct
{
    ringbuffer_f32_t* sample_buffer;
    const float* coeff_buffer;
    uint16_t taps;
};

#endif
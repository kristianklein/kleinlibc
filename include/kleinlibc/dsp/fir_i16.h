#ifndef KLEINLIB_FIR_I16_H
#define KLEINLIB_FIR_I16_H

#include "kleinlibc/containers/ringbuffer_i16.h"

typedef struct {
    ringbuffer_i16_t *sample_buffer;
    int16_t *coeff_buffer;
    uint16_t taps;
} fir_i16_t;

void fir_i16_init(fir_i16_t* handle, ringbuffer_i16_t *sample_buffer, const float *coeffs, int16_t *coeff_buffer, uint16_t num_taps);
int16_t fir_i16_process(fir_i16_t* handle, int16_t input);
void fir_i16_process_block(fir_i16_t* handle, const int16_t * const input_block, int16_t * const output_block, size_t num_samples);
void fir_i16_reset(fir_i16_t* handle);

#endif
#include "gtest/gtest.h"
#include <stdint.h>
#include <iostream>

extern "C"
{
    #include "kleinlibc/dsp/fir_f32.h"
}

static constexpr unsigned NUM_TAPS = 10;
static constexpr unsigned NUM_SAMPLES = 100;

// 10 tap low-pass unity gain FIR filter with normalized cutoff at 0.05 relative to Fs (i.e.
// 5 Hz @ 100 Hz sample rate).
static float lowpass[NUM_TAPS] = {0.011982297073578192, 0.032593697188218536, 0.08880972436230841, 0.1590336085502214, 0.20758067282567344, 0.20758067282567344, 0.15903360855022144, 0.08880972436230844, 0.032593697188218536, 0.011982297073578192};

class FirF32Test : public testing::Test
{
protected:
    fir_f32_t fir;
    ringbuffer_f32_t ringbuffer;
    float coeff_buffer[NUM_TAPS];
    float sample_buffer[NUM_TAPS];

    void SetUp() override
    {
        ringbuffer_f32_create(&ringbuffer, sample_buffer, NUM_TAPS);
        fir_f32_init(&fir, &ringbuffer, lowpass, NUM_TAPS);
    }
};

TEST_F(FirF32Test, ZeroInitialized)
{
    float output = fir_f32_process(&fir, 0);
    ASSERT_EQ(output, 0);
}

TEST_F(FirF32Test, ZeroInitializedNonZeroInput)
{
    float input = 1337;
    float output = fir_f32_process(&fir, input);
    
    // all other input samples are zero initialized.
    // process() function handles right-shifting the sum of products
    int16_t expected_output = input * lowpass[0];
    ASSERT_EQ(output, expected_output);
}

TEST_F(FirF32Test, SmoothsOutStepFunction)
{
    float output, prev_output = 0;
    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        output = fir_f32_process(&fir, INT16_MAX);
        ASSERT_TRUE(output >= prev_output);
        
        //std::cout << output << std::endl;

        // Check that output converges to INT16_MAX (with some margin for error in gain due to coefficient rounding)
        prev_output = output;
        if (output > INT16_MAX-5 && prev_output == output && i > 0)
        {
            SUCCEED();
            return;
        }
    }

    FAIL(); // Output should have converged by now
}

TEST_F(FirF32Test, MultipleSamples)
{
    float input = 1337;
    float single_outputs[NUM_SAMPLES];

    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        single_outputs[i] = fir_f32_process(&fir, input);
    }

    fir_f32_reset(&fir);

    float output_block[NUM_SAMPLES];
    float input_block[NUM_SAMPLES];

    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        input_block[i] = input;
    }

    fir_f32_process_block(&fir, input_block, output_block, NUM_SAMPLES);

    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        ASSERT_EQ(single_outputs[i], output_block[i]);
    }
}
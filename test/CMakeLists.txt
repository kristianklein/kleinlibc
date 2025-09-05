set(TEST_SOURCES
    queue_test.cpp
    ringbuffer_i16_test.cpp
    ringbuffer_f32_test.cpp
    fir_i16_test.cpp
    fir_f32_test.cpp
)

target_sources(kleinlibc_test PRIVATE ${TEST_SOURCES})

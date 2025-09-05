set(TEST_SOURCES
    fir_i16_test.cpp
    queue_test.cpp
    ringbuffer_i16_test.cpp
)

target_sources(kleinlibc_test PRIVATE ${TEST_SOURCES})

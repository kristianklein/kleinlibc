#include <gtest/gtest.h>

extern "C"
{
    #include "kleinlibc/containers/ringbuffer_f32.h"
}


static constexpr unsigned BUFFER_LENGTH = 4;

class RingBufferF32Test : public testing::Test
{
protected:
    ringbuffer_f32_t ringbuffer;
    float buffer[BUFFER_LENGTH];

    void SetUp() override
    {
        ringbuffer_f32_create(&ringbuffer, buffer, BUFFER_LENGTH);
    }

};

TEST_F(RingBufferF32Test, NotFullWhenCreated)
{
    ASSERT_FALSE(ringbuffer_f32_is_full(&ringbuffer));
}

TEST_F(RingBufferF32Test, FullWhenAddedLengthElements)
{
    float elem = 42;

    for (unsigned i = 0; i < BUFFER_LENGTH; i++)
    {
        ringbuffer_f32_add(&ringbuffer, elem);
    }

    ASSERT_TRUE(ringbuffer_f32_is_full(&ringbuffer));
}

TEST_F(RingBufferF32Test, ElementCanBeRetrievedByIndex)
{
    float elem = 42;
    ringbuffer_f32_add(&ringbuffer, elem);
    
    float retrieved = ringbuffer_f32_get(&ringbuffer, 0);
    ASSERT_EQ(retrieved, elem);
}

TEST_F(RingBufferF32Test, MultipleElementsAreIndexedNewestFirst)
{
    float elem = 42;
    ringbuffer_f32_add(&ringbuffer, elem);
    elem = 43;
    ringbuffer_f32_add(&ringbuffer, elem);
    
    float retrieved = ringbuffer_f32_get(&ringbuffer, 0);
    ASSERT_EQ(retrieved, 43);
    retrieved = ringbuffer_f32_get(&ringbuffer, 1);
    ASSERT_EQ(retrieved, 42);
}

TEST_F(RingBufferF32Test, OverwritesOldestElementWhenFull)
{
    // Fills buffer
    for (int16_t i = 0; i < BUFFER_LENGTH; i++)
    {
        ringbuffer_f32_add(&ringbuffer, i);
    }

    // Add one extra element, oldest (i.e. 0) should be discarded
    ringbuffer_f32_add(&ringbuffer, BUFFER_LENGTH);

    // Newest element at lowest index
    ASSERT_EQ(ringbuffer_f32_get(&ringbuffer, 0), BUFFER_LENGTH);
    ASSERT_EQ(ringbuffer_f32_get(&ringbuffer, 1), BUFFER_LENGTH - 1);
    ASSERT_EQ(ringbuffer_f32_get(&ringbuffer, 2), BUFFER_LENGTH - 2);
    ASSERT_EQ(ringbuffer_f32_get(&ringbuffer, 3), BUFFER_LENGTH - 3);
}
#include <gtest/gtest.h>

extern "C"
{
    #include "containers/ringbuffer_i16.h"
}


static constexpr unsigned BUFFER_LENGTH = 4;

class RingBufferI16Test : public testing::Test
{
protected:
    ringbuffer_i16_t ringbuffer;
    int16_t buffer[BUFFER_LENGTH];

    void SetUp() override
    {
        ringbuffer_i16_create(&ringbuffer, buffer, BUFFER_LENGTH);
    }

};

TEST_F(RingBufferI16Test, NotFullWhenCreated)
{
    ASSERT_FALSE(ringbuffer_i16_is_full(&ringbuffer));
}

TEST_F(RingBufferI16Test, FullWhenAddedLengthElements)
{
    int16_t elem = 42;

    for (unsigned i = 0; i < BUFFER_LENGTH; i++)
    {
        ringbuffer_i16_add(&ringbuffer, elem);
    }

    ASSERT_TRUE(ringbuffer_i16_is_full(&ringbuffer));
}

TEST_F(RingBufferI16Test, ElementCanBeRetrievedByIndex)
{
    int16_t elem = 42;
    ringbuffer_i16_add(&ringbuffer, elem);
    
    int16_t retrieved = ringbuffer_i16_get(&ringbuffer, 0);
    ASSERT_EQ(retrieved, elem);
}

TEST_F(RingBufferI16Test, MultipleElementsAreIndexedNewestFirst)
{
    int16_t elem = 42;
    ringbuffer_i16_add(&ringbuffer, elem);
    elem = 43;
    ringbuffer_i16_add(&ringbuffer, elem);
    
    int16_t retrieved = ringbuffer_i16_get(&ringbuffer, 0);
    ASSERT_EQ(retrieved, 43);
    retrieved = ringbuffer_i16_get(&ringbuffer, 1);
    ASSERT_EQ(retrieved, 42);
}

TEST_F(RingBufferI16Test, OverwritesOldestElementWhenFull)
{
    // Fills buffer
    for (int16_t i = 0; i < BUFFER_LENGTH; i++)
    {
        ringbuffer_i16_add(&ringbuffer, i);
    }

    // Add one extra element, oldest (i.e. 0) should be discarded
    ringbuffer_i16_add(&ringbuffer, BUFFER_LENGTH);

    // Newest element at lowest index
    ASSERT_EQ(ringbuffer_i16_get(&ringbuffer, 0), BUFFER_LENGTH);
    ASSERT_EQ(ringbuffer_i16_get(&ringbuffer, 1), BUFFER_LENGTH - 1);
    ASSERT_EQ(ringbuffer_i16_get(&ringbuffer, 2), BUFFER_LENGTH - 2);
    ASSERT_EQ(ringbuffer_i16_get(&ringbuffer, 3), BUFFER_LENGTH - 3);
}
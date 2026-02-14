#include "gtest/gtest.h"
#include <stdint.h>
#include <iostream>

extern "C"
{
    #include "kleinlibc/framing.h"
}

class FramingTest : public testing::Test
{
protected:

    void SetUp() override
    {

    }
};

TEST_F(FramingTest, HdlcFrameWithNoEscape)
{
    uint8_t input[] = {1, 2, 3};
    uint8_t expected_output[] = {0x7E, 1, 2, 3, 0x7E};

    uint8_t output[8];
    size_t out_len = frame_hdlc(output, 7, input, 3);

    ASSERT_EQ(5, out_len);

    for (size_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

TEST_F(FramingTest, HdlcFrameWithEscapedFlag)
{
    uint8_t input[] = {1, 0x7E, 3};
    uint8_t expected_output[] = {0x7E, 1, 0x7D, 0x5E, 3, 0x7E};

    uint8_t output[8];
    size_t out_len = frame_hdlc(output, 7, input, 3);

    ASSERT_EQ(6, out_len);

    for (size_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

TEST_F(FramingTest, HdlcFrameWithEscapedEscapeSequence)
{
    uint8_t input[] = {1, 0x7D, 3};
    uint8_t expected_output[] = {0x7E, 1, 0x7D, 0x5D, 3, 0x7E};

    uint8_t output[8];
    size_t out_len = frame_hdlc(output, 7, input, 3);

    ASSERT_EQ(6, out_len);

    for (size_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

TEST_F(FramingTest, HdlcDeframeWithNoEscape)
{
    uint8_t input[] = {0x7E, 1, 2, 3, 0x7E};
    uint8_t expected_output[] = {1, 2, 3};

    uint8_t output[5];
    size_t out_len = deframe_hdlc(output, 5, input, 5);

    ASSERT_EQ(3, out_len);

    for (size_t i = 0; i < 3; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

TEST_F(FramingTest, HdlcDeframeWithEscapedFlag)
{
    uint8_t input[] = {0x7E, 1, 0x7D, 0x5E, 3, 0x7E};
    uint8_t expected_output[] = {1, 0x7E, 3};

    uint8_t output[6];
    size_t out_len = deframe_hdlc(output, 6, input, 6);

    ASSERT_EQ(3, out_len);

    for (size_t i = 0; i < 3; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

TEST_F(FramingTest, HdlcDeframeWithEscapedEscapeSequence)
{
    uint8_t input[] = {0x7E, 1, 0x7D, 0x5D, 3, 0x7E};
    uint8_t expected_output[] = {1, 0x7D, 3};

    uint8_t output[6];
    size_t out_len = deframe_hdlc(output, 6, input, 6);

    ASSERT_EQ(3, out_len);

    for (size_t i = 0; i < 3; i++)
    {
        ASSERT_EQ(expected_output[i], output[i]);
    }
}

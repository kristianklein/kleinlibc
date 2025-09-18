#include "gtest/gtest.h"
#include <stdint.h>

extern "C"
{
    #include "kleinlibc/os/poll_timer.h"
}

uint32_t tick_ms = 0;

uint32_t get_ms()
{
    return tick_ms;
}

class PollTimerTest : public testing::Test
{
protected:
    poll_timer_t timer;
    void SetUp() override
    {
        tick_ms = 0; // reset tick between tests
    }
};

TEST_F(PollTimerTest, DoesNotCountIfNotStarted)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);

    tick_ms = 100;

    ASSERT_EQ(poll_timer_elapsed(&timer), 0);
}

TEST_F(PollTimerTest, CountsElapsedIfStarted)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);
    
    poll_timer_start(&timer);
    tick_ms = 100;

    ASSERT_EQ(poll_timer_elapsed(&timer), 100);
}

TEST_F(PollTimerTest, StopsCountingElapsedAfterStop)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    poll_timer_stop(&timer);
    tick_ms = 200;

    ASSERT_EQ(poll_timer_elapsed(&timer), 100);
}

TEST_F(PollTimerTest, StopsAndZeroElapsedAfterReset)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    poll_timer_reset(&timer);

    ASSERT_EQ(poll_timer_elapsed(&timer), 0);
}

TEST_F(PollTimerTest, AccumulatesElapsedWhenNotReset)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);
    
    poll_timer_start(&timer);
    tick_ms = 100; // counted
    poll_timer_stop(&timer);
    tick_ms = 200; // not counted
    poll_timer_start(&timer);
    tick_ms = 300; // counted
    poll_timer_stop(&timer);
    tick_ms = 400; // not counted

    ASSERT_EQ(poll_timer_elapsed(&timer), 200);
}

TEST_F(PollTimerTest, AccumulatesElapsedWhenNotResetAndStillRunning)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 0);
    
    poll_timer_start(&timer);
    tick_ms = 100; // counted
    poll_timer_stop(&timer);
    tick_ms = 200; // not counted
    poll_timer_start(&timer);
    tick_ms = 300; // counted

    ASSERT_EQ(poll_timer_elapsed(&timer), 200);
}

TEST_F(PollTimerTest, NotExpiredBeforeDurationElapsed)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 200);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    ASSERT_FALSE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, ExpiredAfterDurationElapsed)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 200);
    
    poll_timer_start(&timer);
    tick_ms = 200;
    ASSERT_TRUE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, NotExpiredBeforeAccumulatedDurationElapsed)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 300);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    poll_timer_stop(&timer);
    tick_ms = 200;
    poll_timer_start(&timer);
    tick_ms = 300;
    poll_timer_stop(&timer);
    ASSERT_FALSE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, ExpiredAfterAccumulatedDurationElapsed)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 200);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    poll_timer_stop(&timer);
    tick_ms = 200;
    poll_timer_start(&timer);
    tick_ms = 300;
    poll_timer_stop(&timer);
    ASSERT_TRUE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, ExpiredAfterAccumulatedDurationElapsedWhileRunning)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_SINGLE_SHOT, 200);
    
    poll_timer_start(&timer);
    tick_ms = 100;
    poll_timer_stop(&timer);
    tick_ms = 200;
    poll_timer_start(&timer);
    tick_ms = 300;
    ASSERT_TRUE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, ExpiresInContinuousMode)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_CONTINUOUS, 200);
    
    poll_timer_start(&timer);
    tick_ms = 300;
    ASSERT_TRUE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, ExpireAutoClearsInContinuousMode)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_CONTINUOUS, 200);
    
    poll_timer_start(&timer);
    tick_ms = 300;
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_FALSE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, AccumulatesExpirationsInContinuousMode)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_CONTINUOUS, 200);
    
    poll_timer_start(&timer);
    tick_ms = 900; // expired four times, but not five
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_FALSE(poll_timer_expired(&timer));
}

TEST_F(PollTimerTest, AccumulatesExpirationsInContinuousModeWhenPaused)
{
    poll_timer_init(&timer, get_ms, POLL_TIMER_CONTINUOUS, 200);
    
    poll_timer_start(&timer);
    tick_ms = 300;
    poll_timer_stop(&timer);
    tick_ms = 500;
    poll_timer_start(&timer);
    tick_ms = 700; // total elapsed time 500 ms, should have expired twice
    poll_timer_stop(&timer);
    ASSERT_EQ(poll_timer_elapsed(&timer), 500);
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_TRUE(poll_timer_expired(&timer));
    ASSERT_FALSE(poll_timer_expired(&timer));
}
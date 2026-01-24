set(KLEINLIBC_SOURCES
    dc_blocker.c
    fir_f32.c
    fir_i16.c
    poll_timer.c
    queue.c
    ringbuffer_i16.c
    ringbuffer_f32.c
    rtc_scheduler.c
    semaphore.c
    smoothing_filter.c
    arena.c
)

target_sources(kleinlibc PRIVATE ${KLEINLIBC_SOURCES})
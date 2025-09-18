set(OS_SOURCES
    semaphore.c
    rtc_scheduler.c
    poll_timer.c
)

target_sources(kleinlibc PRIVATE ${OS_SOURCES})

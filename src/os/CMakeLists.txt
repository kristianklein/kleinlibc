set(OS_SOURCES
    semaphore.c
    rtc_scheduler.c
)

target_sources(kleinlibc PRIVATE ${OS_SOURCES})

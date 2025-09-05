set(CONTAINER_SOURCES
    queue.c
    ringbuffer_i16.c
    ringbuffer_f32.c
)

target_sources(kleinlibc PRIVATE ${CONTAINER_SOURCES})
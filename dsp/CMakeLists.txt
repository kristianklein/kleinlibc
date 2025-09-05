set(DSP_SOURCES
    dc_blocker.c
    smoothing_filter.c
    fir_i16.c
)

target_sources(kleinlibc PRIVATE ${DSP_SOURCES})

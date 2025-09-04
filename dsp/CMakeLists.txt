set(DSP_SOURCES
    dc_blocker.c
    smoothing_filter.c
)

if(TARGET kleinlibc)
    target_sources(kleinlibc PRIVATE ${DSP_SOURCES})
endif()

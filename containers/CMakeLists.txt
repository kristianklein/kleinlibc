set(CONTAINER_SOURCES
    queue.c
    ringbuffer_i16.c
)

set(TEST_DIRECTORY "../test")

set(CONTAINER_TEST_SOURCES
    ${TEST_DIRECTORY}/queue_test.cpp
    ${TEST_DIRECTORY}/ringbuffer_i16_test.cpp
)

if(TARGET kleinlibc)
    target_sources(kleinlibc PRIVATE ${CONTAINER_SOURCES})
else()
    cmake_minimum_required(VERSION 3.20)
    
    message("Building unit tests")
    project(container_test
        LANGUAGES C CXX
    )

    set(CMAKE_BUILD_TYPE Debug)

    add_executable(container_test ${CONTAINER_SOURCES} ${CONTAINER_TEST_SOURCES})

    target_include_directories(container_test PRIVATE ../include/kleinlibc)
    target_link_libraries(container_test gtest gtest_main)
endif()
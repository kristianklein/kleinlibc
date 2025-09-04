set(CONTAINER_SOURCES
    queue.c
)
    
set(CONTAINER_TEST_SOURCES
    ../test/queue_test.cpp
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
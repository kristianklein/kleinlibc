cmake_minimum_required(VERSION 3.20)

project(kleinlibc
    LANGUAGES C CXX
)

option(BUILD_UNIT_TESTS "Build unit tests with GoogleTest" OFF)

add_library(kleinlibc STATIC)

if(BUILD_UNIT_TESTS)
    set(CMAKE_BUILD_TYPE Debug)
    add_executable(kleinlibc_test)
    add_subdirectory(test)
    target_link_libraries(kleinlibc_test gtest gtest_main kleinlibc)
endif()

add_subdirectory(src/containers)
add_subdirectory(src/dsp)
add_subdirectory(src/os)

target_include_directories(kleinlibc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
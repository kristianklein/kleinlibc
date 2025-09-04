cmake_minimum_required(VERSION 3.20)

project(kleinlibc
    LANGUAGES C CXX
)

add_library(kleinlibc STATIC)

add_subdirectory(containers)
add_subdirectory(dsp)

target_include_directories(kleinlibc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kleinlibc
)
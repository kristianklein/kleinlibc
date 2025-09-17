cmake_minimum_required(VERSION 3.20)

project(kleinlibc
    LANGUAGES C CXX
)

option(BUILD_UNIT_TESTS "Build unit tests with GoogleTest" OFF)
option(BUILD_DOCS "Build documentation" OFF)

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

#################
# DOCUMENTATION #
#################
function(create_venv venv_dir requirements)
    file(TO_CMAKE_PATH "${venv_dir}" venv)
    file(TO_CMAKE_PATH "${requirements}" reqs)

    if (EXISTS "${venv}")
        file(REMOVE_RECURSE "${venv}")
    endif()

    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m venv ${venv}
    )

    if (WIN32)
        execute_process(
            COMMAND "${venv}/Scripts/python.exe" -m pip install -r "${reqs}"
            )
    else()
        execute_process(
            COMMAND "${venv}/bin/python" -m pip install -r "${reqs}"
            )
    endif()

endfunction()

if (BUILD_DOCS)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        # Set up virtual environment in build directory
        find_package(Python3 REQUIRED)
        set(DOCS_BUILD_DIR "${CMAKE_BINARY_DIR}/docs")
        set(DOCS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/docs")
        file(MAKE_DIRECTORY "${DOCS_BUILD_DIR}")
        execute_process(
            COMMAND "${Python3_EXECUTABLE}" -m venv "${DOCS_BUILD_DIR}/venv"
        )

        create_venv("${DOCS_BUILD_DIR}/venv" "${CMAKE_SOURCE_DIR}/docs/requirements.txt")

        # Doxygen target
        set(DOXYFILE_DIR "${CMAKE_SOURCE_DIR}/docs")
        set(DOXYGEN_INPUT_DIRS "${DOCS_SOURCE_DIR}/src ${DOCS_SOURCE_DIR}/include")
        set(DOXYGEN_OUTPUT_DIR "${DOCS_BUILD_DIR}/doxygen")

        configure_file(
            "${DOCS_SOURCE_DIR}/Doxyfile.in"
            "${DOCS_BUILD_DIR}/Doxyfile"
            @ONLY)

        add_custom_target(
            doxygen
            COMMAND "${DOXYGEN_EXECUTABLE}"
            WORKING_DIRECTORY "${DOCS_BUILD_DIR}"
        )

        # Sphinx target
        find_program(
            SPHINX_EXECUTABLE REQUIRED
            NAMES sphinx-build sphinx-build.exe
            HINTS "${DOCS_BUILD_DIR}/venv/bin"
        )

        set(SPHINX_SOURCE_DIR "${CMAKE_SOURCE_DIR}/docs/source")
        set(SPHINX_BUILD_DIR "${DOCS_BUILD_DIR}/build")

        add_custom_target(
            docs ALL
            COMMAND "${CMAKE_COMMAND}" -E remove_directory "${SPHINX_BUILD_DIR}"
            COMMAND "${SPHINX_EXECUTABLE}" -M html "${SPHINX_SOURCE_DIR}" "${SPHINX_BUILD_DIR}"
            WORKING_DIRECTORY "${DOCS_SOURCE_DIR}"
            DEPENDS doxygen
        )
    else()
        message(FATAL_ERROR "Could not find Doxygen. If already installed, rerun the CMake configuration with '-DCMAKE_PREFIX_PATH=<path_to_doxygen>'.")
    endif()
endif()

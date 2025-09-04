# STM32F410RB-lib.cmake - Toolchain file for building libraries only

# Target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Binary utilities
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_RANLIB arm-none-eabi-ranlib)

# MCU-specific compilation flags
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# C and C++ flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -Os -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS}")

# CMake find root path settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# kleinlibc
A collection of modules for embedded systems written in C.

## Design philosophy
The library is designed specifically with resource-constrained embedded systems (such as microcontrollers) in mind. Thus, no modules perform any dynamic memory allocation. Instead, the consumer is required to statically allocate memory and pass a pointer/size into the module.

For now, some modules may depend on the reduced C standard library (i.e. newlib-nano), but this might change in the future.

The library is intended to be platform-agnostic, but is currently tested primarily on STM32 microcontrollers.

All modules should be hardware-independent, and able to run and be tested on the development machine (using GoogleTest). Dependency injection should be used anytime a module needs to interact with hardware. For example, if a module needs to read a GPIO pin, instead of including a platform-specific GPIO header in the 
module, the module should take a function pointer to a `read_pin()` function as a parameter. This allows
`read_pin()` to be easily mocked for off-target testing.

## Using the library
The easiest way to include the library in your project is simply to clone the repository into your
project tree, add the `include` directory to your include path and build everything in the `src`
directory. You can then include a module with e.g. `#include "kleinlibc/containers/queue.h"`.

If you wish, you can compile the source code into a static library using CMake. You will need a CMake toolchain file in order to cross-compile for your specific target platform.

```
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=stm32f410rb.cmake
$ cmake --build .
```

## Build unit tests
*Note: GoogleTest must be installed in your PATH.*

To build the library and unit tests for your development machine, set the `BUILD_UNIT_TESTS` variable
when invoking CMake:

```
$ mkdir build_test && cd build_test
$ cmake .. -DBUILD_UNIT_TESTS=ON
$ cmake --build .
```

Now you can run the unit test suite with:
```
$ ./kleinlibc_test
```

### Building and installing GoogleTest

To install GoogleTest system-wide, simply clone the Github repository
and build/install using CMake and make:

```
$ git clone https://github.com/google/googletest
$ cd googletest
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
$ make
$ sudo make install
```
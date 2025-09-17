# kleinlibc
**kleinlibc** is a collection of modules for embedded systems written in C.

## Design philosophy
The library is designed specifically with resource-constrained embedded
systems (such as microcontrollers) in mind. Thus, no modules perform 
any dynamic memory allocation. Instead, the consumer is required to 
statically allocate memory and pass a pointer/size into the module.

For now, some modules may depend on the reduced C standard library 
(i.e. newlib-nano), but this might change in the future.

The library is intended to be platform-agnostic, but is currently 
tested primarily on STM32 microcontrollers.

All modules must be hardware-independent and able to run and be tested 
on the development machine. Dependency injection should be used anytime 
a module needs to interact with hardware. For example, if a module 
needs to read a GPIO pin, instead of including a platform-specific GPIO 
header in the module, the module should take a generic function pointer 
(or a struct of function pointers) as input for any hardware-related 
functionality. This allows for hardware-related functions to be easily 
mocked for off-target testing.

## Building the library
The easiest way to include the library in your project is simply to 
clone the repository into your project tree, add the `include` 
directory to your include path and build everything in the `src`
directory. You can then include a module with e.g. 
`#include "kleinlibc/containers/queue.h"`.

If you wish, you can compile the source code into a static library 
using CMake. You will need a CMake toolchain file in order to 
cross-compile for your specific target platform.

```
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=stm32f410rb.cmake
$ cmake --build .
```

## Build unit tests
*Note: GoogleTest must be installed in your PATH. See below for 
instructions.*

To build the library and unit tests for your development machine, set 
the `BUILD_UNIT_TESTS` variable during CMake configuration:

```
$ mkdir build_test && cd build_test
$ cmake .. -DBUILD_UNIT_TESTS=ON
$ cmake --build .
```

You can now run the unit test suite with:
```
$ ./kleinlibc_test
```

### Building and installing GoogleTest

To install GoogleTest system-wide, simply clone the Github repository
and build/install using CMake:

```
$ git clone https://github.com/google/googletest
$ cd googletest
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
$ cmake --build . --target install
```

## Documentation
The documentation is located in the `docs` directory. The code itself is documented
using Doxygen comments, while the final documentation is built with Sphinx. Make
sure Doxygen and Python3 is installed on your machine before trying to build the docs.

Enabling the CMake option `BUILD_DOCS` during configuration will create a Python virtual
environment in the build directory and install the required packages.

```
$ mkdir build_docs && cd build_docs
$ cmake .. -DBUILD_DOCS=ON
```

*Note: It may be necessary to help CMake find your Doxygen installation, e.g. with `-DCMAKE_PREFIX_PATH=/usr/local`*

Now build the documentation with:

```
$ cmake --build . --target docs
```

Open `build_docs/docs/build/html/index.html` to view the documentation.

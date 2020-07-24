# Installation of LTOList

LTOList is a header-only library consisting of only one file `LTOList.hpp`, so put it in any directory you like and include by `#include "path/to/LTOList.hpp"` it.

**Although there is a file `CMakeLists.txt`, you do not need `cmake` if you just use LTOList.**

# Running examples and tests

## Preparation

Examples and tests are built by `cmake` (https://cmake.org/). In the directory of LTOList, the command will produce `Makefile`:

```
cmake .
```

## Building an example

Examples are implemented as files named `LTOList_example_*.cpp`.

After the `Makefile` is produced by `cmake` as above, to build an example, use the command `make [ExampleName]`, where `[ExampleName]` is the example file name without the extension `.cpp`.

For example of building the example file `LTOList_example_performance.cpp`, the following command will build the executable for the example:

```
make LTOList_example_performance
```

## Building a test

Tests are implemented as files named `LTOList_test_*.cpp`.

First, these tests are implemented with Catch2 (https://github.com/catchorg/Catch2). Before building, put `catch2` directory (including its contents) at https://github.com/catchorg/Catch2/tree/master/single_include in the same directory as `CMakeLists.txt` (Note: the directory `catch2` is at the same directory as `CMakeLists.txt`, not the contents in `catch2` are at the same directory as `CMakeLists.txt`).

Then, after the `Makefile` is produced by `cmake` as above, to build a test, use the command `make [TestName]`, where `[TestName]` is the example file name without the extension `.cpp`.

In addition, using the command `make test_[TestName]` will run the test code after building the executable (if not exist). Using the command `make test` will do them for all tests.

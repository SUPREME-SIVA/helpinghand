# HelpingHand Test Suite

This directory contains unit tests for the C and C++ programs in the HelpingHand repository.

## Test Files

### C Tests

The following C test files are available:

1. `test_palindrome.c` - Tests for the palindrome checking functions in `Palindrome.c`
2. `test_string_reverse.c` - Tests for the string reversal functions in `StringRevers.c`
3. `test_array_rotation.c` - Tests for the array rotation functions in `array_rotation.c`
4. `test_bit_flip.c` - Tests for the bit manipulation functions in `bit_flip.c`
5. `test_addition.c` - Tests for the addition function in `addition.c`

### C++ Tests

The following C++ test files are available:

1. `test_shared_ptr_demo.cpp` - Tests for shared pointer functionality in `shared_ptr_demo.cpp`
2. `test_unique_ptr_demo.cpp` - Tests for unique pointer functionality in `unique_ptr_demo.cpp`
3. `test_weather_observer.cpp` - Tests for the Observer pattern implementation in `weather_observer.cpp`
4. `test_vector_capacity_demo.cpp` - Tests for vector capacity and operations in `vector_capacity_demo.cpp`
5. `test_logger_singleton.cpp` - Tests for the Singleton pattern implementation in `logger_singleton.cpp`

## Running Tests

You can run all tests or individual tests using the Makefile:

```bash
# Build and run all tests (C and C++)
make run-tests

# Build and run only C tests
make run-c-tests

# Build and run only C++ tests
make run-cxx-tests

# Build and run a specific test
make run-test-test_palindrome
make run-test-test_shared_ptr_demo
```

## Test Methodology

Each test file follows a similar structure:

1. **Function Implementation**: Each test file includes the necessary functions from the original source file to be tested.
2. **Helper Functions**: Additional helper functions for test validation and debugging.
3. **Test Cases**: Multiple test cases covering:
   - Normal operation
   - Edge cases
   - Boundary conditions
   - Special inputs

4. **Assertions**: Using C's `assert.h` library for C tests and C++'s `cassert` for C++ tests to verify expected behavior.

## Adding New Tests

To add a new test file:

1. Create a new file in the `tests` directory with the naming convention:
   - `test_<program_name>.c` for C programs
   - `test_<program_name>.cpp` for C++ programs
2. Implement the necessary test functions and cases
3. Add the test file to the appropriate variable in the Makefile:
   - `C_TEST_SRCS` for C test files
   - `CXX_TEST_SRCS` for C++ test files
4. Run `make run-tests` to verify the test works correctly

## Test Coverage

The current test suite covers:

- Basic functionality testing
- Edge case handling
- Input validation
- Performance considerations (where applicable)
- Design pattern verification (for C++ programs)
- Memory management testing (for C++ smart pointers)

Future improvements may include:

- Integration with a more robust testing framework
- Code coverage analysis
- Benchmark testing for performance-critical functions
- Memory leak detection for complex operations

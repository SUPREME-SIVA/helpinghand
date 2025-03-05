# HelpingHand C/C++ Repository

This repository contains various C and C++ programs, examples, and utilities for learning and reference purposes.

## Main Programs

### Simple Addition Program in C

This is a basic C program that adds two numbers entered by the user.

#### How to Compile and Run

##### Using Make
```bash
# Compile the program
make

# Run the program
./addition
```

##### Manual Compilation
```bash
gcc -Wall -Wextra -std=c99 -o addition addition.c
./addition
```

#### Features
- Takes two integer inputs from the user
- Calculates and displays their sum
- Uses a separate function for the addition operation

## Examples

The repository includes a collection of example programs in the `examples/` directory that demonstrate various C++ concepts and features.

### C++ Vector Examples

The following examples demonstrate the usage of the `std::vector` container in C++:

1. **Basic Vector Operations** (`vector_basics.cpp`)
   - Creating and initializing vectors
   - Adding and removing elements
   - Accessing elements by index
   - Iterating through vectors
   - Modifying elements
   - Clearing vectors

2. **Vector with Custom Objects** (`vector_custom_objects.cpp`)
   - Creating vectors of custom class objects
   - Using vector capacity and reserve
   - In-place construction with emplace_back
   - Bounds checking with at()
   - Exception handling

3. **Advanced Vector Operations** (`vector_advanced.cpp`)
   - Sorting vectors
   - Reversing vectors
   - Finding elements
   - Calculating sum of elements
   - Creating 2D vectors (vector of vectors)
   - Inserting elements at specific positions
   - Removing elements based on conditions
   - Creating vectors with initial size and value

### How to Build and Run Examples

#### Using Make

```bash
# Build all examples
make examples

# Run all examples
make run-examples

# Build and run a specific example
make run-example-vector_basics
make run-example-vector_custom_objects
make run-example-vector_advanced
```

#### Manual Compilation

```bash
# Compile a specific example
g++ -std=c++11 examples/vector_basics.cpp -o examples/vector_basics

# Run the example
./examples/vector_basics
```

## Design Patterns

The repository includes a collection of modern C++ design pattern implementations in the `design_patterns/` directory:

### Creational Patterns

1. **Thread-Safe Singleton** (`thread_safe_singleton.cpp`)
   - Implements the Singleton pattern with thread safety using std::mutex
   - Uses unique_ptr for automatic memory management
   - Implements double-checked locking pattern for performance
   - Provides exception handling for multiple instance creation attempts
   - Demonstrates RAII principles through smart pointers

### How to Build and Run Design Patterns

#### Using Make

```bash
# Build all design patterns
make design-patterns

# Run all design patterns
make run-design-patterns

# Build and run a specific design pattern
make run-design-pattern-thread_safe_singleton
```

#### Manual Compilation

```bash
# Compile a specific design pattern
g++ -std=c++17 -pthread design_patterns/thread_safe_singleton.cpp -o design_patterns/thread_safe_singleton

# Run the design pattern
./design_patterns/thread_safe_singleton
```

## Testing

The repository includes a comprehensive testing approach for C and C++ programs:

- Unit tests are organized in the `/tests` directory with a naming convention of `test_*.c`
- Each test file includes test cases for normal operation, edge cases, and boundary conditions
- Tests use assert.h for validation
- Test results are documented in TEST_SUMMARY.md

### Running Tests

```bash
# Build all tests
make tests

# Run all tests
make run-tests

# Run a specific test
make run-test-NAME
```

## License
This project is open source and available under the MIT License.

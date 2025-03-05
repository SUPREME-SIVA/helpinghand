# C++ Design Patterns

This directory contains implementations of various design patterns in C++. Each implementation includes detailed explanations, practical usage examples, and demonstrates modern C++ techniques.

## Available Patterns

### Creational Patterns

1. **Singleton Pattern** (`thread_safe_singleton.cpp`)
   - Thread-safe implementation with unique_ptr
   - Double-checked locking pattern for performance
   - Error handling for multiple instance creation attempts
   - Proper synchronization using std::mutex
   - RAII principles through smart pointers

## Key Features

- **Modern C++**: All implementations use C++11/14/17 features
- **Thread Safety**: Patterns are designed with concurrency in mind
- **Memory Management**: Smart pointers for automatic memory management
- **Exception Safety**: Proper exception handling throughout the code
- **Comprehensive Documentation**: Detailed comments and explanations

## Using the Examples

Each pattern implementation is a standalone C++ file that can be compiled and run independently:

```bash
# Compile specific pattern
g++ -std=c++17 -pthread thread_safe_singleton.cpp -o thread_safe_singleton

# Run the example
./thread_safe_singleton
```

## Pattern Implementation Guidelines

All design pattern implementations in this directory follow these guidelines:

1. **Practical Focus**: Demonstrate the pattern in a realistic context
2. **Thread Safety**: Consider concurrent usage where appropriate
3. **Memory Safety**: Prevent leaks through RAII and smart pointers
4. **Error Handling**: Provide clear and specific error messages
5. **Documentation**: Include comments explaining the pattern and implementation details

## References

- "Design Patterns: Elements of Reusable Object-Oriented Software" by Gamma, Helm, Johnson, and Vlissides
- "Modern C++ Design" by Andrei Alexandrescu
- C++ Core Guidelines

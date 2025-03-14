# C++17 Features Guide

This directory contains examples and explanations of C++17 features, organized for interview preparation. C++17 (formally known as ISO/IEC 14882:2017) introduced many powerful features that simplified code, improved performance, and added new functionality to the language.

## Files in this Directory

- **cpp17_basic_features.cpp**: Covers fundamental C++17 features like structured bindings, if/switch with initialization, inline variables, and more.
- **cpp17_intermediate_features.cpp**: Explores more complex features like fold expressions, constexpr if, template argument deduction for class templates, and more.
- **cpp17_advanced_features.cpp**: Demonstrates advanced features like std::variant, std::optional, std::any, parallel algorithms, and other library improvements.

## Compilation

Use the provided Makefile to compile the examples:

```bash
make all      # Compile all examples
make basic    # Compile only basic features
make intermediate # Compile only intermediate features
make advanced # Compile only advanced features
make clean    # Remove compiled files
```

## Requirements

- A C++17 compliant compiler (GCC 7+, Clang 5+, MSVC 19.14+)
- Make (for using the Makefile)

## Additional Resources

- [C++17 Standard](https://isocpp.org/std/the-standard)
- [cppreference - C++17](https://en.cppreference.com/w/cpp/17)

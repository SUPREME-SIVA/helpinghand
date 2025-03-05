# C++ Examples

This directory contains various C++ example programs that demonstrate different C++ concepts, features, and best practices.

## Vector Examples

The following examples demonstrate the usage of the `std::vector` container in C++:

### 1. Basic Vector Operations (`vector_basics.cpp`)

This example demonstrates the fundamental operations of `std::vector`:

- Creating an empty vector
- Adding elements with `push_back()`
- Accessing elements using the `[]` operator
- Getting the size of the vector
- Iterating through the vector using range-based for loops
- Modifying elements
- Removing elements with `pop_back()`
- Clearing the vector

**Compile and Run:**
```bash
make run-example-vector_basics
```

### 2. Vector with Custom Objects (`vector_custom_objects.cpp`)

This example shows how to use `std::vector` with custom class objects:

- Creating a `Student` class with name, ID, and GPA
- Creating a vector of `Student` objects
- Adding students to the vector using `push_back()`
- Understanding vector capacity vs. size
- Using `reserve()` to pre-allocate memory
- Using `emplace_back()` for in-place construction
- Using `at()` for bounds-checked access
- Exception handling with `std::out_of_range`

**Compile and Run:**
```bash
make run-example-vector_custom_objects
```

### 3. Advanced Vector Operations (`vector_advanced.cpp`)

This example demonstrates more advanced operations with `std::vector` and STL algorithms:

- Initializing a vector with values
- Sorting a vector using `std::sort()`
- Reversing a vector using `std::reverse()`
- Finding elements with `std::find()`
- Calculating the sum of elements with `std::accumulate()`
- Creating and using a 2D vector (vector of vectors)
- Inserting elements at specific positions
- Removing elements that satisfy a condition using `std::remove_if()`
- Creating a vector with a specific size and initial value

**Compile and Run:**
```bash
make run-example-vector_advanced
```

## When to Use std::vector vs. std::list

### Use std::vector When:

- You need frequent random access to elements (O(1) operation)
- You primarily add/remove elements at the end of the container
- You need a cache-friendly data structure
- Memory usage is a concern, and you want elements stored contiguously
- You need to sort elements frequently or use binary search algorithms
- The size of the container is known in advance or doesn't change frequently

### Use std::list When:

- You need frequent insertions and deletions at arbitrary positions
- You don't need random access to elements (lists only provide sequential access)
- You want to avoid the cost of memory reallocation when the container grows
- You need stable iterators (iterators to list elements remain valid even after insertions/deletions)
- You need efficient splicing operations (moving elements from one list to another)
- The data structure needs to grow in both directions efficiently

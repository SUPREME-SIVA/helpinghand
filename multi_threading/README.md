# Multi-Threading and Multi-Processing in C/C++

This directory contains examples and explanations of multi-threading and multi-processing concepts in C and C++.

## Key Concepts

### Multi-Threading vs. Multi-Processing

| Feature | Multi-Threading | Multi-Processing |
|---------|----------------|------------------|
| Definition | Multiple threads within a single process | Multiple processes running independently |
| Memory Space | Shared memory space | Separate memory spaces |
| Communication | Easy (shared memory) | More complex (IPC mechanisms) |
| Context Switching | Lightweight | Heavyweight |
| Fault Tolerance | One thread crash may affect others | Process isolation provides better fault tolerance |
| Resource Usage | Less resource-intensive | More resource-intensive |
| Implementation | pthread, std::thread | fork(), exec() |

### Common Concurrency Issues

1. **Race Conditions**: When multiple threads access shared data and at least one thread modifies it
2. **Deadlocks**: When two or more threads are blocked forever, waiting for each other
3. **Livelocks**: Threads are not blocked but unable to make progress
4. **Priority Inversion**: Lower priority thread holds a resource needed by higher priority thread
5. **Thread Starvation**: Thread is perpetually denied resources to progress

## Examples in this Directory

1. **Basic Threading**
   - `basic_threading.c` - POSIX threads in C
   - `basic_threading.cpp` - std::thread in C++

2. **Basic Processing**
   - `basic_processing.c` - fork() and process creation in C
   - `basic_processing.cpp` - process management in C++

3. **Synchronization**
   - `mutex_example.c` - Mutex usage in C
   - `mutex_example.cpp` - std::mutex in C++
   - `condition_variables.cpp` - std::condition_variable in C++

4. **Deadlock**
   - `deadlock_example.c` - Demonstrating deadlock in C
   - `deadlock_example.cpp` - Demonstrating deadlock in C++
   - `deadlock_prevention.cpp` - Techniques to prevent deadlocks

5. **Thread Pools**
   - `thread_pool.cpp` - Implementation of a thread pool in C++

## Building and Running

Use the Makefile in the root directory:

```bash
# Build all multi-threading examples
make multi-threading

# Run all multi-threading examples
make run-multi-threading

# Run a specific example
make run-multi-threading-basic_threading
```

## Best Practices

1. **Use high-level abstractions** when possible (std::thread, std::mutex, etc.)
2. **Minimize shared mutable state** to reduce race conditions
3. **Acquire locks in a consistent order** to prevent deadlocks
4. **Use RAII** for resource management (in C++)
5. **Consider thread-local storage** for thread-specific data
6. **Prefer task-based programming** over direct thread management
7. **Use atomic operations** for simple shared variables
8. **Test thoroughly** with different thread interleavings

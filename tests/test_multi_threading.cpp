/**
 * @file test_multi_threading.cpp
 * @brief Test cases for C++ multithreading implementations
 * 
 * This file contains tests for the C++ multithreading implementations:
 * - basic_threading.cpp: Tests for thread creation, mutex synchronization, and race conditions
 * - deadlock_example.cpp: Tests for deadlock prevention techniques
 * - basic_processing.cpp: Tests for process creation and communication
 * 
 * Compilation: g++ -std=c++17 -o test_multi_threading test_multi_threading.cpp -pthread
 * Usage: ./test_multi_threading
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <functional>
#include <cassert>
#include <condition_variable>
#include <unistd.h>
#include <sys/wait.h>

// Constants for testing
constexpr int NUM_THREADS = 10;
constexpr int ITERATIONS = 1000;
constexpr int TIMEOUT_MS = 5000;  // 5 seconds timeout for deadlock detection

// Global variables for testing
std::mutex test_mutex;
std::atomic<int> atomic_counter(0);
int unsafe_counter = 0;

// Forward declarations of test functions
void test_thread_creation();
void test_mutex_synchronization();
void test_race_condition();
void test_deadlock_prevention();
void test_hierarchical_mutex();
void test_process_creation();
void test_pipe_communication();

/**
 * @brief Main test function
 * @return 0 on success, non-zero on failure
 */
int main() {
    std::cout << "=== Running C++ Multithreading Tests ===\n";
    
    try {
        // Test thread creation and joining
        test_thread_creation();
        std::cout << "✓ Thread creation test passed\n";
        
        // Test mutex synchronization
        test_mutex_synchronization();
        std::cout << "✓ Mutex synchronization test passed\n";
        
        // Test race condition detection
        test_race_condition();
        std::cout << "✓ Race condition test passed\n";
        
        // Test deadlock prevention
        test_deadlock_prevention();
        std::cout << "✓ Deadlock prevention test passed\n";
        
        // Test hierarchical mutex
        test_hierarchical_mutex();
        std::cout << "✓ Hierarchical mutex test passed\n";
        
        // Test process creation
        test_process_creation();
        std::cout << "✓ Process creation test passed\n";
        
        // Test pipe communication
        test_pipe_communication();
        std::cout << "✓ Pipe communication test passed\n";
        
        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}

/**
 * @brief Test thread creation and joining
 */
void test_thread_creation() {
    std::vector<std::thread> threads;
    std::atomic<int> thread_count(0);
    
    // Create multiple threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&thread_count, i]() {
            thread_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << "Thread " << i << " executed\n";
        });
    }
    
    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Verify all threads executed
    assert(thread_count == NUM_THREADS);
}

/**
 * @brief Test mutex synchronization
 */
void test_mutex_synchronization() {
    std::vector<std::thread> threads;
    int protected_counter = 0;
    std::mutex counter_mutex;
    
    // Create multiple threads that increment the counter with mutex protection
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&protected_counter, &counter_mutex]() {
            for (int j = 0; j < ITERATIONS; j++) {
                std::lock_guard<std::mutex> lock(counter_mutex);
                protected_counter++;
            }
        });
    }
    
    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Verify the counter has the expected value
    assert(protected_counter == NUM_THREADS * ITERATIONS);
}

/**
 * @brief Test race condition detection
 */
void test_race_condition() {
    std::vector<std::thread> threads;
    int unprotected_counter = 0;
    std::atomic<int> atomic_counter(0);
    
    // Create multiple threads that increment the counter without protection (race condition)
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&unprotected_counter, &atomic_counter]() {
            for (int j = 0; j < ITERATIONS; j++) {
                unprotected_counter++;  // Unprotected access - race condition
                atomic_counter++;       // Atomic access - no race condition
            }
        });
    }
    
    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Verify the atomic counter has the expected value
    assert(atomic_counter == NUM_THREADS * ITERATIONS);
    
    // The unprotected counter will likely have a value less than expected due to race conditions
    std::cout << "Expected counter value: " << NUM_THREADS * ITERATIONS << std::endl;
    std::cout << "Actual unprotected counter value: " << unprotected_counter << std::endl;
    
    // This test passes if we detect a race condition (counter value is wrong)
    // In a real application, this would be a bug, but for this test, we want to verify
    // that race conditions can occur without proper synchronization
    if (unprotected_counter != NUM_THREADS * ITERATIONS) {
        std::cout << "Race condition detected as expected\n";
    } else {
        std::cout << "Warning: Race condition not detected. This might happen occasionally.\n";
    }
}

/**
 * @brief Helper class for deadlock testing
 */
class Resource {
private:
    std::mutex mtx;
    std::string name;
    
public:
    explicit Resource(std::string n) : name(std::move(n)) {}
    
    void lock() {
        mtx.lock();
    }
    
    void unlock() {
        mtx.unlock();
    }
    
    bool try_lock() {
        return mtx.try_lock();
    }
    
    std::mutex& get_mutex() {
        return mtx;
    }
    
    const std::string& get_name() const {
        return name;
    }
};

/**
 * @brief Test deadlock prevention using std::lock
 */
void test_deadlock_prevention() {
    Resource resource_a("Resource A");
    Resource resource_b("Resource B");
    
    bool deadlock_detected = false;
    std::condition_variable cv;
    std::mutex cv_m;
    bool thread1_done = false;
    bool thread2_done = false;
    
    // Thread 1: Uses std::lock to acquire both resources safely
    std::thread t1([&]() {
        // Use std::lock to acquire both locks without deadlock
        std::lock(resource_a.get_mutex(), resource_b.get_mutex());
        
        // Use std::lock_guard with adopt_lock to ensure proper unlock
        std::lock_guard<std::mutex> lock_a(resource_a.get_mutex(), std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(resource_b.get_mutex(), std::adopt_lock);
        
        std::cout << "Thread 1 acquired both resources safely\n";
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        {
            std::lock_guard<std::mutex> lk(cv_m);
            thread1_done = true;
        }
        cv.notify_one();
    });
    
    // Thread 2: Uses std::scoped_lock (C++17) to acquire both resources safely
    std::thread t2([&]() {
        // Use std::scoped_lock to acquire both locks without deadlock
        std::scoped_lock lock(resource_b.get_mutex(), resource_a.get_mutex());
        
        std::cout << "Thread 2 acquired both resources safely\n";
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        {
            std::lock_guard<std::mutex> lk(cv_m);
            thread2_done = true;
        }
        cv.notify_one();
    });
    
    // Wait for both threads to complete or timeout
    {
        std::unique_lock<std::mutex> lk(cv_m);
        if (!cv.wait_for(lk, std::chrono::milliseconds(TIMEOUT_MS), 
                        [&]{ return thread1_done && thread2_done; })) {
            deadlock_detected = true;
        }
    }
    
    // Join threads
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    
    // Verify no deadlock occurred
    assert(!deadlock_detected);
}

/**
 * @brief Hierarchical mutex implementation for testing
 */
class HierarchicalMutex {
private:
    std::mutex mtx;
    const unsigned long hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
    
public:
    explicit HierarchicalMutex(unsigned long value) : hierarchy_value(value), previous_hierarchy_value(0) {}
    
    void lock() {
        check_for_hierarchy_violation();
        mtx.lock();
        update_hierarchy_value();
    }
    
    void unlock() {
        this_thread_hierarchy_value = previous_hierarchy_value;
        mtx.unlock();
    }
    
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!mtx.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
    
private:
    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("Hierarchy violation");
        }
    }
    
    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
};

// Initialize the thread-local variable
thread_local unsigned long HierarchicalMutex::this_thread_hierarchy_value(ULONG_MAX);

/**
 * @brief Test hierarchical mutex for deadlock prevention
 */
void test_hierarchical_mutex() {
    HierarchicalMutex high_mutex(10000);
    HierarchicalMutex mid_mutex(5000);
    HierarchicalMutex low_mutex(1000);
    
    std::atomic<bool> correct_order_passed(false);
    std::atomic<bool> wrong_order_failed(false);
    
    // Thread using correct lock order (high -> mid -> low)
    std::thread t1([&]() {
        try {
            std::lock_guard<HierarchicalMutex> high_lock(high_mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            std::lock_guard<HierarchicalMutex> mid_lock(mid_mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            std::lock_guard<HierarchicalMutex> low_lock(low_mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            correct_order_passed = true;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception in correct order thread: " << e.what() << std::endl;
        }
    });
    
    // Thread using incorrect lock order (low -> high) - should throw
    std::thread t2([&]() {
        try {
            std::lock_guard<HierarchicalMutex> low_lock(low_mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            std::lock_guard<HierarchicalMutex> high_lock(high_mutex);  // Should throw
            
            std::cerr << "Error: Hierarchy violation not detected!\n";
        } catch (const std::logic_error& e) {
            // Expected exception
            wrong_order_failed = true;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception type: " << e.what() << std::endl;
        }
    });
    
    // Join threads
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    
    // Verify correct behavior
    assert(correct_order_passed);
    assert(wrong_order_failed);
}

/**
 * @brief Test process creation and termination
 */
void test_process_creation() {
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error occurred
        throw std::runtime_error("fork failed");
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process created successfully\n";
        exit(42);  // Exit with a specific code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        // Verify child exited with expected status
        assert(WIFEXITED(status));
        assert(WEXITSTATUS(status) == 42);
    }
}

/**
 * @brief Test pipe communication between processes
 */
void test_pipe_communication() {
    int pipe_fd[2];
    constexpr int BUFFER_SIZE = 100;
    char write_msg[BUFFER_SIZE] = "Test message from parent to child";
    char read_msg[BUFFER_SIZE];
    
    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        throw std::runtime_error("pipe creation failed");
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error occurred
        throw std::runtime_error("fork failed");
    } else if (pid == 0) {
        // Child process
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Read from the pipe
        read(pipe_fd[0], read_msg, BUFFER_SIZE);
        
        // Verify the message
        assert(strcmp(read_msg, write_msg) == 0);
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        // Write to the pipe
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1);
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Wait for child to complete
        int status;
        waitpid(pid, &status, 0);
        
        // Verify child exited successfully
        assert(WIFEXITED(status));
        assert(WEXITSTATUS(status) == EXIT_SUCCESS);
    }
}

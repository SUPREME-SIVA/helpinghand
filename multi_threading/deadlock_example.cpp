/**
 * @file deadlock_example.cpp
 * @brief Demonstrates deadlock scenarios and prevention techniques in C++
 * 
 * This example shows:
 * 1. How deadlocks occur when multiple threads acquire locks in different orders
 * 2. How to prevent deadlocks using std::lock and std::scoped_lock
 * 3. How to use std::try_lock and timeouts to detect potential deadlocks
 * 4. RAII principles for resource management
 * 
 * Compilation: g++ -std=c++17 -o deadlock_example deadlock_example.cpp -pthread
 * Usage: ./deadlock_example
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <functional>
#include <string>

// Mutex locks for resources
std::mutex resource_a_mutex;
std::mutex resource_b_mutex;

// Class to represent a resource with a name
class Resource {
private:
    std::string name;
    std::mutex& mutex;

public:
    Resource(const std::string& name, std::mutex& mutex) : name(name), mutex(mutex) {}

    // Get the name of the resource
    std::string get_name() const {
        return name;
    }

    // Get the mutex for this resource
    std::mutex& get_mutex() {
        return mutex;
    }

    // Simulate using the resource
    void use() {
        std::cout << "Using resource " << name << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
};

// Global resources
Resource resource_a("A", resource_a_mutex);
Resource resource_b("B", resource_b_mutex);

/**
 * @brief Demonstrates a deadlock scenario
 */
void demonstrate_deadlock() {
    std::cout << "\n=== Demonstrating Deadlock Scenario ===\n";
    std::cout << "In this scenario, Thread 1 acquires Resource A and then tries to acquire Resource B.\n";
    std::cout << "Meanwhile, Thread 2 acquires Resource B and then tries to acquire Resource A.\n";
    std::cout << "This creates a circular wait condition, leading to deadlock.\n\n";

    // Thread 1 function: acquire resource A, then resource B
    auto thread1_func = []() {
        std::cout << "Thread 1: Starting\n";
        std::cout << "Thread 1: Trying to acquire Resource A\n";
        
        // Acquire resource A
        std::lock_guard<std::mutex> lock_a(resource_a.get_mutex());
        std::cout << "Thread 1: Acquired Resource A\n";
        
        // Simulate some work
        std::cout << "Thread 1: Working with Resource A\n";
        resource_a.use();
        
        std::cout << "Thread 1: Trying to acquire Resource B\n";
        
        // This can lead to deadlock if Thread 2 has acquired Resource B
        // and is waiting for Resource A
        std::lock_guard<std::mutex> lock_b(resource_b.get_mutex());
        std::cout << "Thread 1: Acquired Resource B\n";
        
        // Work with both resources
        std::cout << "Thread 1: Working with both resources\n";
        resource_a.use();
        resource_b.use();
        
        std::cout << "Thread 1: Finished\n";
    };

    // Thread 2 function: acquire resource B, then resource A
    auto thread2_func = []() {
        std::cout << "Thread 2: Starting\n";
        std::cout << "Thread 2: Trying to acquire Resource B\n";
        
        // Acquire resource B
        std::lock_guard<std::mutex> lock_b(resource_b.get_mutex());
        std::cout << "Thread 2: Acquired Resource B\n";
        
        // Simulate some work
        std::cout << "Thread 2: Working with Resource B\n";
        resource_b.use();
        
        std::cout << "Thread 2: Trying to acquire Resource A\n";
        
        // This can lead to deadlock if Thread 1 has acquired Resource A
        // and is waiting for Resource B
        std::lock_guard<std::mutex> lock_a(resource_a.get_mutex());
        std::cout << "Thread 2: Acquired Resource A\n";
        
        // Work with both resources
        std::cout << "Thread 2: Working with both resources\n";
        resource_a.use();
        resource_b.use();
        
        std::cout << "Thread 2: Finished\n";
    };

    // Create threads
    std::thread thread1(thread1_func);
    
    // Small delay to ensure Thread 1 starts first
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::thread thread2(thread2_func);
    
    // Wait for threads to complete (they may never complete due to deadlock)
    std::cout << "Main: Waiting for threads to complete (they may deadlock)...\n";
    std::cout << "If the program hangs, press Ctrl+C to terminate.\n";
    
    if (thread1.joinable()) thread1.join();
    if (thread2.joinable()) thread2.join();
    
    std::cout << "Main: Both threads completed successfully (no deadlock occurred).\n";
}

/**
 * @brief Demonstrates deadlock prevention using std::lock
 */
void demonstrate_deadlock_prevention_with_std_lock() {
    std::cout << "\n=== Demonstrating Deadlock Prevention with std::lock ===\n";
    std::cout << "In this scenario, threads use std::lock to acquire multiple locks atomically.\n";
    std::cout << "This prevents the circular wait condition and avoids deadlock.\n\n";

    // Thread 1 function: acquire both resources using std::lock
    auto thread1_func = []() {
        std::cout << "Thread 1: Starting\n";
        std::cout << "Thread 1: Trying to acquire both resources atomically\n";
        
        // Use std::lock to acquire both mutexes atomically
        std::unique_lock<std::mutex> lock_a(resource_a.get_mutex(), std::defer_lock);
        std::unique_lock<std::mutex> lock_b(resource_b.get_mutex(), std::defer_lock);
        
        // Acquire both locks atomically (prevents deadlock)
        std::lock(lock_a, lock_b);
        
        std::cout << "Thread 1: Acquired both resources\n";
        
        // Work with both resources
        std::cout << "Thread 1: Working with both resources\n";
        resource_a.use();
        resource_b.use();
        
        std::cout << "Thread 1: Finished\n";
    };

    // Thread 2 function: acquire both resources using std::lock
    auto thread2_func = []() {
        std::cout << "Thread 2: Starting\n";
        std::cout << "Thread 2: Trying to acquire both resources atomically\n";
        
        // Use std::lock to acquire both mutexes atomically
        std::unique_lock<std::mutex> lock_b(resource_b.get_mutex(), std::defer_lock);
        std::unique_lock<std::mutex> lock_a(resource_a.get_mutex(), std::defer_lock);
        
        // Acquire both locks atomically (prevents deadlock)
        std::lock(lock_b, lock_a);
        
        std::cout << "Thread 2: Acquired both resources\n";
        
        // Work with both resources
        std::cout << "Thread 2: Working with both resources\n";
        resource_b.use();
        resource_a.use();
        
        std::cout << "Thread 2: Finished\n";
    };

    // Create threads
    std::thread thread1(thread1_func);
    std::thread thread2(thread2_func);
    
    // Wait for threads to complete
    std::cout << "Main: Waiting for threads to complete...\n";
    
    if (thread1.joinable()) thread1.join();
    if (thread2.joinable()) thread2.join();
    
    std::cout << "Main: Both threads completed successfully.\n";
}

/**
 * @brief Demonstrates deadlock prevention using std::scoped_lock (C++17)
 */
void demonstrate_deadlock_prevention_with_scoped_lock() {
    std::cout << "\n=== Demonstrating Deadlock Prevention with std::scoped_lock (C++17) ===\n";
    std::cout << "In this scenario, threads use std::scoped_lock to acquire multiple locks atomically.\n";
    std::cout << "This is a simpler and safer alternative to std::lock with std::unique_lock.\n\n";

    // Thread 1 function: acquire both resources using std::scoped_lock
    auto thread1_func = []() {
        std::cout << "Thread 1: Starting\n";
        std::cout << "Thread 1: Trying to acquire both resources atomically\n";
        
        // Use std::scoped_lock to acquire both mutexes atomically (C++17)
        // This is a simpler and safer alternative to std::lock with std::unique_lock
        {
            std::scoped_lock lock(resource_a.get_mutex(), resource_b.get_mutex());
            
            std::cout << "Thread 1: Acquired both resources\n";
            
            // Work with both resources
            std::cout << "Thread 1: Working with both resources\n";
            resource_a.use();
            resource_b.use();
        }
        // scoped_lock automatically releases the mutexes when it goes out of scope
        
        std::cout << "Thread 1: Finished\n";
    };

    // Thread 2 function: acquire both resources using std::scoped_lock
    auto thread2_func = []() {
        std::cout << "Thread 2: Starting\n";
        std::cout << "Thread 2: Trying to acquire both resources atomically\n";
        
        // Use std::scoped_lock to acquire both mutexes atomically (C++17)
        // Note that the order of mutexes doesn't matter with scoped_lock
        {
            std::scoped_lock lock(resource_b.get_mutex(), resource_a.get_mutex());
            
            std::cout << "Thread 2: Acquired both resources\n";
            
            // Work with both resources
            std::cout << "Thread 2: Working with both resources\n";
            resource_b.use();
            resource_a.use();
        }
        // scoped_lock automatically releases the mutexes when it goes out of scope
        
        std::cout << "Thread 2: Finished\n";
    };

    // Create threads
    std::thread thread1(thread1_func);
    std::thread thread2(thread2_func);
    
    // Wait for threads to complete
    std::cout << "Main: Waiting for threads to complete...\n";
    
    if (thread1.joinable()) thread1.join();
    if (thread2.joinable()) thread2.join();
    
    std::cout << "Main: Both threads completed successfully.\n";
}

/**
 * @brief Demonstrates deadlock detection using std::try_lock
 */
void demonstrate_deadlock_detection() {
    std::cout << "\n=== Demonstrating Deadlock Detection ===\n";
    std::cout << "In this scenario, threads use std::try_lock to detect potential deadlocks.\n";
    std::cout << "If a lock cannot be acquired immediately, the thread can take alternative action.\n\n";

    // Thread 1 function: acquire resource A, then try to acquire resource B
    auto thread1_func = []() {
        std::cout << "Thread 1: Starting\n";
        std::cout << "Thread 1: Trying to acquire Resource A\n";
        
        // Acquire resource A
        std::lock_guard<std::mutex> lock_a(resource_a.get_mutex());
        std::cout << "Thread 1: Acquired Resource A\n";
        
        // Simulate some work
        std::cout << "Thread 1: Working with Resource A\n";
        resource_a.use();
        
        std::cout << "Thread 1: Trying to acquire Resource B (with try_lock)\n";
        
        // Try to acquire resource B without blocking
        bool acquired_b = resource_b.get_mutex().try_lock();
        
        if (acquired_b) {
            // Successfully acquired resource B
            std::cout << "Thread 1: Acquired Resource B\n";
            
            // Work with both resources
            std::cout << "Thread 1: Working with both resources\n";
            resource_a.use();
            resource_b.use();
            
            // Release resource B
            resource_b.get_mutex().unlock();
            std::cout << "Thread 1: Released Resource B\n";
        } else {
            // Failed to acquire resource B (potential deadlock detected)
            std::cout << "Thread 1: Could not acquire Resource B (potential deadlock detected)\n";
            std::cout << "Thread 1: Taking alternative action...\n";
            
            // Just continue with resource A only
            std::cout << "Thread 1: Continuing with Resource A only\n";
            resource_a.use();
        }
        
        std::cout << "Thread 1: Finished\n";
    };

    // Thread 2 function: acquire resource B, then try to acquire resource A
    auto thread2_func = []() {
        std::cout << "Thread 2: Starting\n";
        std::cout << "Thread 2: Trying to acquire Resource B\n";
        
        // Acquire resource B
        std::lock_guard<std::mutex> lock_b(resource_b.get_mutex());
        std::cout << "Thread 2: Acquired Resource B\n";
        
        // Simulate some work
        std::cout << "Thread 2: Working with Resource B\n";
        resource_b.use();
        
        std::cout << "Thread 2: Trying to acquire Resource A (with try_lock)\n";
        
        // Try to acquire resource A without blocking
        bool acquired_a = resource_a.get_mutex().try_lock();
        
        if (acquired_a) {
            // Successfully acquired resource A
            std::cout << "Thread 2: Acquired Resource A\n";
            
            // Work with both resources
            std::cout << "Thread 2: Working with both resources\n";
            resource_b.use();
            resource_a.use();
            
            // Release resource A
            resource_a.get_mutex().unlock();
            std::cout << "Thread 2: Released Resource A\n";
        } else {
            // Failed to acquire resource A (potential deadlock detected)
            std::cout << "Thread 2: Could not acquire Resource A (potential deadlock detected)\n";
            std::cout << "Thread 2: Taking alternative action...\n";
            
            // Just continue with resource B only
            std::cout << "Thread 2: Continuing with Resource B only\n";
            resource_b.use();
        }
        
        std::cout << "Thread 2: Finished\n";
    };

    // Create threads
    std::thread thread1(thread1_func);
    
    // Small delay to ensure Thread 1 starts first
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::thread thread2(thread2_func);
    
    // Wait for threads to complete
    std::cout << "Main: Waiting for threads to complete...\n";
    
    if (thread1.joinable()) thread1.join();
    if (thread2.joinable()) thread2.join();
    
    std::cout << "Main: Both threads completed successfully (deadlock was avoided).\n";
}

/**
 * @brief Demonstrates a hierarchical mutex implementation to prevent deadlocks
 */
class HierarchicalMutex {
private:
    std::mutex internal_mutex;
    const unsigned long hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
    
    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw std::logic_error("Mutex hierarchy violated");
        }
    }
    
    void update_hierarchy_value() {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
    
public:
    explicit HierarchicalMutex(unsigned long value) : hierarchy_value(value), previous_hierarchy_value(0) {}
    
    void lock() {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    
    void unlock() {
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }
    
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock()) {
            return false;
        }
        update_hierarchy_value();
        return true;
    }
};

// Initialize the thread-local hierarchy value
thread_local unsigned long HierarchicalMutex::this_thread_hierarchy_value(ULONG_MAX);

/**
 * @brief Demonstrates hierarchical mutexes to prevent deadlocks
 */
void demonstrate_hierarchical_mutex() {
    std::cout << "\n=== Demonstrating Hierarchical Mutex ===\n";
    std::cout << "In this scenario, mutexes have a hierarchy value.\n";
    std::cout << "Threads must acquire mutexes in descending hierarchy order.\n";
    std::cout << "This prevents the circular wait condition and avoids deadlock.\n\n";
    
    // Create hierarchical mutexes with different hierarchy values
    HierarchicalMutex high_mutex(10000);
    HierarchicalMutex mid_mutex(5000);
    HierarchicalMutex low_mutex(1000);
    
    // Thread function that acquires mutexes in correct hierarchy order (high to low)
    auto thread_correct_order = []() {
        HierarchicalMutex high_mutex(10000);
        HierarchicalMutex mid_mutex(5000);
        HierarchicalMutex low_mutex(1000);
        
        std::cout << "Thread Correct: Starting\n";
        
        // Acquire mutexes in descending hierarchy order (high to low)
        std::cout << "Thread Correct: Trying to acquire high mutex\n";
        std::lock_guard<HierarchicalMutex> high_lock(high_mutex);
        std::cout << "Thread Correct: Acquired high mutex\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "Thread Correct: Trying to acquire mid mutex\n";
        std::lock_guard<HierarchicalMutex> mid_lock(mid_mutex);
        std::cout << "Thread Correct: Acquired mid mutex\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "Thread Correct: Trying to acquire low mutex\n";
        std::lock_guard<HierarchicalMutex> low_lock(low_mutex);
        std::cout << "Thread Correct: Acquired low mutex\n";
        
        std::cout << "Thread Correct: All mutexes acquired successfully\n";
        std::cout << "Thread Correct: Finished\n";
    };
    
    // Thread function that tries to acquire mutexes in incorrect hierarchy order (low to high)
    auto thread_incorrect_order = []() {
        HierarchicalMutex high_mutex(10000);
        HierarchicalMutex mid_mutex(5000);
        HierarchicalMutex low_mutex(1000);
        
        std::cout << "Thread Incorrect: Starting\n";
        
        try {
            // Acquire low mutex first
            std::cout << "Thread Incorrect: Trying to acquire low mutex\n";
            std::lock_guard<HierarchicalMutex> low_lock(low_mutex);
            std::cout << "Thread Incorrect: Acquired low mutex\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            // Trying to acquire high mutex after low mutex (violates hierarchy)
            std::cout << "Thread Incorrect: Trying to acquire high mutex (violates hierarchy)\n";
            std::lock_guard<HierarchicalMutex> high_lock(high_mutex);
            std::cout << "Thread Incorrect: Acquired high mutex\n";
            
            std::cout << "Thread Incorrect: All mutexes acquired successfully\n";
        } catch (const std::logic_error& e) {
            std::cout << "Thread Incorrect: Exception caught: " << e.what() << std::endl;
            std::cout << "Thread Incorrect: This demonstrates how hierarchical mutexes prevent deadlock\n";
        }
        
        std::cout << "Thread Incorrect: Finished\n";
    };
    
    // Create threads
    std::thread thread1(thread_correct_order);
    
    // Small delay to ensure Thread 1 starts first
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::thread thread2(thread_incorrect_order);
    
    // Wait for threads to complete
    std::cout << "Main: Waiting for threads to complete...\n";
    
    if (thread1.joinable()) thread1.join();
    if (thread2.joinable()) thread2.join();
    
    std::cout << "Main: Both threads completed.\n";
}

/**
 * @brief Explains the four conditions necessary for a deadlock to occur
 */
void explain_deadlock_conditions() {
    std::cout << "\n=== The Four Conditions for Deadlock ===\n";
    std::cout << "For a deadlock to occur, all four of the following conditions must be true:\n\n";
    
    std::cout << "1. Mutual Exclusion: At least one resource must be held in a non-shareable mode.\n";
    std::cout << "   In our examples, mutexes provide mutual exclusion for resources.\n\n";
    
    std::cout << "2. Hold and Wait: A thread must be holding at least one resource and waiting to\n";
    std::cout << "   acquire additional resources that are currently held by other threads.\n";
    std::cout << "   In our deadlock scenario, Thread 1 holds Resource A and waits for Resource B,\n";
    std::cout << "   while Thread 2 holds Resource B and waits for Resource A.\n\n";
    
    std::cout << "3. No Preemption: Resources cannot be forcibly taken away from a thread.\n";
    std::cout << "   They must be explicitly released by the thread holding them.\n";
    std::cout << "   In our examples, mutexes cannot be forcibly released by another thread.\n\n";
    
    std::cout << "4. Circular Wait: A circular chain of threads exists, where each thread holds\n";
    std::cout << "   one or more resources that are being requested by the next thread in the chain.\n";
    std::cout << "   In our deadlock scenario, Thread 1 -> Resource A -> Thread 2 -> Resource B -> Thread 1\n";
    std::cout << "   forms a circular wait condition.\n\n";
    
    std::cout << "To prevent deadlock, we need to break at least one of these conditions.\n";
    std::cout << "Most deadlock prevention techniques focus on breaking the circular wait condition.\n";
}

/**
 * @brief Summarizes deadlock prevention techniques
 */
void summarize_deadlock_prevention() {
    std::cout << "\n=== Deadlock Prevention Techniques ===\n";
    std::cout << "Here are some common techniques to prevent deadlocks:\n\n";
    
    std::cout << "1. Lock Ordering: Always acquire locks in a consistent order.\n";
    std::cout << "   This breaks the circular wait condition.\n";
    std::cout << "   Example: std::lock and std::scoped_lock ensure locks are acquired in a consistent order.\n\n";
    
    std::cout << "2. Lock Timeout: Use timed lock acquisition to detect potential deadlocks.\n";
    std::cout << "   If a lock cannot be acquired within a timeout, release all locks and try again.\n";
    std::cout << "   Example: std::try_lock and std::mutex::try_lock_for.\n\n";
    
    std::cout << "3. Deadlock Detection: Use a deadlock detection algorithm to identify and recover\n";
    std::cout << "   from deadlocks after they occur.\n";
    std::cout << "   Example: Resource allocation graphs and wait-for graphs.\n\n";
    
    std::cout << "4. Lock Hierarchy: Assign a hierarchy to locks and always acquire them in\n";
    std::cout << "   descending hierarchy order.\n";
    std::cout << "   Example: Our HierarchicalMutex implementation.\n\n";
    
    std::cout << "5. Lock-Free Programming: Use atomic operations and lock-free data structures\n";
    std::cout << "   to avoid the need for locks altogether.\n";
    std::cout << "   Example: std::atomic<T> and lock-free algorithms.\n\n";
    
    std::cout << "6. Resource Allocation: Acquire all needed resources at once, or none at all.\n";
    std::cout << "   Example: std::lock and std::scoped_lock for multiple mutexes.\n\n";
}

int main() {
    std::cout << "=== Deadlock Example in C++ ===\n";
    
    // Explain deadlock conditions
    explain_deadlock_conditions();
    
    // Demonstrate deadlock scenario
    demonstrate_deadlock();
    
    // Demonstrate deadlock prevention using std::lock
    demonstrate_deadlock_prevention_with_std_lock();
    
    // Demonstrate deadlock prevention using std::scoped_lock (C++17)
    demonstrate_deadlock_prevention_with_scoped_lock();
    
    // Demonstrate deadlock detection using std::try_lock
    demonstrate_deadlock_detection();
    
    // Demonstrate hierarchical mutex
    demonstrate_hierarchical_mutex();
    
    // Summarize deadlock prevention techniques
    summarize_deadlock_prevention();
    
    return 0;
}

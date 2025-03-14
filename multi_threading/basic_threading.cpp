/**
 * @file basic_threading.cpp
 * @brief Demonstrates basic threading concepts in C++ using std::thread
 * 
 * This example shows:
 * 1. Creating threads with std::thread
 * 2. Passing arguments to threads
 * 3. Joining threads
 * 4. Thread synchronization with std::mutex
 * 5. RAII lock guards
 * 
 * Compilation: g++ -std=c++17 -o basic_threading basic_threading.cpp -pthread
 * Usage: ./basic_threading
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <functional>

// Constants
constexpr int NUM_THREADS = 5;
constexpr int ITERATIONS = 1000000;

// Global counter (shared resource)
long counter = 0;

// Mutex for protecting the counter
std::mutex counter_mutex;

/**
 * @brief Thread function that increments a counter without synchronization
 * @param thread_id The ID of the thread
 */
void increment_without_mutex(int thread_id) {
    std::cout << "Thread " << thread_id << " starting...\n";
    
    // Increment counter without synchronization (can cause race conditions)
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;  // UNSAFE: Race condition possible here
    }
    
    std::cout << "Thread " << thread_id << " completed\n";
}

/**
 * @brief Thread function that increments a counter with mutex synchronization
 * @param thread_id The ID of the thread
 */
void increment_with_mutex(int thread_id) {
    std::cout << "Thread " << thread_id << " starting...\n";
    
    // Increment counter with synchronization
    for (int i = 0; i < ITERATIONS; i++) {
        // Lock mutex before accessing shared resource using RAII lock_guard
        {
            std::lock_guard<std::mutex> lock(counter_mutex);
            // Critical section (protected by mutex)
            counter++;
        }
        // lock_guard automatically releases the mutex when it goes out of scope
    }
    
    std::cout << "Thread " << thread_id << " completed\n";
}

/**
 * @brief Demonstrates thread creation and execution without synchronization
 */
void demonstrate_without_mutex() {
    std::vector<std::thread> threads;
    
    std::cout << "\n=== Demonstration WITHOUT Mutex (Race Condition) ===\n";
    
    // Reset counter
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(increment_without_mutex, i);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Print final counter value
    std::cout << "Final counter value (expected: " << NUM_THREADS * ITERATIONS 
              << "): " << counter << std::endl;
    
    // Check if race condition occurred
    if (counter != static_cast<long>(NUM_THREADS) * ITERATIONS) {
        std::cout << "Race condition detected! Counter value is incorrect.\n";
    }
}

/**
 * @brief Demonstrates thread creation and execution with mutex synchronization
 */
void demonstrate_with_mutex() {
    std::vector<std::thread> threads;
    
    std::cout << "\n=== Demonstration WITH Mutex (Thread-Safe) ===\n";
    
    // Reset counter
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(increment_with_mutex, i);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Print final counter value
    std::cout << "Final counter value (expected: " << NUM_THREADS * ITERATIONS 
              << "): " << counter << std::endl;
    
    // Check if counter is correct
    if (counter == static_cast<long>(NUM_THREADS) * ITERATIONS) {
        std::cout << "Success! Counter value is correct.\n";
    }
}

/**
 * @brief Demonstrates passing data to threads and using lambda functions
 */
void demonstrate_lambda_and_data_passing() {
    std::cout << "\n=== Demonstration of Lambda and Data Passing ===\n";
    
    // Data to pass to threads
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::mutex cout_mutex; // Mutex for protecting cout
    
    // Create threads using lambda functions
    std::vector<std::thread> threads;
    for (int i = 0; i < data.size(); i++) {
        // Lambda function captures variables by reference
        threads.emplace_back([&data, &cout_mutex, i]() {
            // Process data
            int result = data[i] * data[i];
            
            // Use mutex to protect cout (prevent interleaved output)
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Thread processing data[" << i << "] = " << data[i] 
                          << ", result = " << result << std::endl;
            }
            
            // Modify data (safe because each thread modifies a different element)
            data[i] = result;
            
            // Sleep to simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * i));
        });
    }
    
    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // Print final data
    std::cout << "Final data after processing: ";
    for (const auto& val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== Basic Threading Example in C++ ===\n";
    std::cout << "Number of threads: " << NUM_THREADS << std::endl;
    std::cout << "Iterations per thread: " << ITERATIONS << std::endl;
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads\n";
    
    // Demonstrate thread creation without mutex (race condition)
    demonstrate_without_mutex();
    
    // Demonstrate thread creation with mutex (thread-safe)
    demonstrate_with_mutex();
    
    // Demonstrate lambda functions and data passing
    demonstrate_lambda_and_data_passing();
    
    return EXIT_SUCCESS;
}

/**
 * @file multithreading_interview_guide.cpp
 * @brief Comprehensive guide to multithreading concepts in C++ for interview preparation
 * 
 * This guide covers:
 * 1. Basic thread creation and management
 * 2. Thread synchronization (mutexes, locks)
 * 3. Thread communication (condition variables, futures)
 * 4. Advanced concepts (atomics, thread pools)
 * 5. Common threading problems and solutions
 * 
 * Compilation: g++ -std=c++17 -o multithreading_interview_guide multithreading_interview_guide.cpp -pthread
 * Usage: ./multithreading_interview_guide
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <functional>
#include <chrono>
#include <string>
#include <memory>

// For cleaner output
std::mutex cout_mutex;
#define THREAD_SAFE_COUT(msg) { \
    std::lock_guard<std::mutex> lock(cout_mutex); \
    std::cout << msg << std::endl; \
}

/**
 * SECTION 1: BASIC THREADING CONCEPTS
 * ==================================
 * - Thread creation and management
 * - Joining vs Detaching
 * - Thread arguments and return values
 */
namespace BasicThreading {
    void demonstrate() {
        THREAD_SAFE_COUT("\n=== SECTION 1: BASIC THREADING CONCEPTS ===");
        
        // 1. Creating a thread with a function
        THREAD_SAFE_COUT("1.1 Creating threads with functions");
        
        // Simple thread function
        auto threadFunc = [](int id, const std::string& message) {
            THREAD_SAFE_COUT("Thread " << id << " says: " << message);
        };
        
        // Create and start the thread
        std::thread t1(threadFunc, 1, "Hello from thread!");
        
        // 2. Joining threads (wait for completion)
        THREAD_SAFE_COUT("1.2 Joining threads (waiting for completion)");
        t1.join(); // Main thread waits for t1 to finish
        
        // 3. Detaching threads (run independently)
        THREAD_SAFE_COUT("1.3 Detaching threads (running independently)");
        std::thread t2(threadFunc, 2, "I'm a detached thread");
        t2.detach(); // t2 runs independently, main doesn't wait
        
        // Sleep to give detached thread time to execute
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 4. Thread ID and hardware concurrency
        THREAD_SAFE_COUT("1.4 Thread IDs and hardware concurrency");
        THREAD_SAFE_COUT("Main thread ID: " << std::this_thread::get_id());
        THREAD_SAFE_COUT("Hardware concurrency: " << std::thread::hardware_concurrency() << " threads");
        
        // 5. Passing arguments to threads
        THREAD_SAFE_COUT("1.5 Passing arguments to threads");
        
        // By value
        int val = 42;
        std::string str = "Original string";
        
        std::thread t3([](int x, std::string s) {
            THREAD_SAFE_COUT("Passed by value: " << x << ", " << s);
            // Modifying local copies doesn't affect originals
            x = 100;
            s = "Modified string";
        }, val, str);
        
        t3.join();
        THREAD_SAFE_COUT("After thread: val=" << val << ", str=" << str);
        
        // By reference (using std::ref)
        std::thread t4([](int& x, std::string& s) {
            THREAD_SAFE_COUT("Passed by reference: " << x << ", " << s);
            // Modifying references affects originals
            x = 100;
            s = "Modified string";
        }, std::ref(val), std::ref(str));
        
        t4.join();
        THREAD_SAFE_COUT("After thread with ref: val=" << val << ", str=" << str);
        
        THREAD_SAFE_COUT("Section 1 completed");
    }
}

/**
 * SECTION 2: SYNCHRONIZATION MECHANISMS
 * ====================================
 * - Mutexes (std::mutex, std::recursive_mutex)
 * - Locks (std::lock_guard, std::unique_lock, std::scoped_lock)
 * - Condition variables
 * - Shared mutexes (readers-writer locks)
 */
namespace Synchronization {
    // Shared data
    int shared_counter = 0;
    std::mutex counter_mutex;
    std::recursive_mutex recursive_mtx;
    std::shared_mutex shared_mtx;
    
    // For condition variable example
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::queue<int> task_queue;
    bool finished = false;
    
    void demonstrate() {
        THREAD_SAFE_COUT("\n=== SECTION 2: SYNCHRONIZATION MECHANISMS ===");
        
        // 1. Basic mutex usage
        THREAD_SAFE_COUT("2.1 Basic mutex with lock_guard (RAII)");
        
        auto increment = [](int iterations) {
            for (int i = 0; i < iterations; i++) {
                std::lock_guard<std::mutex> lock(counter_mutex); // RAII lock
                shared_counter++; // Protected by mutex
            }
        };
        
        shared_counter = 0;
        std::vector<std::thread> threads;
        for (int i = 0; i < 5; i++) {
            threads.emplace_back(increment, 1000);
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        THREAD_SAFE_COUT("Counter after 5 threads with 1000 increments each: " 
                        << shared_counter << " (expected: 5000)");
        
        // 2. unique_lock (more flexible than lock_guard)
        THREAD_SAFE_COUT("2.2 unique_lock (more flexible than lock_guard)");
        
        auto unique_lock_demo = []() {
            std::unique_lock<std::mutex> lock(counter_mutex);
            shared_counter++; // Protected by mutex
            
            // Can unlock and relock with unique_lock
            lock.unlock();
            // Unprotected section
            int local_computation = shared_counter * 2;
            
            lock.lock();
            shared_counter += local_computation; // Protected again
            
            // lock is automatically released when function returns
        };
        
        shared_counter = 0;
        std::thread t_unique(unique_lock_demo);
        t_unique.join();
        
        THREAD_SAFE_COUT("Counter after unique_lock demo: " << shared_counter);
        
        // 3. Condition variables
        THREAD_SAFE_COUT("2.3 Condition variables for thread notification");
        
        // Producer thread
        std::thread producer([&]() {
            for (int i = 0; i < 5; i++) {
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    task_queue.push(i);
                    THREAD_SAFE_COUT("Producer: Added task " << i);
                }
                
                // Notify one waiting thread that data is ready
                cv.notify_one();
                
                // Small delay between tasks
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            // Signal that no more data is coming
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                finished = true;
            }
            // Notify all waiting threads that we're done
            cv.notify_all();
        });
        
        // Consumer threads
        std::vector<std::thread> consumers;
        for (int i = 0; i < 2; i++) {
            consumers.emplace_back([i]() {
                while (true) {
                    int task;
                    {
                        // Use unique_lock because condition_variable needs it
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        
                        // Wait until queue has data or finished flag is set
                        cv.wait(lock, []{ 
                            return !task_queue.empty() || finished; 
                        });
                        
                        // If queue is empty and we're finished, exit
                        if (task_queue.empty() && finished) {
                            break;
                        }
                        
                        // Get task from queue
                        task = task_queue.front();
                        task_queue.pop();
                    }
                    
                    // Process task outside the lock
                    THREAD_SAFE_COUT("Consumer " << i << ": Processing task " << task);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                THREAD_SAFE_COUT("Consumer " << i << ": Finished");
            });
        }
        
        producer.join();
        for (auto& t : consumers) {
            t.join();
        }
        
        THREAD_SAFE_COUT("Section 2 completed");
    }
}

/**
 * SECTION 3: ADVANCED THREADING CONCEPTS
 * =====================================
 * - Atomic operations
 * - Memory ordering
 * - Futures and promises
 * - Async and packaged tasks
 */
namespace AdvancedConcepts {
    // Atomic variables
    std::atomic<int> atomic_counter(0);
    
    void demonstrate() {
        THREAD_SAFE_COUT("\n=== SECTION 3: ADVANCED THREADING CONCEPTS ===");
        
        // 1. Atomic operations
        THREAD_SAFE_COUT("3.1 Atomic operations (lock-free synchronization)");
        
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; i++) {
            threads.emplace_back([]() {
                for (int j = 0; j < 1000; j++) {
                    // Atomic increment (no mutex needed)
                    atomic_counter++;
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        THREAD_SAFE_COUT("Atomic counter after 10 threads with 1000 increments each: " 
                        << atomic_counter.load() << " (expected: 10000)");
        
        // 2. Memory ordering
        THREAD_SAFE_COUT("3.2 Memory ordering semantics");
        
        std::atomic<int> x(0), y(0);
        
        std::thread t1([&]() {
            // Release operation - all writes before this are visible to
            // any thread that performs an acquire operation on the same atomic
            x.store(1, std::memory_order_release);
        });
        
        std::thread t2([&]() {
            // Acquire operation - all reads after this will see writes
            // that happened before the release operation
            while (x.load(std::memory_order_acquire) == 0) {
                // Spin until x is set by t1
                std::this_thread::yield();
            }
            
            // This will see all writes that happened before x.store in t1
            y.store(1, std::memory_order_relaxed);
        });
        
        t1.join();
        t2.join();
        
        THREAD_SAFE_COUT("After memory ordering example: x=" << x.load() 
                        << ", y=" << y.load());
        
        // 3. Futures and promises
        THREAD_SAFE_COUT("3.3 Futures and promises for one-time data transfer");
        
        // Create a promise for a future result
        std::promise<std::string> promise;
        
        // Get the future associated with the promise
        std::future<std::string> future = promise.get_future();
        
        // Thread that will fulfill the promise
        std::thread producer_thread([&promise]() {
            THREAD_SAFE_COUT("Producer thread working on result...");
            
            // Simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            // Set the result
            try {
                promise.set_value("Result from producer thread");
            } catch (const std::future_error& e) {
                THREAD_SAFE_COUT("Error setting promise value: " << e.what());
            }
        });
        
        // Main thread waits for the result
        THREAD_SAFE_COUT("Main thread waiting for result...");
        
        // Block until the future has a value
        std::string result = future.get();
        
        THREAD_SAFE_COUT("Main thread received: " << result);
        
        producer_thread.join();
        
        // 4. Async and packaged tasks
        THREAD_SAFE_COUT("3.4 Async and packaged tasks");
        
        // Function that returns a result after some work
        auto calculate = [](int x, int y) -> int {
            THREAD_SAFE_COUT("Calculating " << x << " + " << y);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return x + y;
        };
        
        // Launch async task (may run in a separate thread)
        THREAD_SAFE_COUT("Launching async task...");
        std::future<int> async_result = std::async(std::launch::async, calculate, 40, 2);
        
        // Do other work while calculation is happening
        THREAD_SAFE_COUT("Main thread doing other work...");
        
        // Wait for and get the result
        int sum = async_result.get();
        THREAD_SAFE_COUT("Async result: " << sum);
        
        THREAD_SAFE_COUT("Section 3 completed");
    }
}

/**
 * SECTION 4: THREAD POOLS AND PRACTICAL PATTERNS
 * ============================================
 * - Thread pool implementation
 * - Producer-consumer pattern
 */
namespace ThreadingPatterns {
    // Simple thread pool implementation
    class ThreadPool {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
        
    public:
        ThreadPool(size_t num_threads) : stop(false) {
            for (size_t i = 0; i < num_threads; ++i) {
                workers.emplace_back([this, i] {
                    THREAD_SAFE_COUT("Thread pool worker " << i << " started");
                    
                    while (true) {
                        std::function<void()> task;
                        
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            
                            // Wait for task or stop signal
                            condition.wait(lock, [this] { 
                                return stop || !tasks.empty(); 
                            });
                            
                            // Exit if stopping and no more tasks
                            if (stop && tasks.empty()) {
                                THREAD_SAFE_COUT("Thread pool worker " << i << " stopping");
                                return;
                            }
                            
                            // Get task from queue
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        
                        // Execute task
                        task();
                    }
                });
            }
        }
        
        // Add task to the pool
        template<class F>
        void enqueue(F&& f) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                
                // Don't allow enqueueing after stopping the pool
                if (stop) {
                    throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
                }
                
                // Add task to queue
                tasks.emplace(std::forward<F>(f));
            }
            
            // Notify one waiting thread
            condition.notify_one();
        }
        
        // Destructor - join all threads
        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            
            // Notify all threads to check stop condition
            condition.notify_all();
            
            // Join all threads
            for (std::thread& worker : workers) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
            
            THREAD_SAFE_COUT("Thread pool destroyed");
        }
    };
    
    void demonstrate() {
        THREAD_SAFE_COUT("\n=== SECTION 4: THREAD POOLS AND PRACTICAL PATTERNS ===");
        
        // Thread pool example
        THREAD_SAFE_COUT("4.1 Thread pool implementation");
        
        // Create thread pool with 4 worker threads
        ThreadPool pool(4);
        
        // Enqueue tasks
        for (int i = 0; i < 8; ++i) {
            pool.enqueue([i] {
                THREAD_SAFE_COUT("Task " << i << " running");
                
                // Simulate different workloads
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i % 3 + 1)));
                
                THREAD_SAFE_COUT("Task " << i << " completed");
            });
        }
        
        // Sleep to allow tasks to complete
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        THREAD_SAFE_COUT("Section 4 completed");
    }
}

/**
 * SECTION 5: COMMON THREADING PROBLEMS
 * ==================================
 * - Race conditions
 * - Deadlocks
 * - Thread safety techniques
 */
namespace ThreadingProblems {
    void demonstrate() {
        THREAD_SAFE_COUT("\n=== SECTION 5: COMMON THREADING PROBLEMS ===");
        
        // 1. Race condition demonstration
        THREAD_SAFE_COUT("5.1 Race condition demonstration");
        
        int counter = 0;
        std::vector<std::thread> threads;
        
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < 1000; ++j) {
                    // UNSAFE: Race condition
                    counter++;
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        THREAD_SAFE_COUT("Counter after race condition: " << counter 
                        << " (expected: 5000, but likely less due to race conditions)");
        
        // 2. Deadlock prevention
        THREAD_SAFE_COUT("5.2 Deadlock prevention techniques");
        
        std::mutex m1, m2;
        
        // Using std::scoped_lock (C++17) to prevent deadlocks
        auto safe_locking = [&m1, &m2](int id) {
            // scoped_lock acquires multiple locks atomically, preventing deadlock
            std::scoped_lock lock(m1, m2);
            THREAD_SAFE_COUT("Thread " << id << " safely got both locks with scoped_lock");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        };
        
        std::thread t1(safe_locking, 1);
        std::thread t2(safe_locking, 2);
        
        t1.join();
        t2.join();
        
        THREAD_SAFE_COUT("Section 5 completed");
    }
}

/**
 * SECTION 6: INTERVIEW QUESTIONS AND ANSWERS
 * =======================================
 * Key multithreading concepts for interviews
 */
void printInterviewQuestions() {
    std::cout << "\n=== SECTION 6: INTERVIEW QUESTIONS AND ANSWERS ===\n";
    
    std::cout << "1. What is the difference between process and thread?\n"
              << "   - A process has its own memory space, file handles, and system resources\n"
              << "   - Threads share the same memory space within a process\n"
              << "   - Context switching between threads is faster than between processes\n"
              << "   - Threads are more lightweight than processes\n\n";
    
    std::cout << "2. What is a race condition?\n"
              << "   - A race condition occurs when multiple threads access shared data concurrently\n"
              << "     and at least one thread modifies the data\n"
              << "   - The outcome depends on the relative timing of the threads, making it unpredictable\n"
              << "   - Race conditions can lead to data corruption and program crashes\n\n";
    
    std::cout << "3. How do you prevent race conditions?\n"
              << "   - Use mutexes to protect critical sections\n"
              << "   - Use atomic operations for simple data types\n"
              << "   - Design for immutability where possible\n"
              << "   - Use thread-local storage to avoid sharing\n\n";
    
    std::cout << "4. What is a deadlock and how do you prevent it?\n"
              << "   - A deadlock occurs when two or more threads are blocked forever,\n"
              << "     each waiting for resources held by the other\n"
              << "   - Prevention techniques include:\n"
              << "     - Lock ordering (always acquire locks in the same order)\n"
              << "     - Use std::lock or std::scoped_lock for multiple locks\n"
              << "     - Use timeouts to avoid indefinite waiting\n"
              << "     - Avoid nested locks\n\n";
    
    std::cout << "5. What is the difference between std::lock_guard and std::unique_lock?\n"
              << "   - std::lock_guard is a simple RAII wrapper that locks on construction\n"
              << "     and unlocks on destruction\n"
              << "   - std::unique_lock is more flexible, allowing unlock/lock operations\n"
              << "     during its lifetime, deferred locking, and use with condition variables\n\n";
    
    std::cout << "6. What is a condition variable and when would you use it?\n"
              << "   - A condition variable is a synchronization primitive used to block\n"
              << "     a thread until a condition is met\n"
              << "   - Used for signaling between threads (e.g., producer-consumer pattern)\n"
              << "   - Allows threads to wait efficiently without busy-waiting\n\n";
    
    std::cout << "7. What is std::async and how does it differ from std::thread?\n"
              << "   - std::async is a high-level way to run a function asynchronously\n"
              << "   - It returns a std::future that will contain the result\n"
              << "   - The system may optimize by running the task in the current thread\n"
              << "     or a thread pool\n"
              << "   - std::thread always creates a new thread\n\n";
    
    std::cout << "8. What is a thread pool and why would you use it?\n"
              << "   - A thread pool is a collection of worker threads that can be reused\n"
              << "     to execute tasks\n"
              << "   - Benefits include:\n"
              << "     - Reduced overhead of thread creation/destruction\n"
              << "     - Control over system resource usage\n"
              << "     - Load balancing across available cores\n"
              << "     - Limiting the number of concurrent threads\n\n";
    
    std::cout << "9. How would you implement a thread-safe singleton?\n"
              << "   - Use double-checked locking pattern with std::atomic\n"
              << "   - Use static local variable (C++11 guarantees thread-safe initialization)\n"
              << "   - Use call_once with once_flag\n\n";
    
    std::cout << "10. What are atomic operations and when would you use them?\n"
              << "    - Atomic operations are indivisible operations that cannot be\n"
              << "      interrupted by other threads\n"
              << "    - Used for simple operations on shared data without mutexes\n"
              << "    - Provide better performance for simple operations\n"
              << "    - Common in lock-free algorithms\n\n";
}

int main() {
    std::cout << "=== C++ MULTITHREADING INTERVIEW GUIDE ===\n";
    std::cout << "This guide demonstrates key multithreading concepts for interviews\n";
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads\n";
    
    // Demonstrate basic threading concepts
    BasicThreading::demonstrate();
    
    // Demonstrate synchronization mechanisms
    Synchronization::demonstrate();
    
    // Demonstrate advanced threading concepts
    AdvancedConcepts::demonstrate();
    
    // Demonstrate thread pools and patterns
    ThreadingPatterns::demonstrate();
    
    // Demonstrate common threading problems
    ThreadingProblems::demonstrate();
    
    // Print interview questions and answers
    printInterviewQuestions();
    
    std::cout << "\n=== C++ MULTITHREADING INTERVIEW GUIDE COMPLETED ===\n";
    return 0;
}

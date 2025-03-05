#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

/**
 * @brief Thread-safe Singleton Design Pattern Implementation
 * 
 * This implementation uses:
 * 1. unique_ptr for automatic memory management
 * 2. std::mutex for thread synchronization
 * 3. Custom exception for handling multiple instance creation attempts
 * 4. Double-checked locking pattern for performance
 */

// Custom exception for singleton violation attempts
class SingletonViolationException : public std::runtime_error {
public:
    SingletonViolationException(const std::string& message)
        : std::runtime_error(message) {}
};

class ThreadSafeSingleton {
private:
    // Private constructor to prevent direct instantiation
    ThreadSafeSingleton() {
        std::cout << "Singleton instance created at " << this << std::endl;
        // Simulate some initialization work
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // Declare destructor as private but allow unique_ptr to access it via the Deleter class
    ~ThreadSafeSingleton() {
        std::cout << "Singleton instance destroyed at " << this << std::endl;
    }
    
    // Delete copy and move constructors/assignment operators
    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton(ThreadSafeSingleton&&) = delete;
    ThreadSafeSingleton& operator=(ThreadSafeSingleton&&) = delete;
    
    // Custom deleter class that has access to the private destructor
    class Deleter {
    public:
        void operator()(ThreadSafeSingleton* p) const {
            delete p;
        }
    };
    
    // Static unique_ptr with custom deleter to hold the singleton instance
    static std::unique_ptr<ThreadSafeSingleton, Deleter> instance;
    
    // Mutex for thread synchronization
    static std::mutex mutex;
    
    // Flag to track if instance creation was attempted - make it accessible through a static method
    static bool creationAttempted;
    
    // Friend function to test violation
    friend void demonstrateViolation();

public:
    // Static method to get the singleton instance
    static ThreadSafeSingleton& getInstance() {
        // Fast check without locking (Double-checked locking pattern)
        if (!instance) {
            // Lock the mutex before checking again
            std::lock_guard<std::mutex> lock(mutex);
            
            if (!instance) {
                if (creationAttempted) {
                    throw SingletonViolationException(
                        "Attempted to create more than one singleton instance");
                }
                
                // Set flag before creating the instance
                creationAttempted = true;
                
                // Create the singleton instance using unique_ptr with custom deleter
                instance = std::unique_ptr<ThreadSafeSingleton, Deleter>(new ThreadSafeSingleton());
            }
        }
        
        return *instance;
    }
    
    // Example method to perform some operation
    void someOperation() const {
        std::cout << "Performing operation in Singleton at " << this << std::endl;
    }
    
    // For demonstration purposes: Reset the singleton (usually not needed)
    static void reset() {
        std::lock_guard<std::mutex> lock(mutex);
        instance.reset();
        creationAttempted = false;
        std::cout << "Singleton has been reset" << std::endl;
    }
    
    // Method to check if creation was attempted
    static bool wasCreationAttempted() {
        return creationAttempted;
    }
    
    // Method to set creation attempted flag for demonstration purposes
    static void setCreationAttempted(bool value) {
        std::lock_guard<std::mutex> lock(mutex);
        creationAttempted = value;
    }
};

// Initialize static members
std::unique_ptr<ThreadSafeSingleton, ThreadSafeSingleton::Deleter> ThreadSafeSingleton::instance;
std::mutex ThreadSafeSingleton::mutex;
bool ThreadSafeSingleton::creationAttempted = false;

// Function to demonstrate concurrent access
void accessSingleton(int threadId) {
    try {
        std::cout << "Thread " << threadId << " attempting to access singleton..." << std::endl;
        
        // Get the singleton instance and use it
        ThreadSafeSingleton& singleton = ThreadSafeSingleton::getInstance();
        
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Use the singleton
        singleton.someOperation();
        
        std::cout << "Thread " << threadId << " successfully accessed singleton" << std::endl;
    }
    catch (const SingletonViolationException& e) {
        std::cerr << "Thread " << threadId << " error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Thread " << threadId << " unexpected error: " << e.what() << std::endl;
    }
}

// Function to demonstrate singleton violation attempt
void demonstrateViolation() {
    try {
        std::cout << "\nAttempting to create a second instance after reset..." << std::endl;
        
        // Get the instance once
        ThreadSafeSingleton& first = ThreadSafeSingleton::getInstance();
        first.someOperation();
        
        // Try to create another instance by setting flag but keeping the original
        ThreadSafeSingleton::setCreationAttempted(true);
        
        // This should throw an exception
        ThreadSafeSingleton& second = ThreadSafeSingleton::getInstance();
        second.someOperation();
    }
    catch (const SingletonViolationException& e) {
        std::cerr << "Caught expected exception: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Thread-Safe Singleton Pattern Demonstration ===" << std::endl;
    
    // PART 1: Test thread safety with multiple threads
    {
        std::cout << "\n1. Testing concurrent access with multiple threads:" << std::endl;
        
        const int numThreads = 5;
        std::vector<std::thread> threads;
        
        // Create multiple threads that try to access the singleton
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(accessSingleton, i + 1);
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
        
        // Verify we still have one instance
        ThreadSafeSingleton& instance = ThreadSafeSingleton::getInstance();
        std::cout << "\nVerified singleton instance at: " << &instance << std::endl;
    }
    
    // PART 2: Reset and demonstrate violation detection
    {
        std::cout << "\n2. Resetting the singleton:" << std::endl;
        ThreadSafeSingleton::reset();
        
        // Test normal usage after reset
        std::cout << "\nCreating a new instance after reset:" << std::endl;
        ThreadSafeSingleton& instance = ThreadSafeSingleton::getInstance();
        instance.someOperation();
    }
    
    // PART 3: Demonstrate violation detection
    {
        std::cout << "\n3. Testing violation detection:" << std::endl;
        ThreadSafeSingleton::reset();
        demonstrateViolation();
    }
    
    std::cout << "\n=== End of Demonstration ===" << std::endl;
    
    return 0;
}

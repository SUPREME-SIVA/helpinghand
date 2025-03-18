#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <chrono>

/**
 * Thread-Safe Singleton Pattern: Interview Guide
 * =============================================
 * 
 * Key Points for Interviews:
 * 
 * 1. Definition:
 *    - A singleton ensures a class has only one instance and provides global access to it
 *    - Used when exactly one object is needed to coordinate actions across the system
 * 
 * 2. Thread Safety Requirements:
 *    - Double-checked locking pattern prevents race conditions during initialization
 *    - std::mutex ensures thread-safe access to the singleton instance
 *    - std::call_once with std::once_flag is an alternative approach (shown in comments)
 * 
 * 3. Modern C++ Implementation:
 *    - Use std::unique_ptr for automatic memory management
 *    - Make constructor private to prevent external instantiation
 *    - Delete copy/move constructors and assignment operators
 *    - Use a custom deleter for unique_ptr if the destructor is private
 * 
 * 4. Challenges and Solutions:
 *    - PROBLEM: Thread safety during initialization
 *      SOLUTION: Double-checked locking or std::call_once
 *    
 *    - PROBLEM: Memory leaks
 *      SOLUTION: std::unique_ptr for automatic cleanup
 *    
 *    - PROBLEM: Access to private destructor with unique_ptr
 *      SOLUTION: Custom deleter or friend class
 *    
 *    - PROBLEM: Detecting multiple instance creation attempts
 *      SOLUTION: Track creation attempts and throw exception or warning
 * 
 * 5. When to Use:
 *    - Logging facilities
 *    - Configuration managers
 *    - Thread and connection pools
 *    - Device drivers or hardware interface classes
 *    - Cache implementations
 */

// Custom exception for singleton violation attempts
class SingletonViolationException : public std::runtime_error {
public:
    SingletonViolationException(const std::string& message)
        : std::runtime_error(message) {}
};

class Singleton {
private:
    // Private constructor prevents direct instantiation
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
        // Simulate expensive initialization
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Private destructor
    ~Singleton() {
        std::cout << "Singleton instance destroyed" << std::endl;
    }
    
    // Delete copy and move operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
    // Custom deleter to access private destructor
    struct Deleter {
        void operator()(Singleton* p) const { 
            delete p; 
        }
    };
    
    // Thread synchronization
    static std::mutex mutex_;
    
    // Singleton instance with custom deleter
    static std::unique_ptr<Singleton, Deleter> instance_;
    
    // Flag to track if instance creation was attempted
    static bool creation_attempted_;
    
    // Alternative: once_flag for call_once implementation
    // static std::once_flag init_flag_;

public:
    // Thread-safe instance accessor using double-checked locking
    static Singleton& getInstance() {
        // First check without lock
        if (!instance_) {
            // Lock if instance doesn't exist
            std::lock_guard<std::mutex> lock(mutex_);
            
            // Second check after acquiring lock
            if (!instance_) {
                // Check if this is the first creation attempt
                if (creation_attempted_) {
                    std::cerr << "\n[WARNING] Attempted to create a second instance of Singleton!" << std::endl;
                    std::cerr << "This violates the Singleton pattern. Returning existing instance instead.\n" << std::endl;
                    // Optionally, you could throw an exception instead:
                    // throw SingletonViolationException("Attempted to create more than one Singleton instance");
                }
                
                // Set flag before creating instance
                creation_attempted_ = true;
                
                // Create instance with custom deleter
                instance_ = std::unique_ptr<Singleton, Deleter>(new Singleton());
            } else {
                // Instance was created by another thread while we were waiting for the lock
                std::cerr << "\n[WARNING] Thread race detected! Another thread already created the Singleton instance." << std::endl;
                std::cerr << "Returning existing instance instead.\n" << std::endl;
            }
        } else if (!creation_attempted_) {
            // This should never happen in practice (instance exists but flag not set)
            std::lock_guard<std::mutex> lock(mutex_);
            creation_attempted_ = true;
            std::cerr << "\n[ERROR] Inconsistent Singleton state detected! Instance exists but creation_attempted_ flag is false.\n" << std::endl;
        }
        return *instance_;
    }
    
    // Method to explicitly check if creation was attempted but failed
    static bool wasCreationAttempted() {
        std::lock_guard<std::mutex> lock(mutex_);
        return creation_attempted_;
    }
    
    // Method to simulate a second instance creation attempt (for demonstration)
    static void demonstrateViolation() {
        std::cout << "\nAttempting to create a second Singleton instance..." << std::endl;
        
        // Force creation_attempted to false to simulate violation
        {
            std::lock_guard<std::mutex> lock(mutex_);
            // This is just for demonstration!
            if (instance_) {
                creation_attempted_ = false;
            }
        }
        
        // Try to get instance (should trigger warning)
        Singleton& instance = getInstance();
        instance.doSomething();
    }
    
    // Alternative implementation using std::call_once
    /*
    static Singleton& getInstanceWithCallOnce() {
        std::call_once(init_flag_, []() {
            instance_ = std::unique_ptr<Singleton, Deleter>(new Singleton());
        });
        return *instance_;
    }
    */
    
    // Some business method
    void doSomething() {
        std::cout << "Singleton is doing something" << std::endl;
    }
};

// Initialize static members
std::mutex Singleton::mutex_;
std::unique_ptr<Singleton, Singleton::Deleter> Singleton::instance_;
bool Singleton::creation_attempted_ = false;
// std::once_flag Singleton::init_flag_; // For call_once implementation

/**
 * Interview Questions and Answers
 * ===============================
 * 
 * Q: What are the advantages of using unique_ptr for a Singleton?
 * A: - Automatic memory management prevents leaks
 *    - Clear ownership semantics
 *    - No need for explicit deletion in a destructor
 *    - Exception safety during initialization
 * 
 * Q: How do you ensure thread safety in a Singleton?
 * A: - Mutex to protect critical sections
 *    - Double-checked locking pattern for efficiency
 *    - Alternative: std::call_once with std::once_flag
 *    - Proper memory ordering with C++11's memory model
 * 
 * Q: How do you prevent multiple instances of a Singleton?
 * A: - Private constructor
 *    - Delete copy and move constructors/operators
 *    - Static instance accessible only through getInstance()
 *    - Track creation attempts and issue warning or throw exception
 * 
 * Q: What are the downsides of Singleton pattern?
 * A: - Global state can make testing difficult
 *    - Hidden dependencies
 *    - Threading issues if not implemented correctly
 *    - Potential violation of single responsibility principle
 *    - Can make code less modular
 * 
 * Q: How does double-checked locking work?
 * A: - First check without lock for performance
 *    - If instance doesn't exist, acquire lock
 *    - Check again after acquiring lock (prevents race condition)
 *    - Create instance only if still needed
 * 
 * Q: What's the difference between Singleton and static class?
 * A: - Singleton can implement interfaces and be passed as parameter
 *    - Singleton can have state that's initialized at runtime
 *    - Singleton allows lazy initialization
 *    - Singleton can be extended through inheritance
 */

// Simple demonstration function
void demonstrateSingleton() {
    // Get the singleton instance
    Singleton& singleton = Singleton::getInstance();
    
    // Use the singleton
    singleton.doSomething();
    
    // Getting the instance again returns the same object
    Singleton& sameInstance = Singleton::getInstance();
    
    // Show that they are the same instance
    std::cout << "Address of first instance: " << &singleton << std::endl;
    std::cout << "Address of second instance: " << &sameInstance << std::endl;
}

int main() {
    std::cout << "=== Singleton Pattern Interview Guide ===" << std::endl;
    
    // Basic demonstration
    std::cout << "\n1. Basic usage:" << std::endl;
    demonstrateSingleton();
    
    // Thread safety demonstration
    std::cout << "\n2. Thread safety demonstration:" << std::endl;
    
    // Create several threads that access the singleton
    std::thread t1([]() {
        std::cout << "Thread 1: " << &Singleton::getInstance() << std::endl;
        Singleton::getInstance().doSomething();
    });
    
    std::thread t2([]() {
        std::cout << " Thread 2: " << &Singleton::getInstance() << std::endl;
        Singleton::getInstance().doSomething();
    });
    
    // Wait for threads to complete
    t1.join();
    t2.join();
    
    // Demonstrate violation detection
    std::cout << "\n3. Multiple instance creation attempt:" << std::endl;
    Singleton::demonstrateViolation();
    
    std::cout << "\n=== End of Demonstration ===" << std::endl;
    return 0;
}

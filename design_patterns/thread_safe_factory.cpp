#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
#include <stdexcept>
#include <chrono>

/**
 * @brief Thread-safe Factory Design Pattern Implementation
 * 
 * This implementation uses:
 * 1. Smart pointers for automatic memory management
 * 2. std::mutex for thread synchronization
 * 3. Registration-based product creation
 * 4. Thread-safe singleton factory instance
 */

// Base Product interface
class Product {
public:
    virtual ~Product() = default;
    virtual std::string operation() const = 0;
    virtual void configure(const std::string& config) = 0;
};

// Concrete Product A
class ConcreteProductA : public Product {
private:
    std::string config_;
    
public:
    ConcreteProductA() {
        std::cout << "ConcreteProductA created at " << this << std::endl;
        // Simulate some initialization work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    ~ConcreteProductA() override {
        std::cout << "ConcreteProductA destroyed at " << this << std::endl;
    }
    
    std::string operation() const override {
        return "Result of ConcreteProductA with config: " + config_;
    }
    
    void configure(const std::string& config) override {
        std::cout << "Configuring ConcreteProductA with: " << config << std::endl;
        config_ = config;
    }
};

// Concrete Product B
class ConcreteProductB : public Product {
private:
    std::string config_;
    
public:
    ConcreteProductB() {
        std::cout << "ConcreteProductB created at " << this << std::endl;
        // Simulate some initialization work
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    
    ~ConcreteProductB() override {
        std::cout << "ConcreteProductB destroyed at " << this << std::endl;
    }
    
    std::string operation() const override {
        return "Result of ConcreteProductB with config: " + config_;
    }
    
    void configure(const std::string& config) override {
        std::cout << "Configuring ConcreteProductB with: " << config << std::endl;
        config_ = config;
    }
};

// Concrete Product C - more complex product
class ConcreteProductC : public Product {
private:
    std::string config_;
    std::unique_ptr<int[]> data_; // Example of internal resource
    size_t dataSize_;
    
public:
    ConcreteProductC() : dataSize_(1000) {
        std::cout << "ConcreteProductC created at " << this << std::endl;
        // Allocate some resources
        data_ = std::make_unique<int[]>(dataSize_);
        // Initialize data (simulate complex initialization)
        for (size_t i = 0; i < dataSize_; ++i) {
            data_[i] = static_cast<int>(i);
        }
        // Simulate heavy initialization
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    ~ConcreteProductC() override {
        std::cout << "ConcreteProductC destroyed at " << this << std::endl;
        // data_ will be automatically cleaned up by unique_ptr
    }
    
    std::string operation() const override {
        return "Result of ConcreteProductC with config: " + config_ + 
               ", data sum: " + std::to_string(calculateSum());
    }
    
    void configure(const std::string& config) override {
        std::cout << "Configuring ConcreteProductC with: " << config << std::endl;
        config_ = config;
    }
    
private:
    // Example of internal operation
    int calculateSum() const {
        int sum = 0;
        for (size_t i = 0; i < dataSize_; ++i) {
            sum += data_[i];
        }
        return sum;
    }
};

/**
 * Thread-safe Factory class using smart pointers
 * 
 * This factory:
 * - Uses std::unique_ptr for memory management
 * - Provides thread-safe product creation
 * - Allows runtime registration of new product types
 * - Implements the Singleton pattern for the factory itself
 */
class ThreadSafeFactory {
private:
    // Private constructor for singleton
    ThreadSafeFactory() {
        std::cout << "Factory instance created" << std::endl;
    }
    
    // Delete copy and move constructors/operators
    ThreadSafeFactory(const ThreadSafeFactory&) = delete;
    ThreadSafeFactory& operator=(const ThreadSafeFactory&) = delete;
    ThreadSafeFactory(ThreadSafeFactory&&) = delete;
    ThreadSafeFactory& operator=(ThreadSafeFactory&&) = delete;
    
    // Thread synchronization
    static std::mutex mutex_;
    
    // Registry of creator functions
    std::map<std::string, std::function<std::unique_ptr<Product>()>> creators_;

public:
    // Get the singleton instance
    static ThreadSafeFactory& getInstance() {
        // Thread-safe in C++11 and later
        static ThreadSafeFactory instance;
        return instance;
    }
    
    // Register a new product type with its creator function
    template<typename T>
    void registerProduct(const std::string& type) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        creators_[type] = []() -> std::unique_ptr<Product> {
            return std::make_unique<T>();
        };
        
        std::cout << "Registered product type: " << type << std::endl;
    }
    
    // Create a product by type
    std::unique_ptr<Product> createProduct(const std::string& type) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = creators_.find(type);
        if (it != creators_.end()) {
            return it->second();
        }
        
        throw std::runtime_error("Unknown product type: " + type);
    }
    
    // Create and configure a product in one step
    std::unique_ptr<Product> createConfiguredProduct(
        const std::string& type, 
        const std::string& config
    ) {
        auto product = createProduct(type);
        product->configure(config);
        return product;
    }
    
    // Check if a product type is registered
    bool isProductRegistered(const std::string& type) {
        std::lock_guard<std::mutex> lock(mutex_);
        return creators_.find(type) != creators_.end();
    }
    
    // Get list of registered product types
    std::vector<std::string> getRegisteredProductTypes() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        std::vector<std::string> types;
        for (const auto& pair : creators_) {
            types.push_back(pair.first);
        }
        
        return types;
    }
};

// Initialize static mutex
std::mutex ThreadSafeFactory::mutex_;

// Thread function to demonstrate concurrent factory usage
void useFactory(int threadId, const std::string& productType) {
    try {
        std::cout << "Thread " << threadId << " requesting " << productType << "..." << std::endl;
        
        // Get the factory and create a product
        auto& factory = ThreadSafeFactory::getInstance();
        
        // Add a small delay to increase chance of thread interleaving
        std::this_thread::sleep_for(std::chrono::milliseconds(threadId * 10));
        
        auto product = factory.createConfiguredProduct(
            productType, 
            "Config from thread " + std::to_string(threadId)
        );
        
        // Use the product
        std::cout << "Thread " << threadId << " using product: " << product->operation() << std::endl;
        
        // Product will be automatically deleted when unique_ptr goes out of scope
        std::cout << "Thread " << threadId << " finished using " << productType << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Thread " << threadId << " error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Thread-Safe Factory Pattern Demonstration ===" << std::endl;
    
    // Register product types
    auto& factory = ThreadSafeFactory::getInstance();
    factory.registerProduct<ConcreteProductA>("ProductA");
    factory.registerProduct<ConcreteProductB>("ProductB");
    factory.registerProduct<ConcreteProductC>("ProductC");
    
    // Display registered products
    std::cout << "\nRegistered product types:" << std::endl;
    for (const auto& type : factory.getRegisteredProductTypes()) {
        std::cout << "- " << type << std::endl;
    }
    
    // PART 1: Test with multiple threads creating different products
    {
        std::cout << "\n1. Testing concurrent product creation:" << std::endl;
        
        const int numThreads = 6;
        std::vector<std::thread> threads;
        
        // Create threads that request different product types
        threads.emplace_back(useFactory, 1, "ProductA");
        threads.emplace_back(useFactory, 2, "ProductB");
        threads.emplace_back(useFactory, 3, "ProductC");
        threads.emplace_back(useFactory, 4, "ProductA");
        threads.emplace_back(useFactory, 5, "ProductB");
        threads.emplace_back(useFactory, 6, "ProductC");
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
    }
    
    // PART 2: Test with error handling
    {
        std::cout << "\n2. Testing error handling:" << std::endl;
        
        try {
            auto product = factory.createProduct("NonExistentProduct");
        }
        catch (const std::exception& e) {
            std::cout << "Expected error caught: " << e.what() << std::endl;
        }
    }
    
    // PART 3: Demonstrate runtime registration of new product type
    {
        std::cout << "\n3. Testing runtime product registration:" << std::endl;
        
        // Define a new product type at runtime
        class RuntimeProduct : public Product {
        private:
            std::string config_;
            
        public:
            RuntimeProduct() {
                std::cout << "RuntimeProduct created at " << this << std::endl;
            }
            
            ~RuntimeProduct() override {
                std::cout << "RuntimeProduct destroyed at " << this << std::endl;
            }
            
            std::string operation() const override {
                return "Result of RuntimeProduct with config: " + config_;
            }
            
            void configure(const std::string& config) override {
                std::cout << "Configuring RuntimeProduct with: " << config << std::endl;
                config_ = config;
            }
        };
        
        // Register and use the new product type
        factory.registerProduct<RuntimeProduct>("RuntimeProduct");
        auto product = factory.createConfiguredProduct("RuntimeProduct", "Runtime config");
        std::cout << "Using runtime product: " << product->operation() << std::endl;
    }
    
    std::cout << "\n=== Factory Pattern Demonstration Complete ===" << std::endl;
    return 0;
}

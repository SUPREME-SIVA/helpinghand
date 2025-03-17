#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <functional>
#include <vector>

/**
 * Factory Design Pattern: Interview Guide
 * =======================================
 * 
 * Key Points for Interviews:
 * 
 * 1. Definition:
 *    - The Factory Pattern is a creational design pattern that provides an interface for creating objects
 *      without specifying their concrete classes
 *    - It defines a separate method/class for creating objects, which subclasses can override to change
 *      the type of objects that will be created
 * 
 * 2. Types of Factory Patterns:
 *    - Simple Factory: Not a formal pattern, but a simple way to separate object creation
 *    - Factory Method: Defines an interface for creating objects, but lets subclasses decide which classes to instantiate
 *    - Abstract Factory: Provides an interface for creating families of related or dependent objects
 * 
 * 3. Thread Safety Requirements:
 *    - Factory methods may need to be thread-safe if they maintain state or a registry of created objects
 *    - Lazy initialization of product registries requires proper synchronization
 *    - Thread-safe factories often use mutexes or atomic operations
 * 
 * 4. Modern C++ Implementation:
 *    - Use smart pointers (std::unique_ptr, std::shared_ptr) for automatic memory management
 *    - Use std::function and lambdas for flexible factory registration
 *    - Use templates for type-safe factory implementations
 * 
 * 5. Advantages:
 *    - Decouples client code from concrete product classes
 *    - Makes adding new product types easier without changing existing code
 *    - Centralizes object creation logic and enforces consistency
 *    - Enables runtime decisions about which objects to create
 * 
 * 6. Challenges and Solutions:
 *    - PROBLEM: Thread safety during object creation and registration
 *      SOLUTION: Use mutexes or std::call_once for synchronization
 *    
 *    - PROBLEM: Memory management of created objects
 *      SOLUTION: Return smart pointers instead of raw pointers
 *    
 *    - PROBLEM: Type safety with heterogeneous factories
 *      SOLUTION: Use templates and type traits
 *    
 *    - PROBLEM: Extensibility without modifying existing code
 *      SOLUTION: Use registration-based factories with std::function
 * 
 * 7. When to Use:
 *    - When a class can't anticipate the type of objects it needs to create
 *    - When you want to provide a library of products and only reveal their interfaces
 *    - When you need to create objects based on external factors (config, user input)
 *    - When you want to conserve system resources by reusing existing objects
 */

// Forward declarations
class Product;
class ConcreteProductA;
class ConcreteProductB;

// Base Product interface
class Product {
public:
    virtual ~Product() = default;
    virtual std::string operation() const = 0;
    virtual void configure(const std::string& config) = 0;
};

// Concrete Product implementations
class ConcreteProductA : public Product {
public:
    ConcreteProductA() {
        std::cout << "ConcreteProductA created at " << this << std::endl;
    }
    
    ~ConcreteProductA() override {
        std::cout << "ConcreteProductA destroyed at " << this << std::endl;
    }
    
    std::string operation() const override {
        return "Result of ConcreteProductA";
    }
    
    void configure(const std::string& config) override {
        std::cout << "Configuring ConcreteProductA with: " << config << std::endl;
        config_ = config;
    }

private:
    std::string config_;
};

class ConcreteProductB : public Product {
public:
    ConcreteProductB() {
        std::cout << "ConcreteProductB created at " << this << std::endl;
    }
    
    ~ConcreteProductB() override {
        std::cout << "ConcreteProductB destroyed at " << this << std::endl;
    }
    
    std::string operation() const override {
        return "Result of ConcreteProductB";
    }
    
    void configure(const std::string& config) override {
        std::cout << "Configuring ConcreteProductB with: " << config << std::endl;
        config_ = config;
    }

private:
    std::string config_;
};

/**
 * EXAMPLE 1: Thread-Safe Factory Method with Smart Pointers
 * 
 * This implementation demonstrates:
 * - Using std::unique_ptr for memory management
 * - Thread-safe product creation
 * - Factory method pattern with a registry of creators
 * - Runtime registration of new product types
 */
class SmartPointerFactory {
public:
    // Get the singleton instance
    static SmartPointerFactory& getInstance() {
        static SmartPointerFactory instance;  // Thread-safe in C++11 and later
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

private:
    // Private constructor for singleton
    SmartPointerFactory() = default;
    
    // Delete copy/move operations
    SmartPointerFactory(const SmartPointerFactory&) = delete;
    SmartPointerFactory& operator=(const SmartPointerFactory&) = delete;
    SmartPointerFactory(SmartPointerFactory&&) = delete;
    SmartPointerFactory& operator=(SmartPointerFactory&&) = delete;
    
    // Thread synchronization
    std::mutex mutex_;
    
    // Registry of creator functions
    std::map<std::string, std::function<std::unique_ptr<Product>()>> creators_;
};

/**
 * EXAMPLE 2: Thread-Safe Factory with Raw Pointers and Object Pool
 * 
 * This implementation demonstrates:
 * - Using raw pointers with careful memory management
 * - Object pooling for resource reuse
 * - Thread-safe access to the object pool
 * - Factory method pattern with object caching
 */
class RawPointerFactory {
public:
    // Get the singleton instance
    static RawPointerFactory& getInstance() {
        static RawPointerFactory instance;  // Thread-safe in C++11 and later
        return instance;
    }
    
    // Create or reuse a product
    Product* createProduct(const std::string& type) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if we have an available object in the pool
        auto poolIt = objectPool_.find(type);
        if (poolIt != objectPool_.end() && !poolIt->second.empty()) {
            // Reuse an existing object from the pool
            Product* product = poolIt->second.back();
            poolIt->second.pop_back();
            std::cout << "Reusing " << type << " from object pool" << std::endl;
            return product;
        }
        
        // Create a new object if none available in the pool
        if (type == "ProductA") {
            return new ConcreteProductA();
        } else if (type == "ProductB") {
            return new ConcreteProductB();
        }
        
        throw std::runtime_error("Unknown product type: " + type);
    }
    
    // Return a product to the pool instead of deleting it
    void releaseProduct(const std::string& type, Product* product) {
        if (!product) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Add the product back to the pool for future reuse
        objectPool_[type].push_back(product);
        std::cout << "Returned " << type << " to object pool" << std::endl;
    }
    
    // Clean up all pooled objects (call at program end)
    void cleanup() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        for (auto& pair : objectPool_) {
            for (auto* product : pair.second) {
                std::cout << "Deleting pooled " << pair.first << std::endl;
                delete product;
            }
            pair.second.clear();
        }
    }
    
    // Destructor ensures all pooled objects are deleted
    ~RawPointerFactory() {
        cleanup();
    }

private:
    // Private constructor for singleton
    RawPointerFactory() = default;
    
    // Delete copy/move operations
    RawPointerFactory(const RawPointerFactory&) = delete;
    RawPointerFactory& operator=(const RawPointerFactory&) = delete;
    RawPointerFactory(RawPointerFactory&&) = delete;
    RawPointerFactory& operator=(RawPointerFactory&&) = delete;
    
    // Thread synchronization
    std::mutex mutex_;
    
    // Object pool for reusing products
    std::map<std::string, std::vector<Product*>> objectPool_;
};

// Thread function to demonstrate thread safety with smart pointer factory
void useSmartPointerFactory(int threadId, const std::string& productType) {
    try {
        std::cout << "Thread " << threadId << " creating " << productType << std::endl;
        
        // Get the factory and create a product
        auto& factory = SmartPointerFactory::getInstance();
        auto product = factory.createConfiguredProduct(
            productType, 
            "Config from thread " + std::to_string(threadId)
        );
        
        // Use the product
        std::cout << "Thread " << threadId << " using product: " << product->operation() << std::endl;
        
        // Product will be automatically deleted when unique_ptr goes out of scope
    }
    catch (const std::exception& e) {
        std::cerr << "Thread " << threadId << " error: " << e.what() << std::endl;
    }
}

// Thread function to demonstrate thread safety with raw pointer factory
void useRawPointerFactory(int threadId, const std::string& productType) {
    try {
        std::cout << "Thread " << threadId << " creating " << productType << std::endl;
        
        // Get the factory and create a product
        auto& factory = RawPointerFactory::getInstance();
        Product* product = factory.createProduct(productType);
        
        // Configure and use the product
        product->configure("Config from thread " + std::to_string(threadId));
        std::cout << "Thread " << threadId << " using product: " << product->operation() << std::endl;
        
        // Return the product to the pool instead of deleting it
        factory.releaseProduct(productType, product);
    }
    catch (const std::exception& e) {
        std::cerr << "Thread " << threadId << " error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Factory Pattern Demonstration ===" << std::endl;
    
    // PART 1: Smart Pointer Factory demonstration
    {
        std::cout << "\n1. Smart Pointer Factory (with unique_ptr):" << std::endl;
        
        // Register product types
        auto& smartFactory = SmartPointerFactory::getInstance();
        smartFactory.registerProduct<ConcreteProductA>("ProductA");
        smartFactory.registerProduct<ConcreteProductB>("ProductB");
        
        // Create threads to demonstrate thread safety
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(useSmartPointerFactory, i + 1, "ProductA");
            threads.emplace_back(useSmartPointerFactory, i + 4, "ProductB");
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
    }
    
    // PART 2: Raw Pointer Factory with object pooling
    {
        std::cout << "\n2. Raw Pointer Factory (with object pooling):" << std::endl;
        
        // Create threads to demonstrate thread safety
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(useRawPointerFactory, i + 1, "ProductA");
            threads.emplace_back(useRawPointerFactory, i + 4, "ProductB");
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
        
        // Clean up the object pool
        RawPointerFactory::getInstance().cleanup();
    }
    
    std::cout << "\n=== Factory Pattern Demonstration Complete ===" << std::endl;
    return 0;
}

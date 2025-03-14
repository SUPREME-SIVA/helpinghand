/**
 * C++17 Advanced Features
 * 
 * This file demonstrates the advanced features introduced in C++17.
 * Each feature is explained with examples and comments to help with interview preparation.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <optional>
#include <variant>
#include <any>
#include <memory>
#include <string_view>
#include <filesystem>
#include <algorithm>
#include <execution>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <numeric>  // For std::iota
#include <thread>   // For std::thread

// ======================================================================
// std::optional
// ======================================================================
std::optional<std::string> find_user(int id) {
    // Simulating a database lookup
    if (id == 1) return "Alice";
    if (id == 2) return "Bob";
    if (id == 3) return "Charlie";
    return std::nullopt; // User not found
}

void demo_optional() {
    std::cout << "\n=== std::optional ===\n";
    
    // 1. Basic usage
    auto user1 = find_user(1);
    auto user4 = find_user(4);
    
    std::cout << "User 1: " << (user1.has_value() ? user1.value() : "Not found") << "\n";
    std::cout << "User 4: " << (user4.has_value() ? user4.value() : "Not found") << "\n";
    
    // 2. Using value_or for default values
    std::cout << "User 1: " << user1.value_or("Unknown") << "\n";
    std::cout << "User 4: " << user4.value_or("Unknown") << "\n";
    
    // 3. Creating optionals
    std::optional<int> opt1;              // Empty optional
    std::optional<int> opt2 = 42;         // Optional with value
    std::optional<int> opt3 = std::nullopt; // Explicitly empty
    std::optional<std::string> opt4{"Hello"}; // Optional with string
    
    // 4. Checking and accessing values
    if (opt2) {
        std::cout << "opt2 has value: " << *opt2 << "\n";
    }
    
    // 5. Modifying values
    opt1 = 10;
    opt1.reset(); // Make it empty again
    
    // Interview tip: std::optional represents a value that may or may not be present,
    // providing a safer alternative to nullable pointers or sentinel values
}

// ======================================================================
// std::variant
// ======================================================================
// Helper visitor using C++17 template deduction guides
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void demo_variant() {
    std::cout << "\n=== std::variant ===\n";
    
    // 1. Creating variants
    std::variant<int, double, std::string> v1 = 42;
    std::variant<int, double, std::string> v2 = 3.14;
    std::variant<int, double, std::string> v3 = "hello";
    
    // 2. Checking which type is currently held
    std::cout << "v1 holds int? " << std::boolalpha << (std::holds_alternative<int>(v1)) << "\n";
    std::cout << "v2 holds double? " << (std::holds_alternative<double>(v2)) << "\n";
    std::cout << "v3 holds string? " << (std::holds_alternative<std::string>(v3)) << "\n";
    
    // 3. Getting values (with type checking)
    try {
        int i = std::get<int>(v1);
        double d = std::get<double>(v2);
        std::string s = std::get<std::string>(v3);
        
        std::cout << "Values: " << i << ", " << d << ", " << s << "\n";
        
        // This would throw std::bad_variant_access
        // int bad = std::get<int>(v3);
    } catch (const std::bad_variant_access& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    // 4. Getting values safely
    if (auto pval = std::get_if<int>(&v1)) {
        std::cout << "v1 contains int: " << *pval << "\n";
    }
    
    // 5. Changing the value/type
    v1 = "now a string";
    std::cout << "v1 now holds: " << std::get<std::string>(v1) << "\n";
    
    // 6. Using visitor pattern with variants
    std::variant<int, double, std::string> var = 42;
    
    auto visitor = overloaded {
        [](int i) { std::cout << "int: " << i << "\n"; },
        [](double d) { std::cout << "double: " << d << "\n"; },
        [](const std::string& s) { std::cout << "string: " << s << "\n"; }
    };
    
    std::visit(visitor, var);
    var = 3.14;
    std::visit(visitor, var);
    var = "hello";
    std::visit(visitor, var);
    
    // Interview tip: std::variant is a type-safe union that can hold values of different
    // types, but only one at a time. It's useful for representing a value that could be
    // one of several types without using inheritance or dynamic allocation
}

// ======================================================================
// std::any
// ======================================================================
void demo_any() {
    std::cout << "\n=== std::any ===\n";
    
    // 1. Creating any objects
    std::any a1;                    // Empty
    std::any a2 = 42;               // Holds int
    std::any a3 = 3.14;             // Holds double
    std::any a4 = std::string("hello"); // Holds string
    
    // 2. Checking if any has a value
    std::cout << "a1 has value? " << a1.has_value() << "\n";
    std::cout << "a2 has value? " << a2.has_value() << "\n";
    
    // 3. Getting the type info
    std::cout << "a2 type: " << a2.type().name() << "\n";
    std::cout << "a3 type: " << a3.type().name() << "\n";
    std::cout << "a4 type: " << a4.type().name() << "\n";
    
    // 4. Accessing values
    try {
        int i = std::any_cast<int>(a2);
        double d = std::any_cast<double>(a3);
        std::string s = std::any_cast<std::string>(a4);
        
        std::cout << "Values: " << i << ", " << d << ", " << s << "\n";
        
        // This would throw std::bad_any_cast
        // int bad = std::any_cast<int>(a4);
    } catch (const std::bad_any_cast& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    // 5. Accessing values safely
    if (int* i = std::any_cast<int>(&a2)) {
        std::cout << "a2 contains int: " << *i << "\n";
    }
    
    // 6. Changing the value/type
    a1 = std::vector<int>{1, 2, 3, 4, 5};
    
    // 7. Practical example: heterogeneous container
    std::vector<std::any> mixed_data;
    mixed_data.push_back(42);
    mixed_data.push_back(3.14);
    mixed_data.push_back(std::string("hello"));
    mixed_data.push_back(std::vector<int>{1, 2, 3});
    
    for (const auto& data : mixed_data) {
        if (data.type() == typeid(int)) {
            std::cout << "int: " << std::any_cast<int>(data) << "\n";
        } else if (data.type() == typeid(double)) {
            std::cout << "double: " << std::any_cast<double>(data) << "\n";
        } else if (data.type() == typeid(std::string)) {
            std::cout << "string: " << std::any_cast<std::string>(data) << "\n";
        } else {
            std::cout << "other type\n";
        }
    }
    
    // Interview tip: std::any provides type-safe container for single values of any type,
    // with runtime type checking. It's more flexible than std::variant but has runtime overhead
}

// ======================================================================
// Filesystem Library
// ======================================================================
void demo_filesystem() {
    std::cout << "\n=== Filesystem Library ===\n";
    namespace fs = std::filesystem;
    
    // 1. Working with paths
    fs::path p1 = "/usr/local/bin/example";
    fs::path p2 = "C:\\Program Files\\Example";
    fs::path p3 = "~/documents/example.txt";
    
    std::cout << "p1 filename: " << p1.filename() << "\n";
    std::cout << "p2 parent path: " << p2.parent_path() << "\n";
    std::cout << "p3 extension: " << p3.extension() << "\n";
    
    // 2. Path operations
    fs::path dir = "/usr/local";
    fs::path file = "example.txt";
    fs::path combined = dir / file;  // Path concatenation
    
    std::cout << "Combined path: " << combined << "\n";
    
    // 3. File status
    try {
        fs::path current = fs::current_path();
        std::cout << "Current path: " << current << "\n";
        
        if (fs::exists(current)) {
            std::cout << "Path exists\n";
            
            if (fs::is_directory(current)) {
                std::cout << "Path is a directory\n";
            }
            
            std::cout << "File size: " << fs::file_size(current) << " bytes\n";
            auto last_write_time = fs::last_write_time(current);
            std::cout << "Last write time: (timestamp available but not displayed)\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem error: " << e.what() << "\n";
    }
    
    // 4. Directory iteration
    try {
        fs::path dir_path = fs::current_path();
        std::cout << "Directory contents of " << dir_path << ":\n";
        
        // Only show the first few entries to avoid excessive output
        int count = 0;
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            std::cout << "  " << entry.path().filename() << "\n";
            if (++count >= 5) {
                std::cout << "  ... (more entries)\n";
                break;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem error: " << e.what() << "\n";
    }
    
    // Interview tip: The filesystem library provides portable ways to work with
    // files and directories, replacing platform-specific APIs with a unified interface
}

// ======================================================================
// Parallel Algorithms
// ======================================================================
void demo_parallel_algorithms() {
    std::cout << "\n=== Parallel Algorithms ===\n";
    
    // Create a large vector for testing
    std::vector<int> v(10'000'000);
    std::iota(v.begin(), v.end(), 0);  // Fill with 0, 1, 2, ...
    
    // 3. Comparing sequential vs parallel execution
    {
        auto v_copy = v;
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(v_copy.begin(), v_copy.end(), std::greater<>());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Sequential sort time: " << elapsed.count() << " ms\n";
    }
    
    {
        auto v_copy = v;
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(v_copy.begin(), v_copy.end(), std::greater<>());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Parallel sort time: " << elapsed.count() << " ms\n";
    }
    
    {
        auto v_copy = v;
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(v_copy.begin(), v_copy.end(), std::greater<>());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Parallel unseq sort time: " << elapsed.count() << " ms\n";
    }
    
    // 4. Other parallel algorithms
    std::vector<int> v2(1'000'000);
    std::iota(v2.begin(), v2.end(), 0);
    
    // Parallel transform
    std::vector<int> results(v2.size());
    std::transform(
        v2.begin(), v2.end(),
        results.begin(),
        [](int x) { return x * x; }
    );
    
    // Parallel for_each
    std::for_each(
        results.begin(), results.end(),
        [](int& x) { x = x / 2; }
    );
    
    // Parallel reduce
    int sum = std::reduce(
        results.begin(), results.end(),
        0
    );
    
    std::cout << "Parallel operations result sum: " << sum << "\n";
    
    // Interview tip: C++17 parallel algorithms allow existing STL algorithms
    // to be executed with different execution policies (sequential, parallel,
    // parallel unsequenced) for potential performance improvements
}

// ======================================================================
// std::shared_mutex
// ======================================================================
class ThreadSafeCounter {
private:
    mutable std::shared_mutex mutex_;
    int value_ = 0;
    
public:
    // Multiple threads can read the counter's value at the same time
    int get() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return value_;
    }
    
    // Only one thread can increment the counter at a time
    void increment() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_++;
    }
    
    // Only one thread can reset the counter at a time
    void reset() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ = 0;
    }
};

void demo_shared_mutex() {
    std::cout << "\n=== std::shared_mutex ===\n";
    
    ThreadSafeCounter counter;
    
    auto reader = [&counter](int id) {
        for (int i = 0; i < 3; ++i) {
            // Reading is done under shared lock - multiple threads can read simultaneously
            int value = counter.get();
            std::cout << "Reader " << id << " read value: " << value << "\n";
            // Use a small sleep to simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    
    auto writer = [&counter](int id) {
        for (int i = 0; i < 3; ++i) {
            // Writing is done under exclusive lock - only one thread can write at a time
            counter.increment();
            std::cout << "Writer " << id << " incremented counter\n";
            // Use a small sleep to simulate some work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    
    // Create reader and writer threads
    std::thread r1(reader, 1);
    std::thread r2(reader, 2);
    std::thread w1(writer, 1);
    
    // Wait for all threads to finish
    r1.join();
    r2.join();
    w1.join();
    
    std::cout << "Final counter value: " << counter.get() << "\n";
    
    // Interview tip: std::shared_mutex allows multiple readers to access a resource
    // simultaneously, but ensures that writers have exclusive access. This improves
    // performance in read-heavy scenarios compared to using a standard mutex
}

// ======================================================================
// Main function to demonstrate all features
// ======================================================================
int main() {
    std::cout << "===== C++17 Advanced Features =====\n";
    
    demo_optional();
    demo_variant();
    demo_any();
    demo_filesystem();
    demo_parallel_algorithms();
    demo_shared_mutex();
    
    std::cout << "\nAll advanced C++17 features demonstrated successfully!\n";
    return 0;
}

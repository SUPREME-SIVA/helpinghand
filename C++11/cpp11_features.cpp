#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <thread>
#include <mutex>
#include <chrono>
#include <regex>
#include <type_traits>
#include <utility>
#include <array>
#include <future>
#include <atomic>
#include <condition_variable>

// Template function declarations
template<typename T>
void print(const T& arg);

void println();

template<typename T, typename... Args>
void println(const T& first, const Args&... rest);

template<typename... Args>
int sum_fold(const Args&... args);

template<typename... Args>
std::tuple<Args...> make_tuple_demo(Args... args);

template<typename T>
T&& perfect_forward(T&& arg);

void safe_printf(const char* format);

template<typename T, typename... Args>
void safe_printf(const char* format, const T& first, const Args&... rest);

// Constexpr function declarations
constexpr int square(int x);
constexpr int factorial(int n);

// Template function implementations
template<typename T>
void print(const T& arg) {
    std::cout << arg << " ";
}

void println() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void println(const T& first, const Args&... rest) {
    print(first);
    println(rest...);
}

template<typename... Args>
int sum_fold(const Args&... args) {
    int result = 0;
    // Use initializer list and variadic expansion
    int dummy[] = { 0, (result += args, 0)... };
    (void)dummy; // Suppress unused variable warning
    return result;
}

template<typename... Args>
std::tuple<Args...> make_tuple_demo(Args... args) {
    return std::make_tuple(args...);
}

template<typename T>
T&& perfect_forward(T&& arg) {
    std::cout << "Perfect forwarding: ";
    // std::forward preserves value category (lvalue/rvalue)
    return std::forward<T>(arg);
}

void safe_printf(const char* format) {
    std::cout << format;
}

template<typename T, typename... Args>
void safe_printf(const char* format, const T& first, const Args&... rest) {
    while (*format) {
        if (*format == '%' && *(format + 1) != '%') {
            std::cout << first;
            safe_printf(format + 1, rest...);
            return;
        }
        std::cout << *format++;
    }
}

// Constexpr function implementations
constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// Forward declarations for all feature demonstration functions
void demo_auto_and_decltype();
void demo_nullptr();
void demo_range_based_for();
void demo_lambda_expressions();
void demo_move_semantics();
void demo_smart_pointers();
void demo_variadic_templates();
void demo_initializer_lists();
void demo_uniform_initialization();
void demo_type_inference();
void demo_constexpr();
void demo_enum_class();
void demo_static_assert();
void demo_threading();
void demo_tuple_and_tie();
void demo_regex();
void demo_chrono();
void demo_random();
void demo_thread_local();
void demo_delegating_constructors();
void demo_explicit_overrides();

int main() {
    std::cout << "=== C++11 Features Demonstration ===\n";
    std::cout << "This program demonstrates key C++11 features with examples and interview questions.\n\n";
    
    int choice = 0;
    bool continue_program = true;
    
    while (continue_program) {
        std::cout << "\nChoose a C++11 feature to explore:\n";
        std::cout << "1. Auto and Decltype\n";
        std::cout << "2. Nullptr\n";
        std::cout << "3. Range-based For Loops\n";
        std::cout << "4. Lambda Expressions\n";
        std::cout << "5. Move Semantics\n";
        std::cout << "6. Smart Pointers\n";
        std::cout << "7. Variadic Templates\n";
        std::cout << "8. Initializer Lists\n";
        std::cout << "9. Uniform Initialization\n";
        std::cout << "10. Type Inference\n";
        std::cout << "11. Constexpr\n";
        std::cout << "12. Enum Class\n";
        std::cout << "13. Static Assert\n";
        std::cout << "14. Threading\n";
        std::cout << "15. Regular Expressions\n";
        std::cout << "16. Chrono Library\n";
        std::cout << "17. Random Number Generation\n";
        std::cout << "18. Tuple\n";
        std::cout << "19. Thread Local Storage\n";
        std::cout << "20. Delegating Constructors\n";
        std::cout << "21. Explicit Overrides and Final\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice (0-21): ";
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                demo_auto_and_decltype();
                break;
            case 2:
                demo_nullptr();
                break;
            case 3:
                demo_range_based_for();
                break;
            case 4:
                demo_lambda_expressions();
                break;
            case 5:
                demo_move_semantics();
                break;
            case 6:
                demo_smart_pointers();
                break;
            case 7:
                demo_variadic_templates();
                break;
            case 8:
                demo_initializer_lists();
                break;
            case 9:
                demo_uniform_initialization();
                break;
            case 10:
                demo_type_inference();
                break;
            case 11:
                demo_constexpr();
                break;
            case 12:
                demo_enum_class();
                break;
            case 13:
                demo_static_assert();
                break;
            case 14:
                demo_threading();
                break;
            case 15:
                demo_regex();
                break;
            case 16:
                demo_chrono();
                break;
            case 17:
                demo_random();
                break;
            case 18:
                demo_tuple_and_tie();
                break;
            case 19:
                demo_thread_local();
                break;
            case 20:
                demo_delegating_constructors();
                break;
            case 21:
                demo_explicit_overrides();
                break;
            case 0:
                continue_program = false;
                std::cout << "Exiting program. Thank you for exploring C++11 features!\n";
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 0 and 21.\n";
        }
        
        if (continue_program) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    
    return 0;
}

/**
 * @brief Demonstrates auto and decltype for type inference
 * 
 * Interview Questions:
 * 1. What is the difference between auto and decltype?
 * 2. When would you use auto vs decltype?
 * 3. What are the limitations of auto?
 */
void demo_auto_and_decltype() {
    std::cout << "\n=== Auto and Decltype ===\n";
    
    // Auto for variable declarations
    auto i = 42;                  // int
    auto d = 42.5;                // double
    auto s = "C++11";             // const char*
    auto v = std::vector<int>{1, 2, 3}; // std::vector<int>
    
    std::cout << "Auto variables: " << i << ", " << d << ", " << s << "\n";
    
    // Auto with references and const
    const auto& ri = i;           // const int&
    
    // Auto with iterators (one of the most common use cases)
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    // Decltype for expressions
    int x = 0;
    decltype(x) y = 5;            // int
    decltype(x + 5.5) z = 10.5;   // double
    
    // Decltype with functions
    auto add = [](int a, double b) { return a + b; };
    decltype(add(5, 3.5)) result = add(10, 20.5);
    std::cout << "Decltype result: " << result << "\n";
    
    // Common use case: trailing return type
    auto multiply = [](int a, double b) -> decltype(a * b) {
        return a * b;
    };
    
    std::cout << "Multiply result: " << multiply(5, 3.5) << "\n";
    
    // Interview tip: auto doesn't preserve references or cv-qualifiers
    const int c = 10;
    auto c1 = c;      // int, not const int
    c1 = 20;          // OK, c1 is not const
    
    decltype(c) c2 = 30; // const int
    // c2 = 40;         // Error: c2 is const
    
    std::cout << "Interview Notes:\n";
    std::cout << "1. auto deduces type from initializer, decltype from expression\n";
    std::cout << "2. auto drops references and cv-qualifiers, decltype preserves them\n";
    std::cout << "3. Use auto for iterator types and long type names\n";
    std::cout << "4. Use decltype for generic programming and perfect forwarding\n";
}

/**
 * @brief Demonstrates nullptr for null pointer representation
 * 
 * Interview Questions:
 * 1. What is nullptr and why is it better than NULL?
 * 2. How does nullptr solve the NULL overloaded function ambiguity problem?
 */
void demo_nullptr() {
    std::cout << "\n=== Nullptr ===\n";
    
    // Old way: using NULL or 0
    int* oldPtr1 = nullptr;  // C-style null pointer
    int* oldPtr2 = 0;     // Using 0 as null pointer
    
    // New way: using nullptr
    int* newPtr = nullptr;  // Modern C++ null pointer
    
    std::cout << "All pointers are null: " 
              << (oldPtr1 == nullptr) << ", " 
              << (oldPtr2 == nullptr) << ", " 
              << (newPtr == nullptr) << "\n";
    
    // Function overloading problem with NULL
    auto overloaded_func = [](int i) { 
        std::cout << "Called with int: " << i << "\n"; 
    };
    
    auto overloaded_func_ptr = [](int* p) { 
        std::cout << "Called with pointer: " << (p ? "non-null" : "null") << "\n"; 
    };
    
    // With NULL or 0, this would call the int version
    // overloaded_func(NULL); // Calls int version, not what we want!
    overloaded_func(0);       // Calls int version
    
    // With nullptr, it correctly calls the pointer version
    overloaded_func_ptr(nullptr); // Calls pointer version as expected
    
    // nullptr is of type std::nullptr_t, not an integer type
    std::nullptr_t null_type = nullptr;
    
    // Can be implicitly converted to any pointer type
    char* charPtr = static_cast<char*>(null_type);
    double* doublePtr = nullptr;
    
    // Can be used with smart pointers too
    std::shared_ptr<int> smartPtr = nullptr;
    
    std::cout << "Interview Notes:\n";
    std::cout << "1. nullptr is type-safe and has its own type (std::nullptr_t)\n";
    std::cout << "2. nullptr solves overloaded function resolution ambiguity\n";
    std::cout << "3. nullptr can be implicitly converted to any pointer type\n";
    std::cout << "4. Always use nullptr instead of NULL or 0 for pointers in modern C++\n";
}

/**
 * @brief Demonstrates range-based for loops
 * 
 * Interview Questions:
 * 1. How do range-based for loops work?
 * 2. What types can be used with range-based for loops?
 * 3. How do you modify elements in a container using range-based for?
 */
void demo_range_based_for() {
    std::cout << "\n=== Range-based For Loops ===\n";
    
    // Basic usage with array
    int numbers[] = {1, 2, 3, 4, 5};
    
    std::cout << "Array elements: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // With STL containers
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    
    std::cout << "Vector elements: ";
    for (const auto& fruit : fruits) {
        std::cout << fruit << " ";
    }
    std::cout << std::endl;
    
    // Modifying elements
    std::cout << "Modifying vector elements: ";
    for (auto& fruit : fruits) {
        fruit[0] = std::toupper(fruit[0]);  // Capitalize first letter
    }
    
    // Print modified elements
    for (const auto& fruit : fruits) {
        std::cout << fruit << " ";
    }
    std::cout << std::endl;
    
    // With initializer list
    std::cout << "Initializer list: ";
    for (auto x : {10, 20, 30, 40}) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // With map
    std::unordered_map<std::string, int> ages = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };
    
    std::cout << "Map elements: ";
    for (const auto& pair : ages) {
        std::cout << pair.first << "=" << pair.second << " ";
    }
    std::cout << std::endl;
    
    // C++17 structured binding (worth mentioning)
    std::cout << "C++17 structured binding (mentioned for reference): \n";
    std::cout << "for (const auto& [name, age] : ages) { ... }\n";
    
    // How range-based for works under the hood
    std::cout << "\nEquivalent to traditional for loop:\n";
    std::cout << "for (auto it = container.begin(); it != container.end(); ++it) { ... }\n";
    
    std::cout << "Interview Notes:\n";
    std::cout << "1. Works with any type that has begin() and end() or can be used with std::begin/std::end\n";
    std::cout << "2. Use const auto& for read-only access to avoid copying\n";
    std::cout << "3. Use auto& to modify elements in the container\n";
    std::cout << "4. Range-based for loops are more concise and less error-prone than traditional loops\n";
}

/**
 * @brief Demonstrates lambda expressions and closures
 * 
 * Interview Questions:
 * 1. What are lambda expressions and when would you use them?
 * 2. Explain capture clauses in lambdas
 * 3. What's the difference between capturing by value vs by reference?
 * 4. How do you capture this in a lambda?
 */
void demo_lambda_expressions() {
    std::cout << "\n=== Lambda Expressions ===\n";
    
    // Basic lambda syntax: [capture](parameters) -> return_type { body }
    auto simple_lambda = []() { std::cout << "Simple lambda\n"; };
    simple_lambda();
    
    // Lambda with parameters
    auto add = [](int a, int b) { return a + b; };
    std::cout << "Lambda add: " << add(5, 3) << "\n";
    
    // Lambda with explicit return type
    auto divide = [](int a, int b) -> double { return static_cast<double>(a) / b; };
    std::cout << "Lambda divide: " << divide(5, 2) << "\n";
    
    // Capture by value
    int x = 10;
    auto val_capture = [x]() { 
        // x = 20; // Error: x is captured by value and is immutable
        return x;
    };
    x = 20; // This doesn't affect the lambda's captured x
    std::cout << "Capture by value: " << val_capture() << " (original x: " << x << ")\n";
    
    // Capture by reference
    int y = 10;
    auto ref_capture = [&y]() {
        y = 30; // OK: y is captured by reference and can be modified
        return y;
    };
    std::cout << "Before ref_capture: y = " << y << "\n";
    std::cout << "Capture by reference: " << ref_capture() << "\n";
    std::cout << "After ref_capture: y = " << y << "\n";
    
    // Capture all by value
    int a = 1, b = 2, c = 3;
    auto capture_all_val = [=]() { return a + b + c; };
    
    // Capture all by reference
    auto capture_all_ref = [&]() { a = 10; b = 20; c = 30; };
    
    // Mixed capture
    auto mixed_capture = [a, &b]() { 
        // a = 100; // Error: a is captured by value
        b = 200;     // OK: b is captured by reference
        return a + b;
    };
    
    // Mutable lambda
    auto mutable_lambda = [x]() mutable {
        x += 10; // OK with mutable
        return x;
    };
    std::cout << "First call to mutable lambda: " << mutable_lambda() << "\n";
    std::cout << "Second call to mutable lambda: " << mutable_lambda() << "\n";
    
    // Capturing this (in a class context)
    struct MyClass {
        int value = 100;
        
        // Fix: Return the value directly instead of a lambda
        int get_value_lambda() {
            // Simplified to return the value directly
            return value;
        }
    };
    
    MyClass obj;
    auto lambda = obj.get_value_lambda();
    std::cout << "Lambda capturing this: " << lambda << "\n";
    
    // Using lambdas with algorithms
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sum = 0;
    
    std::for_each(nums.begin(), nums.end(), [&sum](int n) {
        if (n % 2 == 0) {
            sum += n;
        }
    });
    
    std::cout << "Sum of even numbers: " << sum << "\n";
    
    // Lambda as predicate
    auto is_even = [](int n) { return n % 2 == 0; };
    auto even_count = std::count_if(nums.begin(), nums.end(), is_even);
    std::cout << "Number of even values: " << even_count << "\n";
    
    // Recursive lambda (C++14, but worth mentioning)
    std::cout << "Recursive lambda (C++14 feature):\n";
    std::function<int(int)> factorial = [&factorial](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    std::cout << "Factorial of 5: " << factorial(5) << "\n";
    
    std::cout << "Interview Notes:\n";
    std::cout << "1. Lambdas are anonymous functions that can capture their environment\n";
    std::cout << "2. Use [=] to capture all by value, [&] for all by reference\n";
    std::cout << "3. Use mutable to modify variables captured by value\n";
    std::cout << "4. Lambdas are implemented as function objects (closures)\n";
    std::cout << "5. Prefer lambdas over std::bind for simple function adapters\n";
    std::cout << "6. Be careful with reference captures in asynchronous code\n";
}

/**
 * @brief Demonstrates move semantics and rvalue references
 * 
 * Interview Questions:
 * 1. What are rvalue references and how do they enable move semantics?
 * 2. What is the difference between copy and move operations?
 * 3. When would you use std::move and what are its potential dangers?
 * 4. Explain the Rule of Five in C++11
 */
void demo_move_semantics() {
    std::cout << "\n=== Move Semantics and Rvalue References ===\n";
    
    // A simple class to demonstrate move semantics
    class Resource {
    private:
        int* data;
        size_t size;
        
    public:
        // Constructor
        Resource(size_t sz) : size(sz) {
            std::cout << "Constructor called\n";
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<int>(i);
            }
        }
        
        // Destructor
        ~Resource() {
            std::cout << "Destructor called\n";
            delete[] data;
        }
        
        // Copy constructor
        Resource(const Resource& other) : size(other.size) {
            std::cout << "Copy constructor called (expensive)\n";
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        
        // Copy assignment operator
        Resource& operator=(const Resource& other) {
            std::cout << "Copy assignment called (expensive)\n";
            if (this != &other) {
                delete[] data;
                size = other.size;
                data = new int[size];
                for (size_t i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            }
            return *this;
        }
        
        // Move constructor
        Resource(Resource&& other) noexcept : data(nullptr), size(0) {
            std::cout << "Move constructor called (cheap)\n";
            // Steal the resources from other
            data = other.data;
            size = other.size;
            
            // Leave other in a valid but empty state
            other.data = nullptr;
            other.size = 0;
        }
        
        // Move assignment operator
        Resource& operator=(Resource&& other) noexcept {
            std::cout << "Move assignment called (cheap)\n";
            if (this != &other) {
                // Free current resources
                delete[] data;
                
                // Steal the resources from other
                data = other.data;
                size = other.size;
                
                // Leave other in a valid but empty state
                other.data = nullptr;
                other.size = 0;
            }
            return *this;
        }
        
        // Utility method
        size_t getSize() const { return size; }
        
        void display() const {
            if (data) {
                std::cout << "Resource with " << size << " elements: ";
                for (size_t i = 0; i < std::min(size, size_t(5)); ++i) {
                    std::cout << data[i] << " ";
                }
                std::cout << (size > 5 ? "..." : "") << "\n";
            } else {
                std::cout << "Empty resource (moved from)\n";
            }
        }
    };
    
    std::cout << "Creating r1:\n";
    Resource r1(1000000);  // Large resource
    r1.display();
    
    std::cout << "\nCopying r1 to r2 (expensive):\n";
    Resource r2 = r1;  // Copy constructor
    r2.display();
    
    std::cout << "\nMoving r1 to r3 (cheap):\n";
    Resource r3 = std::move(r1);  // Move constructor
    r3.display();
    r1.display();  // r1 is now in a moved-from state
    
    std::cout << "\nCreating r4:\n";
    Resource r4(10);
    r4.display();
    
    std::cout << "\nMove assigning r3 to r4:\n";
    r4 = std::move(r3);  // Move assignment
    r4.display();
    r3.display();  // r3 is now in a moved-from state
    
    // Perfect forwarding with std::forward
    // template<typename T>
    // T&& perfect_forward(T&& arg) {
    //     std::cout << "Perfect forwarding: ";
    //     // std::forward preserves value category (lvalue/rvalue)
    //     return std::forward<T>(arg);
    // }
    
    std::cout << "\nPerfect forwarding with temporary:\n";
    Resource r5 = perfect_forward(Resource(5));  // Forwards as rvalue
    r5.display();
    
    std::cout << "\nPerfect forwarding with named variable:\n";
    Resource r6(6);
    Resource& r7 = perfect_forward<Resource&>(r6);  // Forwards as lvalue
    r7.display();
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Move semantics allow transferring resources instead of copying them\n";
    std::cout << "2. std::move doesn't actually move anything - it casts to an rvalue reference\n";
    std::cout << "3. Rule of Five: if you need any of destructor, copy constructor, copy assignment,\n";
    std::cout << "   move constructor, or move assignment, you probably need all five\n";
    std::cout << "4. Always mark move operations as noexcept for optimizations in containers\n";
    std::cout << "5. After a move, the source object should be in a valid but unspecified state\n";
    std::cout << "6. Perfect forwarding preserves value category with std::forward\n";
}

/**
 * @brief Demonstrates smart pointers for automatic memory management
 * 
 * Interview Questions:
 * 1. What are the different types of smart pointers and when would you use each?
 * 2. How does shared_ptr implement reference counting?
 * 3. What is the relationship between unique_ptr and move semantics?
 * 4. What problem does weak_ptr solve in relation to shared_ptr?
 */
void demo_smart_pointers() {
    std::cout << "\n=== Smart Pointers ===\n";
    
    // Class to demonstrate resource management
    class Resource {
    public:
        std::string name;
        
        Resource(const std::string& n) : name(n) {
            std::cout << "Resource " << name << " created\n";
        }
        
        ~Resource() {
            std::cout << "Resource " << name << " destroyed\n";
        }
        
        void use() {
            std::cout << "Using resource " << name << "\n";
        }
    };
    
    // unique_ptr - exclusive ownership
    std::cout << "\n--- std::unique_ptr ---\n";
    {
        std::cout << "Creating unique_ptr:\n";
        std::unique_ptr<Resource> uniq1(new Resource("Unique1"));
        uniq1->use();
        
        // Modern way with make_unique (C++14, but worth mentioning)
        std::cout << "\nCreating with make_unique (C++14):\n";
        // auto uniq2 = std::make_unique<Resource>("Unique2");  // C++14
        
        // Cannot copy unique_ptr
        // std::unique_ptr<Resource> uniq3 = uniq1;  // Compilation error
        
        // But can transfer ownership via move
        std::cout << "\nTransferring ownership with move:\n";
        std::unique_ptr<Resource> uniq4 = std::move(uniq1);
        
        // Original pointer is now null
        std::cout << "Original unique_ptr is " << (uniq1 ? "not null" : "null") << "\n";
        
        // Can reset to release ownership
        std::cout << "\nResetting unique_ptr:\n";
        uniq4.reset();
        
        // Custom deleters
        std::cout << "\nUnique_ptr with custom deleter:\n";
        auto custom_deleter = [](Resource* r) {
            std::cout << "Custom deleting " << r->name << "\n";
            delete r;
        };
        
        std::unique_ptr<Resource, decltype(custom_deleter)> 
            uniq5(new Resource("CustomDeleted"), custom_deleter);
        
        std::cout << "End of unique_ptr scope\n";
    }  // All resources automatically cleaned up here
    
    // shared_ptr - shared ownership
    std::cout << "\n--- std::shared_ptr ---\n";
    {
        std::cout << "Creating shared_ptr:\n";
        std::shared_ptr<Resource> shared1(new Resource("Shared1"));
        
        // Modern way with make_shared (preferred for performance)
        std::cout << "\nCreating with make_shared (preferred):\n";
        auto shared2 = std::make_shared<Resource>("Shared2");
        
        // Can copy shared_ptr
        std::cout << "\nCopying shared_ptr:\n";
        std::shared_ptr<Resource> shared3 = shared2;
        
        std::cout << "Reference count for shared2: " << shared2.use_count() << "\n";
        
        // Create a vector of shared pointers to the same resource
        std::cout << "\nCreating multiple references:\n";
        std::vector<std::shared_ptr<Resource>> vec;
        for (int i = 0; i < 3; ++i) {
            vec.push_back(shared2);
            std::cout << "Reference count: " << shared2.use_count() << "\n";
        }
        
        // Clear some references
        std::cout << "\nClearing some references:\n";
        vec.clear();
        std::cout << "Reference count after clear: " << shared2.use_count() << "\n";
        
        std::cout << "End of shared_ptr scope\n";
    }  // All resources automatically cleaned up when last reference is gone
    
    // weak_ptr - non-owning reference
    std::cout << "\n--- std::weak_ptr ---\n";
    {
        // Demonstrating circular reference problem
        struct Node {
            std::string name;
            // std::shared_ptr<Node> next;  // This can cause circular reference
            std::weak_ptr<Node> next;       // This breaks the cycle
            
            Node(const std::string& n) : name(n) {
                std::cout << "Node " << name << " created\n";
            }
            
            ~Node() {
                std::cout << "Node " << name << " destroyed\n";
            }
        };
        
        std::cout << "Creating nodes with weak_ptr to prevent circular references:\n";
        auto node1 = std::make_shared<Node>("Node1");
        auto node2 = std::make_shared<Node>("Node2");
        
        // Create circular reference
        node1->next = node2;
        node2->next = node1;
        
        std::cout << "Reference count for node1: " << node1.use_count() << "\n";
        std::cout << "Reference count for node2: " << node2.use_count() << "\n";
        
        // Using a weak_ptr
        std::cout << "\nAccessing through weak_ptr:\n";
        if (auto temp = node1->next.lock()) {
            std::cout << "Node1 points to " << temp->name << "\n";
        } else {
            std::cout << "Weak pointer expired\n";
        }
        
        // Resetting one shared_ptr
        std::cout << "\nResetting node2:\n";
        node2.reset();
        std::cout << "Reference count for node1: " << node1.use_count() << "\n";
        
        // Try to access through weak_ptr again
        if (auto temp = node1->next.lock()) {
            std::cout << "Node1 still points to " << temp->name << "\n";
        } else {
            std::cout << "Weak pointer expired\n";
        }
        
        std::cout << "End of weak_ptr scope\n";
    }
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. unique_ptr: Use for exclusive ownership with no overhead\n";
    std::cout << "2. shared_ptr: Use for shared ownership with reference counting\n";
    std::cout << "3. weak_ptr: Use to break circular references with shared_ptr\n";
    std::cout << "4. Prefer make_shared over direct constructor for better performance\n";
    std::cout << "5. unique_ptr can be converted to shared_ptr but not vice versa\n";
    std::cout << "6. Custom deleters can be used with all smart pointers\n";
    std::cout << "7. Smart pointers implement RAII for dynamic memory\n";
}

/**
 * @brief Demonstrates variadic templates for handling arbitrary number of arguments
 * 
 * Interview Questions:
 * 1. What are variadic templates and how do they work?
 * 2. How do you implement a recursive variadic template function?
 * 3. What is parameter pack expansion?
 * 4. How do variadic templates compare to traditional approaches like va_args?
 */
void demo_variadic_templates() {
    std::cout << "\n=== Variadic Templates ===\n";
    
    // Basic variadic template function
    std::cout << "\nBasic variadic template function:\n";
    print(1);
    print(2.5);
    print("hello");
    println();
    
    // Recursive variadic template function
    std::cout << "\nRecursive variadic template function:\n";
    println(1, 2.5, "hello", 'c', true);
    
    // Variadic template with parameter pack expansion
    std::cout << "\nVariadic template sum:\n";
    std::cout << "Sum of 1, 2, 3, 4, 5: " << sum_fold(1, 2, 3, 4, 5) << std::endl;
    
    // Tuple creation with variadic templates
    std::cout << "\nTuple with variadic templates:\n";
    auto tuple = make_tuple_demo(1, 2.5, std::string("hello"));
    std::cout << "Tuple elements: " << std::get<0>(tuple) << ", " 
              << std::get<1>(tuple) << ", " << std::get<2>(tuple) << std::endl;
    
    // Variadic class template
    std::cout << "\nVariadic class template example:\n";
    std::cout << "Tuple is a variadic class template that can store any number of types\n";
    
    // Safe printf with variadic templates
    std::cout << "\nSafe printf with variadic templates:\n";
    safe_printf("Values: %,%,% end\n", 10, 20.5, "hello");
    
    // Interview questions
    std::cout << "\nInterview Questions:\n";
    std::cout << "1. What are variadic templates and how do they differ from regular templates?\n";
    std::cout << "   Answer: Variadic templates can accept any number of template arguments of any type.\n";
    std::cout << "           Regular templates have a fixed number of parameters.\n\n";
    
    std::cout << "2. How do you unpack a parameter pack in a variadic template?\n";
    std::cout << "   Answer: Using recursion, fold expressions (C++17), or expansion into initializer lists.\n\n";
    
    std::cout << "3. What is a common use case for variadic templates in C++11?\n";
    std::cout << "   Answer: Implementing type-safe printf-like functions, forwarding arguments in perfect forwarding,\n";
    std::cout << "           and implementing tuple-like data structures.\n";
}

/**
 * @brief Demonstrates initializer lists for uniform initialization
 * 
 * Interview Questions:
 * 1. What is std::initializer_list and how is it used?
 * 2. What is the difference between direct initialization and copy initialization?
 * 3. How does uniform initialization solve the "most vexing parse" problem?
 * 4. When would you use initializer_list constructor vs. other constructors?
 */
void demo_initializer_lists() {
    std::cout << "\n=== Initializer Lists ===\n";
    
    // Basic usage with standard containers
    std::cout << "Vector with initializer list: ";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // Map with initializer list
    std::cout << "Map with initializer list: ";
    std::unordered_map<std::string, int> ages = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };
    for (const auto& pair : ages) {
        std::cout << pair.first << "=" << pair.second << " ";
    }
    std::cout << std::endl;
    
    // Custom class with initializer_list constructor
    class IntContainer {
    private:
        std::vector<int> data;
        
    public:
        // Constructor taking an initializer list
        IntContainer(std::initializer_list<int> list) : data(list) {
            std::cout << "Initializer list constructor called\n";
        }
        
        // Regular constructor
        IntContainer(int size, int value) : data(size, value) {
            std::cout << "Regular constructor called\n";
        }
        
        void print() const {
            std::cout << "Container contents: ";
            for (int i : data) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    };
    
    std::cout << "\nCustom class with initializer list constructor:\n";
    IntContainer c1 = {1, 2, 3, 4, 5};  // Uses initializer_list constructor
    c1.print();
    
    IntContainer c2(3, 7);  // Uses regular constructor
    c2.print();
    
    // Initializer list for function parameters
    auto sum_list = [](std::initializer_list<int> list) {
        int total = 0;
        for (int i : list) {
            total += i;
        }
        return total;
    };
    
    std::cout << "\nFunction with initializer_list parameter:\n";
    std::cout << "Sum of {1, 2, 3, 4, 5}: " << sum_list({1, 2, 3, 4, 5}) << std::endl;
    
    // Uniform initialization
    std::cout << "\nUniform initialization examples:\n";
    
    // Old style initialization
    std::cout << "Old style initialization:\n";
    std::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    
    // Uniform initialization
    std::cout << "Uniform initialization:\n";
    std::vector<int> v2{1, 2};  // Direct list initialization
    
    // Most vexing parse problem
    std::cout << "\nSolving the most vexing parse problem:\n";
    
    class Widget {};
    
    // Widget w1();  // Oops, declares a function, not an object
    Widget w2{};     // Clearly creates an object
    
    std::cout << "Widget w1(); // Declares a function that returns Widget\n";
    std::cout << "Widget w2{}; // Creates a Widget object\n";
    
    // Narrowing conversion prevention
    std::cout << "\nNarrowing conversion prevention:\n";
    
    int x = 1.1;  // Allowed, but loses precision
    // int y{1.1};  // Error: narrowing conversion
    
    std::cout << "int x = 1.1; // Allowed, x = " << x << "\n";
    std::cout << "int y{1.1}; // Error: narrowing conversion\n";
    
    // Different forms of initialization
    std::cout << "\nDifferent forms of initialization:\n";
    
    std::vector<int> v3 = {1, 2, 3};  // Copy initialization
    std::vector<int> v4{1, 2, 3};     // Direct initialization
    auto v5 = std::vector<int>{1, 2, 3};  // Copy initialization with auto
    
    std::cout << "v3 = {1, 2, 3}; // Copy initialization\n";
    std::cout << "v4{1, 2, 3};    // Direct initialization\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. std::initializer_list allows constructor overloading for list initialization\n";
    std::cout << "2. Uniform initialization provides consistent syntax for all types\n";
    std::cout << "3. Braced initialization prevents narrowing conversions\n";
    std::cout << "4. Initializer lists are lightweight proxy objects that refer to arrays\n";
    std::cout << "5. The most vexing parse is resolved by using braces instead of parentheses\n";
    std::cout << "6. Initializer list constructors are preferred over other constructors when using braces\n";
}

/**
 * @brief Demonstrates uniform initialization syntax
 * 
 * Interview Questions:
 * 1. What is uniform initialization and why was it introduced?
 * 2. How does uniform initialization differ from other initialization methods?
 * 3. What are the advantages and potential pitfalls of uniform initialization?
 */
void demo_uniform_initialization() {
    std::cout << "\n=== Uniform Initialization ===\n";
    
    // Different types of initialization before C++11
    std::cout << "Pre-C++11 initialization methods:\n";
    
    int a1 = 10;                // Copy initialization
    int a2(20);                 // Direct initialization
    std::string s1 = "hello";   // Copy initialization
    std::string s2("world");    // Direct initialization
    
    // C++11 uniform initialization
    std::cout << "\nC++11 uniform initialization:\n";
    
    int b1{30};                 // Direct list initialization
    int b2 = {40};              // Copy list initialization
    std::string s3{"hello"};    // Direct list initialization
    std::string s4 = {"world"}; // Copy list initialization
    
    std::cout << "Values: " << b1 << ", " << b2 << ", " << s3 << ", " << s4 << "\n";
    
    // Initialization of arrays
    std::cout << "\nArray initialization:\n";
    
    int arr1[3] = {1, 2, 3};    // Traditional array initialization
    int arr2[3]{4, 5, 6};       // Uniform initialization
    
    std::cout << "arr1: " << arr1[0] << ", " << arr1[1] << ", " << arr1[2] << "\n";
    std::cout << "arr2: " << arr2[0] << ", " << arr2[1] << ", " << arr2[2] << "\n";
    
    // Initialization of dynamically allocated arrays
    std::cout << "\nDynamic array initialization:\n";
    
    int* p1 = new int[3]{7, 8, 9};  // Uniform initialization for dynamic arrays
    std::cout << "p1: " << p1[0] << ", " << p1[1] << ", " << p1[2] << "\n";
    delete[] p1;
    
    // Initialization of member variables
    std::cout << "\nMember variable initialization:\n";
    
    struct Point {
        int x{0};  // Default member initializer
        int y{0};
        
        // Different constructors all using uniform initialization
        Point() = default;
        Point(int x_val) : x{x_val} {}
        Point(int x_val, int y_val) : x{x_val}, y{y_val} {}
    };
    
    Point p2{10, 20};  // Uniform initialization for objects
    std::cout << "Point: (" << p2.x << ", " << p2.y << ")\n";
    
    // Initialization of containers
    std::cout << "\nContainer initialization:\n";
    
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::unordered_map<std::string, int> map{{"one", 1}, {"two", 2}, {"three", 3}};
    
    std::cout << "Vector: ";
    for (int i : vec) std::cout << i << " ";
    std::cout << "\n";
    
    std::cout << "Map: ";
    for (const auto& pair : map) std::cout << pair.first << "=" << pair.second << " ";
    std::cout << "\n";
    
    // Nested initialization
    std::cout << "\nNested initialization:\n";
    
    struct Person {
        std::string name;
        int age;
        std::vector<std::string> hobbies;
    };
    
    Person person{"Alice", 30, {"reading", "hiking", "coding"}};
    
    std::cout << "Person: " << person.name << ", " << person.age << ", Hobbies: ";
    for (const auto& hobby : person.hobbies) std::cout << hobby << " ";
    std::cout << "\n";
    
    // Preventing narrowing conversions
    std::cout << "\nPreventing narrowing conversions:\n";
    
    int n1 = 3.14;  // Allowed, but loses precision
    // int n2{3.14};  // Error: narrowing conversion
    
    std::cout << "int n1 = 3.14; // Allowed, n1 = " << n1 << "\n";
    std::cout << "int n2{3.14}; // Error: narrowing conversion\n";
    
    // Most vexing parse
    std::cout << "\nSolving the most vexing parse:\n";
    
    class Timer {};
    
    // Timer t1();  // Oops, declares a function, not an object
    Timer t2{};     // Clearly creates an object
    
    std::cout << "Timer t1(); // Declares a function that returns Timer\n";
    std::cout << "Timer t2{}; // Creates a Timer object\n";
    
    // Empty braces vs. parentheses
    std::cout << "\nEmpty braces vs. parentheses:\n";
    
    std::vector<int> v1(5);     // Vector with 5 elements, all 0
    std::vector<int> v2{5};     // Vector with 1 element: 5
    
    std::cout << "v1(5): size = " << v1.size() << ", elements: ";
    for (int i : v1) std::cout << i << " ";
    std::cout << "\n";
    
    std::cout << "v2{5}: size = " << v2.size() << ", elements: ";
    for (int i : v2) std::cout << i << " ";
    std::cout << "\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Uniform initialization provides a consistent syntax for initializing objects\n";
    std::cout << "2. Benefits: prevents narrowing conversions, solves most vexing parse, works with arrays\n";
    std::cout << "3. Potential issues: initializer_list constructors are preferred over other constructors\n";
    std::cout << "4. Empty braces {} create value-initialized objects, not default-initialized\n";
    std::cout << "5. Uniform initialization works with auto: auto x{1}; // int\n";
    std::cout << "6. Use uniform initialization for increased type safety and consistency\n";
}

/**
 * @brief Demonstrates type inference features in C++11
 * 
 * Interview Questions:
 * 1. How does type inference with auto and decltype differ?
 * 2. When should you use auto vs. explicit types?
 * 3. What are the limitations of auto type deduction?
 * 4. How does decltype(auto) work in C++14?
 */
void demo_type_inference() {
    std::cout << "\n=== Type Inference ===\n";
    
    // Basic auto usage
    std::cout << "Basic auto usage:\n";
    
    auto i = 42;                // int
    auto d = 42.5;              // double
    auto s = "hello";           // const char*
    auto v = std::vector<int>{1, 2, 3}; // std::vector<int>
    
    std::cout << "auto i = 42;      // Type: int, Value: " << i << "\n";
    std::cout << "auto d = 42.5;    // Type: double, Value: " << d << "\n";
    std::cout << "auto s = \"hello\"; // Type: const char*, Value: " << s << "\n";
    
    // Auto with references and const
    std::cout << "\nAuto with references and pointers:\n";
    
    int x = 10;
    int& rx = x;
    
    auto ax = rx;       // int, not int&
    auto& arx = rx;     // int&
    
    x = 20;
    std::cout << "x = " << x << ", rx = " << rx << ", ax = " << ax << ", arx = " << arx << "\n";
    
    // Auto with const
    std::cout << "\nAuto with const:\n";
    
    const int cx = 10;
    auto acx = cx;      // int, not const int
    const auto cacx = cx; // const int
    auto& arcx = cx;    // const int&
    
    // acx = 20;        // OK, acx is not const
    // cacx = 20;       // Error, cacx is const
    // arcx = 20;       // Error, arcx is const int&
    
    std::cout << "const int cx = 10;\n";
    std::cout << "auto acx = cx;      // Type: int (const dropped)\n";
    std::cout << "const auto cacx = cx; // Type: const int\n";
    std::cout << "auto& arcx = cx;    // Type: const int&\n";
    
    // Auto with initializer lists
    std::cout << "\nAuto with initializer lists:\n";
    
    // auto il = {1, 2, 3};  // std::initializer_list<int>
    std::cout << "auto il = {1, 2, 3};  // Type: std::initializer_list<int>\n";
    
    // Auto with function pointers and lambdas
    std::cout << "\nAuto with function pointers and lambdas:\n";
    
    auto add = [](int a, int b) { return a + b; };
    auto result = add(5, 3);
    
    std::cout << "auto add = [](int a, int b) { return a + b; }; // Type: lambda\n";
    std::cout << "auto result = add(5, 3); // Type: int, Value: " << result << "\n";
    
    // Decltype basics
    std::cout << "\nDecltype basics:\n";
    
    int a = 0;
    decltype(a) b = 20;      // int
    decltype(a + 1.5) c = 30.5; // double
    
    std::cout << "int a = 0;\n";
    std::cout << "decltype(a) b = 20;      // Type: int, Value: " << b << "\n";
    std::cout << "decltype(a + 1.5) c = 30.5; // Type: double, Value: " << c << "\n";
    
    // Decltype with expressions
    std::cout << "\nDecltype with expressions:\n";
    
    int n = 0;
    decltype(n) d1 = 1;        // int
    decltype((n)) d2 = n;      // int&
    
    std::cout << "decltype(n) d1 = 1;    // Type: int\n";
    std::cout << "decltype((n)) d2 = n;  // Type: int&\n";
    
    // Decltype with functions
    std::cout << "\nDecltype with functions:\n";
    
    auto multiply = [](int x, double y) -> double { return x * y; };
    decltype(multiply(5, 2.5)) mult_result = multiply(3, 4.5);
    std::cout << "decltype(multiply(5, 2.5)) mult_result = multiply(3, 4.5);\n";
    std::cout << "// Type: double, Value: " << mult_result << "\n";
    
    // Trailing return type
    std::cout << "\nTrailing return type:\n";
    
    auto subtract = [](int x, int y) -> decltype(x - y) {
        return x - y;
    };
    
    auto sub_result1 = subtract(10, 5);     // int
    auto sub_result2 = subtract(10.5, 5.0); // double
    
    std::cout << "auto subtract = [](int x, int y) -> decltype(x - y) { return x - y; };\n";
    std::cout << "auto sub_result1 = subtract(10, 5);     // Type: int, Value: " << sub_result1 << "\n";
    std::cout << "auto sub_result2 = subtract(10.5, 5.0); // Type: double, Value: " << sub_result2 << "\n";
    
    // decltype(auto) (C++14, but worth mentioning)
    std::cout << "\ndecltype(auto) (C++14 feature):\n";
    std::cout << "decltype(auto) preserves references and cv-qualifiers exactly as decltype would\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. auto deduces type from initializer, decltype from expression\n";
    std::cout << "2. auto drops references and cv-qualifiers, decltype preserves them\n";
    std::cout << "3. auto with reference (auto&) preserves references\n";
    std::cout << "4. decltype((var)) is always a reference type\n";
    std::cout << "5. Use auto for iterator types and when type is obvious\n";
    std::cout << "6. Use decltype for generic programming and perfect forwarding\n";
    std::cout << "7. decltype(auto) in C++14 combines auto and decltype semantics\n";
}

/**
 * @brief Demonstrates constexpr for compile-time computation
 * 
 * Interview Questions:
 * 1. What is constexpr and how does it differ from const?
 * 2. What are the benefits of compile-time computation?
 * 3. What are the limitations of constexpr in C++11 vs C++14/17?
 * 4. How can constexpr improve performance?
 */
void demo_constexpr() {
    std::cout << "\n=== Constexpr ===\n";
    
    // Basic constexpr variable
    constexpr int answer = 42;
    std::cout << "constexpr int answer = 42; // Compile-time constant\n";
    
    // Constexpr with functions
    std::cout << "\nConstexpr function:\n";
    std::cout << "constexpr int square(int x) { return x * x; }\n";
    
    // Compile-time computation
    constexpr int compile_time_result = square(10);
    std::cout << "constexpr int compile_time_result = square(10); // Value: " << compile_time_result << "\n";
    
    // Runtime computation with same function
    int input = 5;
    // constexpr int runtime_result = square(input); // Error: input is not constexpr
    int runtime_result = square(input);
    std::cout << "int runtime_result = square(input); // Value: " << runtime_result << "\n";
    
    // Constexpr with user-defined types
    struct Point {
        int x, y;
        
        // Constexpr constructor
        constexpr Point(int x_val, int y_val) : x(x_val), y(y_val) {}
        
        // Constexpr method
        constexpr int sum() const {
            return x + y;
        }
    };
    
    constexpr Point p1(3, 4);
    constexpr int point_sum = p1.sum();
    
    std::cout << "\nConstexpr with user-defined types:\n";
    std::cout << "constexpr Point p1(3, 4);\n";
    std::cout << "constexpr int point_sum = p1.sum(); // Value: " << point_sum << "\n";
    
    // Constexpr for array sizes
    constexpr int array_size = square(5);
    int array[array_size]; // Legal: array_size is a compile-time constant
    
    std::cout << "\nConstexpr for array sizes:\n";
    std::cout << "constexpr int array_size = square(5); // Value: " << array_size << "\n";
    std::cout << "int array[array_size]; // Legal because array_size is known at compile time\n";
    
    // Recursive constexpr function
    std::cout << "\nRecursive constexpr function:\n";
    std::cout << "constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }\n";
    constexpr int fact5 = factorial(5);
    std::cout << "constexpr int fact5 = factorial(5); // Value: " << fact5 << "\n";
    
    // Limitations in C++11
    std::cout << "\nC++11 constexpr limitations (mentioned for reference):\n";
    std::cout << "- Only one return statement allowed\n";
    std::cout << "- No local variables\n";
    std::cout << "- No loops (must use recursion)\n";
    std::cout << "- No side effects\n";
    
    // C++14 improvements (mentioned for reference)
    std::cout << "\nC++14 constexpr improvements (mentioned for reference):\n";
    std::cout << "- Multiple return statements allowed\n";
    std::cout << "- Local variables allowed\n";
    std::cout << "- Loops allowed\n";
    std::cout << "- More complex logic possible\n";
    
    // Constexpr vs const
    const int const_val = 10;
    constexpr int constexpr_val = 10;
    
    std::cout << "\nconst vs constexpr:\n";
    std::cout << "const int const_val = 10; // May be compile-time or runtime constant\n";
    std::cout << "constexpr int constexpr_val = 10; // Must be compile-time constant\n";
    
    // const can be initialized at runtime
    int runtime_value = 42;
    const int const_runtime = runtime_value; // OK: const can be runtime value
    // constexpr int constexpr_runtime = runtime_value; // Error: constexpr must be compile-time
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. constexpr guarantees compile-time evaluation, const only guarantees immutability\n";
    std::cout << "2. Benefits: faster runtime, smaller binary size, compile-time error detection\n";
    std::cout << "3. C++11 constexpr functions are limited to a single return statement\n";
    std::cout << "4. constexpr functions can be used in both compile-time and runtime contexts\n";
    std::cout << "5. Use constexpr for values that can be computed at compile time\n";
    std::cout << "6. constexpr is particularly useful for template metaprogramming\n";
}

/**
 * @brief Demonstrates scoped enumerations (enum class)
 * 
 * Interview Questions:
 * 1. What are the advantages of enum class over traditional enums?
 * 2. How does enum class improve type safety?
 * 3. How do you specify the underlying type of an enum class?
 * 4. How do you convert between enum class values and underlying types?
 */
void demo_enum_class() {
    std::cout << "\n=== Enum Class ===\n";
    
    // Traditional C-style enum
    enum Color {
        RED,    // 0
        GREEN,  // 1
        BLUE    // 2
    };
    
    // C++11 enum class (scoped enumeration)
    enum class TrafficLight {
        RED,
        YELLOW,
        GREEN
    };
    
    // Using traditional enum
    Color c1 = RED;
    int colorNum = c1;  // Implicit conversion to int
    
    std::cout << "Traditional enum:\n";
    std::cout << "Color c1 = RED; // Value: " << c1 << "\n";
    std::cout << "int colorNum = c1; // Implicit conversion, Value: " << colorNum << "\n";
    
    // Using enum class
    TrafficLight light = TrafficLight::RED;
    // int lightNum = light;  // Error: no implicit conversion
    int lightNum = static_cast<int>(light);  // Explicit conversion required
    
    std::cout << "\nEnum class:\n";
    std::cout << "TrafficLight light = TrafficLight::RED;\n";
    std::cout << "int lightNum = static_cast<int>(light); // Explicit conversion, Value: " << lightNum << "\n";
    
    // Enum class with specified underlying type
    enum class Direction : uint8_t {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };
    
    Direction d = Direction::EAST;
    uint8_t dValue = static_cast<uint8_t>(d);
    
    std::cout << "\nEnum class with specified underlying type:\n";
    std::cout << "enum class Direction : uint8_t { NORTH, EAST, SOUTH, WEST };\n";
    std::cout << "Direction d = Direction::EAST;\n";
    std::cout << "uint8_t dValue = static_cast<uint8_t>(d); // Value: " << static_cast<int>(dValue) << "\n";
    
    // Enum class prevents name conflicts
    enum class Animal {
        CAT,
        DOG,
        BIRD
    };
    
    enum class Pet {
        CAT,  // No conflict with Animal::CAT
        DOG,  // No conflict with Animal::DOG
        FISH
    };
    
    Animal a = Animal::CAT;
    Pet p = Pet::CAT;
    
    std::cout << "\nEnum class prevents name conflicts:\n";
    std::cout << "enum class Animal { CAT, DOG, BIRD };\n";
    std::cout << "enum class Pet { CAT, DOG, FISH };\n";
    std::cout << "Animal a = Animal::CAT; // No conflict with Pet::CAT\n";
    std::cout << "Pet p = Pet::CAT; // No conflict with Animal::CAT\n";
    
    // Traditional enums can cause conflicts
    std::cout << "\nTraditional enums can cause conflicts:\n";
    std::cout << "enum Fruit { APPLE, ORANGE };\n";
    std::cout << "enum Food { APPLE, BREAD }; // Error: redefinition of APPLE\n";
    
    // Enum class with operations
    enum class Day : int {
        MONDAY = 1,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };
    
    Day today = Day::WEDNESDAY;
    Day tomorrow = static_cast<Day>(static_cast<int>(today) + 1);
    
    std::cout << "\nEnum class with operations:\n";
    std::cout << "Day today = Day::WEDNESDAY;\n";
    std::cout << "Day tomorrow = static_cast<Day>(static_cast<int>(today) + 1); // THURSDAY\n";
    
    // Forward declarations possible with enum class
    std::cout << "\nForward declarations possible with enum class:\n";
    std::cout << "enum class Status : int; // Forward declaration\n";
    std::cout << "// ... later ...\n";
    std::cout << "enum class Status : int { ACTIVE, INACTIVE, SUSPENDED };\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. enum class provides stronger type safety than traditional enums\n";
    std::cout << "2. enum class values are scoped to the enum name (Enum::Value)\n";
    std::cout << "3. enum class doesn't allow implicit conversion to int\n";
    std::cout << "4. enum class can have a specified underlying type\n";
    std::cout << "5. enum class can be forward-declared\n";
    std::cout << "6. enum class prevents name conflicts in the same scope\n";
    std::cout << "7. Use enum class by default for new code for better type safety\n";
}

/**
 * @brief Demonstrates static_assert for compile-time assertions
 * 
 * Interview Questions:
 * 1. What is static_assert and how does it differ from runtime assertions?
 * 2. When would you use static_assert?
 * 3. How does static_assert improve code quality?
 * 4. What are the limitations of static_assert?
 */
void demo_static_assert() {
    std::cout << "\n=== Static Assert ===\n";
    
    // Basic static_assert
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    std::cout << "static_assert(sizeof(int) >= 4, \"int must be at least 4 bytes\");\n";
    std::cout << "// Passes because int is " << sizeof(int) << " bytes on this platform\n";
    
    // Static assert with constexpr
    constexpr bool is_64bit = (sizeof(void*) == 8);
    static_assert(is_64bit, "This code requires a 64-bit platform");
    std::cout << "static_assert(is_64bit, \"This code requires a 64-bit platform\");\n";
    std::cout << "// Passes because this is a " << (is_64bit ? "64" : "32") << "-bit platform\n";
    
    // Static assert in templates
    std::cout << "\nStatic assert in templates:\n";
    std::cout << "template <typename T>\n";
    std::cout << "void process_numeric_data(T value) {\n";
    std::cout << "    static_assert(std::is_arithmetic<T>::value, \"T must be a numeric type\");\n";
    std::cout << "    // Process data...\n";
    std::cout << "}\n";
    
    // Example of a failing static_assert (commented out)
    std::cout << "\nExample of a failing static_assert (would cause compilation error):\n";
    std::cout << "// static_assert(sizeof(int) == 8, \"int must be 8 bytes\"); // Error on most platforms\n";
    
    // Static assert without message (C++17, mentioned for reference)
    std::cout << "\nStatic assert without message (C++17 feature):\n";
    std::cout << "// static_assert(sizeof(long) >= 4); // C++17 doesn't require message\n";
    
    // Static assert vs runtime assert
    std::cout << "\nStatic assert vs runtime assert:\n";
    std::cout << "// static_assert(condition, message); // Checked at compile time\n";
    std::cout << "// assert(condition);                 // Checked at runtime, can be disabled\n";
    
    // Using static_assert with type traits
    std::cout << "\nUsing static_assert with type traits:\n";
    std::cout << "template <typename T>\n";
    std::cout << "class Vector {\n";
    std::cout << "    static_assert(std::is_default_constructible<T>::value,\n";
    std::cout << "                  \"Vector requires default-constructible elements\");\n";
    std::cout << "    // ...\n";
    std::cout << "};\n";
    
    // Using static_assert for library requirements
    std::cout << "\nUsing static_assert for library requirements:\n";
    std::cout << "static_assert(__cplusplus >= 201103L, \"This library requires C++11 support\");\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. static_assert performs compile-time assertion checking\n";
    std::cout << "2. Use static_assert to validate template parameters, sizes, and other compile-time constants\n";
    std::cout << "3. static_assert helps catch errors earlier in the development process\n";
    std::cout << "4. static_assert can only check compile-time expressions\n";
    std::cout << "5. static_assert is particularly useful with type traits and template metaprogramming\n";
    std::cout << "6. static_assert messages appear as compilation errors\n";
}

/**
 * @brief Demonstrates C++11 threading features
 * 
 * Interview Questions:
 * 1. What threading facilities does C++11 provide?
 * 2. How do you create and manage threads in C++11?
 * 3. What synchronization primitives are available in C++11?
 * 4. How do you handle thread communication and data sharing?
 * 5. What are the common threading pitfalls and how to avoid them?
 */
void demo_threading() {
    std::cout << "\n=== Threading ===\n";
    
    // Basic thread creation
    std::cout << "Basic thread creation:\n";
    
    auto thread_func = [](int id) {
        std::cout << "Thread " << id << " is running\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
        std::cout << "Thread " << id << " finished\n";
    };
    
    std::thread t1(thread_func, 1);
    std::cout << "Main thread continues execution...\n";
    
    // Joining threads
    t1.join();
    std::cout << "Thread 1 joined\n";
    
    // Detaching threads
    std::cout << "\nDetaching threads:\n";
    std::thread t2(thread_func, 2);
    t2.detach();
    std::cout << "Thread 2 detached (may still be running)\n";
    
    // Checking if thread is joinable
    std::cout << "\nChecking if thread is joinable:\n";
    std::thread t3(thread_func, 3);
    std::cout << "t3.joinable(): " << (t3.joinable() ? "true" : "false") << "\n";
    t3.join();
    std::cout << "After joining, t3.joinable(): " << (t3.joinable() ? "true" : "false") << "\n";
    
    // Thread with class member function
    std::cout << "\nThread with class member function:\n";
    
    class Worker {
    public:
        void work(int iterations) {
            for (int i = 0; i < iterations; ++i) {
                std::cout << "Worker iteration " << i << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    };
    
    Worker worker;
    std::thread t4(&Worker::work, &worker, 3);
    t4.join();
    
    // Mutex for synchronization
    std::cout << "\nMutex for synchronization:\n";
    
    std::mutex mtx;
    int counter = 0;
    
    auto increment = [&](int id, int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(mtx);  // RAII lock
            ++counter;
            std::cout << "Thread " << id << " incremented counter to " << counter << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    
    std::thread t5(increment, 5, 3);
    std::thread t6(increment, 6, 3);
    
    t5.join();
    t6.join();
    
    std::cout << "Final counter value: " << counter << "\n";
    
    // Deadlock avoidance with std::lock
    std::cout << "\nDeadlock avoidance with std::lock:\n";
    
    std::mutex mutex1, mutex2;
    
    auto potential_deadlock = [&](bool reverse_order) {
        if (reverse_order) {
            std::cout << "Thread trying to lock in reverse order\n";
            std::lock(mutex2, mutex1);  // Safe from deadlock
            std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
            std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
            std::cout << "Thread acquired locks in reverse order\n";
        } else {
            std::cout << "Thread trying to lock in normal order\n";
            std::lock(mutex1, mutex2);  // Safe from deadlock
            std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
            std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
            std::cout << "Thread acquired locks in normal order\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    
    std::thread t7(potential_deadlock, false);
    std::thread t8(potential_deadlock, true);
    
    t7.join();
    t8.join();
    
    // Condition variable for thread communication
    std::cout << "\nCondition variable for thread communication:\n";
    
    std::mutex cv_m;
    std::condition_variable cv;
    bool data_ready = false;
    
    auto data_preparation = [&]() {
        std::cout << "Preparing data...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        {
            std::lock_guard<std::mutex> lock(cv_m);
            data_ready = true;
            std::cout << "Data ready!\n";
        }
        
        cv.notify_one();
    };
    
    auto data_processing = [&]() {
        std::cout << "Waiting for data...\n";
        
        std::unique_lock<std::mutex> lock(cv_m);
        cv.wait(lock, [&]{ return data_ready; });
        
        std::cout << "Processing data...\n";
    };
    
    std::thread prep_thread(data_preparation);
    std::thread proc_thread(data_processing);
    
    prep_thread.join();
    proc_thread.join();
    
    // Atomic operations
    std::cout << "\nAtomic operations:\n";
    
    std::atomic<int> atomic_counter(0);
    
    auto atomic_increment = [&](int iterations) {
        for (int i = 0; i < iterations; ++i) {
            ++atomic_counter;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    
    std::thread t9(atomic_increment, 10);
    std::thread t10(atomic_increment, 10);
    
    t9.join();
    t10.join();
    
    std::cout << "Final atomic counter value: " << atomic_counter << "\n";
    
    // Future and promise
    std::cout << "\nFuture and promise:\n";
    
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    auto calculate = [](std::promise<int> p) {
        std::cout << "Calculating result...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        p.set_value(42);
    };
    
    std::thread t11(calculate, std::move(promise));
    
    std::cout << "Waiting for result...\n";
    int result = future.get();
    std::cout << "Result: " << result << "\n";
    
    t11.join();
    
    // Async
    std::cout << "\nAsync:\n";
    
    auto async_task = [](int x, int y) {
        std::cout << "Computing sum asynchronously...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return x + y;
    };
    
    std::future<int> async_result = std::async(std::launch::async, async_task, 10, 20);
    
    std::cout << "Doing other work while async task runs...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    std::cout << "Async result: " << async_result.get() << "\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. C++11 provides std::thread, std::mutex, std::condition_variable, std::atomic, etc.\n";
    std::cout << "2. Always join or detach threads before they go out of scope\n";
    std::cout << "3. Use std::lock_guard for RAII-style mutex locking\n";
    std::cout << "4. Use std::lock to avoid deadlocks when locking multiple mutexes\n";
    std::cout << "5. Use std::atomic for simple thread-safe operations without locks\n";
    std::cout << "6. Use std::future and std::promise for one-time communication between threads\n";
    std::cout << "7. Use std::async for simple asynchronous tasks\n";
    std::cout << "8. Beware of race conditions, deadlocks, and thread synchronization issues\n";
}

/**
 * @brief Demonstrates C++11 regular expression support
 * 
 * Interview Questions:
 * 1. What regex capabilities does C++11 provide?
 * 2. How do you use std::regex for pattern matching?
 * 3. What are the different regex grammars supported in C++11?
 * 4. How do you handle regex exceptions?
 * 5. What are the performance considerations when using regex?
 */
void demo_regex() {
    std::cout << "\n=== Regular Expressions ===\n";
    
    // Basic pattern matching
    std::cout << "Basic pattern matching:\n";
    
    std::string text = "Hello, my email is john.doe@example.com and my phone is (123) 456-7890";
    std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    
    std::cout << "Text: " << text << "\n";
    std::cout << "Pattern: [a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}\n";
    
    bool match = std::regex_search(text, email_pattern);
    std::cout << "Match found: " << (match ? "yes" : "no") << "\n";
    
    // Extracting matches
    std::cout << "\nExtracting matches:\n";
    
    std::smatch matches;
    if (std::regex_search(text, matches, email_pattern)) {
        std::cout << "Found email: " << matches[0] << "\n";
    }
    
    // Finding all matches
    std::cout << "\nFinding all matches:\n";
    
    std::regex word_pattern(R"(\b[a-zA-Z]{4,}\b)");  // Words with 4+ letters
    
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_pattern);
    auto words_end = std::sregex_iterator();
    
    std::cout << "Words with 4+ letters:\n";
    for (auto i = words_begin; i != words_end; ++i) {
        std::cout << "  " << i->str() << "\n";
    }
    std::cout << "Total matches: " << std::distance(words_begin, words_end) << "\n";
    
    // Regex replace
    std::cout << "\nRegex replace:\n";
    
    std::string phone_text = "Call me at (123) 456-7890 or (987) 654-3210";
    std::regex phone_pattern(R"(\(\d{3}\)\s\d{3}-\d{4})");
    std::string redacted = std::regex_replace(phone_text, phone_pattern, "(XXX) XXX-XXXX");
    
    std::cout << "Original: " << phone_text << "\n";
    std::cout << "Redacted: " << redacted << "\n";
    
    // Capture groups
    std::cout << "\nCapture groups:\n";
    
    std::string date_text = "Date: 2023-11-15";
    std::regex date_pattern(R"((\d{4})-(\d{2})-(\d{2}))");
    std::smatch date_matches;
    
    if (std::regex_search(date_text, date_matches, date_pattern)) {
        std::cout << "Full match: " << date_matches[0] << "\n";
        std::cout << "Year: " << date_matches[1] << "\n";
        std::cout << "Month: " << date_matches[2] << "\n";
        std::cout << "Day: " << date_matches[3] << "\n";
    }
    
    // Different regex grammars
    std::cout << "\nDifferent regex grammars:\n";
    std::cout << "std::regex supports multiple grammar options:\n";
    std::cout << "- std::regex::ECMAScript (default)\n";
    std::cout << "- std::regex::basic\n";
    std::cout << "- std::regex::extended\n";
    std::cout << "- std::regex::awk\n";
    std::cout << "- std::regex::grep\n";
    std::cout << "- std::regex::egrep\n";
    
    // Regex with options
    std::cout << "\nRegex with options:\n";
    
    std::string case_text = "Hello HELLO hello";
    std::regex case_pattern("hello", std::regex::icase);  // Case-insensitive
    
    std::cout << "Text: " << case_text << "\n";
    std::cout << "Pattern: hello (case-insensitive)\n";
    
    auto matches_begin = std::sregex_iterator(case_text.begin(), case_text.end(), case_pattern);
    auto matches_end = std::sregex_iterator();
    
    std::cout << "Matches:\n";
    for (auto i = matches_begin; i != matches_end; ++i) {
        std::cout << "  " << i->str() << " at position " << i->position() << "\n";
    }
    
    // Error handling
    std::cout << "\nError handling:\n";
    
    try {
        std::regex invalid_pattern("(unclosed parenthesis");
    } catch (const std::regex_error& e) {
        std::cout << "Regex error: " << e.what() << "\n";
        std::cout << "Error code: " << e.code() << "\n";
    }
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. C++11 provides std::regex, std::smatch, std::regex_search, std::regex_match, std::regex_replace\n";
    std::cout << "2. Use raw string literals (R\"()\") for cleaner regex patterns\n";
    std::cout << "3. C++11 supports different regex grammars (ECMAScript is default)\n";
    std::cout << "4. Handle std::regex_error exceptions for invalid patterns\n";
    std::cout << "5. Regex operations can be computationally expensive, use with care in performance-critical code\n";
    std::cout << "6. Use std::sregex_iterator to find all matches in a string\n";
    std::cout << "7. Use capture groups with std::smatch to extract parts of matches\n";
}

/**
 * @brief Demonstrates C++11 chrono library for time manipulation
 * 
 * Interview Questions:
 * 1. What time facilities does C++11 provide?
 * 2. How do you measure elapsed time in C++11?
 * 3. How do you convert between different time units?
 * 4. What are the advantages of the chrono library over traditional time functions?
 * 5. How do you handle system clock vs steady clock?
 */
void demo_chrono() {
    std::cout << "\n=== Chrono Library ===\n";
    
    // Duration types
    std::cout << "Duration types:\n";
    
    std::chrono::hours h(1);
    std::chrono::minutes m = h;
    std::chrono::seconds s = h;
    std::chrono::milliseconds ms = h;
    std::chrono::microseconds us = h;
    std::chrono::nanoseconds ns = h;
    
    std::cout << "1 hour = " << m.count() << " minutes\n";
    std::cout << "1 hour = " << s.count() << " seconds\n";
    std::cout << "1 hour = " << ms.count() << " milliseconds\n";
    std::cout << "1 hour = " << us.count() << " microseconds\n";
    std::cout << "1 hour = " << ns.count() << " nanoseconds\n";
    
    // Creating durations
    std::cout << "\nCreating durations:\n";
    
    auto day = std::chrono::hours(24);
    auto half_day = day / 2;
    auto quarter_day = day / 4;
    
    std::cout << "1 day = " << day.count() << " hours\n";
    std::cout << "Half day = " << half_day.count() << " hours\n";
    std::cout << "Quarter day = " << quarter_day.count() << " hours\n";
    
    // Duration arithmetic
    std::cout << "\nDuration arithmetic:\n";
    
    auto duration1 = std::chrono::minutes(30);
    auto duration2 = std::chrono::minutes(15);
    
    auto sum = duration1 + duration2;
    auto diff = duration1 - duration2;
    auto mul = duration1 * 2;
    auto div = duration1 / 2;
    
    std::cout << "30min + 15min = " << sum.count() << " minutes\n";
    std::cout << "30min - 15min = " << diff.count() << " minutes\n";
    std::cout << "30min * 2 = " << mul.count() << " minutes\n";
    std::cout << "30min / 2 = " << div.count() << " minutes\n";
    
    // Duration casting
    std::cout << "\nDuration casting:\n";
    
    auto seconds_d = std::chrono::seconds(123);
    auto minutes_d = std::chrono::duration_cast<std::chrono::minutes>(seconds_d);
    
    std::cout << seconds_d.count() << " seconds = " << minutes_d.count() << " minutes (truncated)\n";
    
    // Clock types
    std::cout << "\nClock types:\n";
    std::cout << "1. system_clock - Wall clock time, can be adjusted\n";
    std::cout << "2. steady_clock - Monotonic clock, never adjusted\n";
    std::cout << "3. high_resolution_clock - Clock with the smallest tick period available\n";
    
    // Current time
    std::cout << "\nCurrent time:\n";
    
    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&now_t);
    
    // Measuring elapsed time
    std::cout << "\nMeasuring elapsed time:\n";
    
    auto start = std::chrono::steady_clock::now();
    
    // Do some work
    int sum_result = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum_result += i;
    }
    
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
    
    std::cout << "Sum result: " << sum_result << "\n";
    std::cout << "Elapsed time: " << elapsed_ms.count() << " ms\n";
    std::cout << "Elapsed time: " << elapsed_us.count() << " us\n";
    
    // Time point arithmetic
    std::cout << "\nTime point arithmetic:\n";
    
    auto now_tp = std::chrono::system_clock::now();
    auto tomorrow = now_tp + std::chrono::hours(24);
    auto yesterday = now_tp - std::chrono::hours(24);
    
    auto tomorrow_t = std::chrono::system_clock::to_time_t(tomorrow);
    auto yesterday_t = std::chrono::system_clock::to_time_t(yesterday);
    
    std::cout << "Now: " << std::ctime(&now_t);
    std::cout << "Tomorrow: " << std::ctime(&tomorrow_t);
    std::cout << "Yesterday: " << std::ctime(&yesterday_t);
    
    // Sleep operations
    std::cout << "\nSleep operations:\n";
    
    std::cout << "Sleeping for 100ms...\n";
    auto sleep_start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto sleep_end = std::chrono::steady_clock::now();
    auto sleep_duration = std::chrono::duration_cast<std::chrono::milliseconds>(sleep_end - sleep_start);
    std::cout << "Actual sleep duration: " << sleep_duration.count() << " ms\n";
    
    // Sleep until
    std::cout << "\nSleep until:\n";
    std::cout << "Sleeping until 50ms from now...\n";
    auto until_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
    std::this_thread::sleep_until(until_time);
    std::cout << "Woke up!\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. C++11 chrono provides type-safe time representations\n";
    std::cout << "2. Use steady_clock for measuring elapsed time (monotonic, never adjusted)\n";
    std::cout << "3. Use system_clock for wall clock time (can be adjusted)\n";
    std::cout << "4. Use duration_cast to convert between different time units\n";
    std::cout << "5. Chrono is more type-safe and less error-prone than C-style time functions\n";
    std::cout << "6. Chrono handles unit conversions automatically\n";
    std::cout << "7. Chrono works well with threading facilities (sleep_for, sleep_until)\n";
}

/**
 * @brief Demonstrates C++11 random number generation facilities
 * 
 * Interview Questions:
 * 1. What random number facilities does C++11 provide?
 * 2. How do C++11 random generators differ from rand()?
 * 3. How do you generate random numbers in different distributions?
 * 4. How do you seed random number generators?
 * 5. What are the advantages of the <random> library?
 */
void demo_random() {
    std::cout << "\n=== Random Number Generation ===\n";
    
    // Random engines
    std::cout << "Random engines:\n";
    std::cout << "1. std::mt19937 - Mersenne Twister (recommended for most uses)\n";
    std::cout << "2. std::minstd_rand - Minimal Standard Linear Congruential Generator\n";
    std::cout << "3. std::ranlux24 - RANLUX algorithm with 24-bit precision\n";
    std::cout << "4. std::default_random_engine - Implementation-defined engine\n";
    
    // Basic random number generation
    std::cout << "\nBasic random number generation:\n";
    
    // Using <random> header
    std::cout << "Using <random> header (C++11):\n";
    
    // Random device and generator
    std::cout << "Random integers between 1 and 6 (dice rolls):\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << (rand() % 6) + 1 << " ";
    }
    std::cout << std::endl;
    
    // Uniform distribution
    std::cout << "\nRandom real numbers between 0 and 1:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << static_cast<double>(rand()) / RAND_MAX << " ";
    }
    std::cout << std::endl;
    
    // Interview questions
    std::cout << "\nInterview Questions:\n";
    std::cout << "1. What are the advantages of C++11's <random> library over C's rand()?\n";
    std::cout << "   Answer: Better quality random numbers, more distributions, thread safety,\n";
    std::cout << "           and separation of generation and distribution.\n\n";
    
    std::cout << "2. How do you generate cryptographically secure random numbers in C++11?\n";
    std::cout << "   Answer: Use std::random_device, but be aware that its implementation might not\n";
    std::cout << "           be cryptographically secure on all platforms. For critical applications,\n";
    std::cout << "           consider using platform-specific APIs or dedicated crypto libraries.\n";
}

/**
 * @brief Demonstrates C++11 tuple features
 * 
 * Interview Questions:
 * 1. What is std::tuple and how does it differ from std::pair?
 * 2. How do you create and access elements in a tuple?
 * 3. How do you use std::tie for multiple return values?
 * 4. What are the common use cases for tuples?
 * 5. How do you iterate over a tuple?
 */
void demo_tuple_and_tie() {
    std::cout << "\n=== Tuple and Tie ===\n";
    
    // Creating tuples
    std::cout << "\nCreating tuples:\n";
    std::tuple<int, std::string, double> t1(42, "Hello", 3.14);
    auto t2 = std::make_tuple(10, "World", 2.718);
    
    std::cout << "std::tuple<int, std::string, double> t1(42, \"Hello\", 3.14);\n";
    std::cout << "auto t2 = std::make_tuple(10, \"World\", 2.718);\n";
    
    // Accessing tuple elements
    std::cout << "\nAccessing tuple elements:\n";
    std::cout << "std::get<0>(t1) = " << std::get<0>(t1) << std::endl;
    std::cout << "std::get<1>(t1) = " << std::get<1>(t1) << std::endl;
    std::cout << "std::get<2>(t1) = " << std::get<2>(t1) << std::endl;
    
    // Using std::tie for unpacking
    std::cout << "\nUnpacking with std::tie:\n";
    int a;
    std::string b;
    double c;
    std::tie(a, b, c) = t1;
    
    std::cout << "int a; std::string b; double c;\n";
    std::cout << "std::tie(a, b, c) = t1;\n";
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    
    // Ignoring some elements with std::ignore
    std::cout << "\nUsing std::ignore:\n";
    int x;
    double z;
    std::tie(x, std::ignore, z) = t1;
    
    std::cout << "int x; double z;\n";
    std::cout << "std::tie(x, std::ignore, z) = t1;\n";
    std::cout << "x = " << x << ", z = " << z << std::endl;
    
    // Tuple concatenation
    std::cout << "\nTuple concatenation with tuple_cat:\n";
    auto t3 = std::tuple_cat(t1, t2);
    std::cout << "auto t3 = std::tuple_cat(t1, t2);\n";
    std::cout << "std::tuple_size<decltype(t3)>::value = " << std::tuple_size<decltype(t3)>::value << std::endl;
    
    // Function returning multiple values
    std::cout << "\nFunction returning multiple values:\n";
    auto get_person_info = []() -> std::tuple<std::string, int, double> {
        return std::make_tuple("John Doe", 30, 75.5);
    };
    
    std::string name;
    int age;
    double weight;
    std::tie(name, age, weight) = get_person_info();
    
    std::cout << "auto get_person_info = []() -> std::tuple<std::string, int, double> {\n";
    std::cout << "    return std::make_tuple(\"John Doe\", 30, 75.5);\n";
    std::cout << "};\n\n";
    std::cout << "std::string name; int age; double weight;\n";
    std::cout << "std::tie(name, age, weight) = get_person_info();\n";
    std::cout << "name = " << name << ", age = " << age << ", weight = " << weight << std::endl;
    
    // Interview questions
    std::cout << "\nInterview Questions:\n";
    std::cout << "1. What is std::tuple and how does it differ from std::pair?\n";
    std::cout << "   Answer: std::tuple is a fixed-size collection of heterogeneous values,\n";
    std::cout << "           while std::pair is limited to exactly two values. Tuples can\n";
    std::cout << "           hold any number of values of different types.\n\n";
    
    std::cout << "2. How do you access elements in a tuple?\n";
    std::cout << "   Answer: Using std::get<N>(tuple) where N is the zero-based index.\n";
    std::cout << "           C++14 also allows access by type if the type is unique in the tuple.\n\n";
    
    std::cout << "3. When would you use std::tie?\n";
    std::cout << "   Answer: To unpack a tuple into individual variables, to create a tuple of\n";
    std::cout << "           references for comparison operations, or to ignore certain elements\n";
    std::cout << "           using std::ignore.\n\n";
    
    std::cout << "4. What's the advantage of returning a tuple from a function?\n";
    std::cout << "   Answer: It allows a function to return multiple values of different types\n";
    std::cout << "           without creating a custom struct or class.\n";
}

/**
 * @brief Demonstrates thread_local storage duration
 * 
 * Interview Questions:
 * 1. What is thread_local and how does it differ from other storage durations?
 * 2. When would you use thread_local variables?
 * 3. How does thread_local interact with static and global variables?
 * 4. What are the performance implications of thread_local?
 */
void demo_thread_local() {
    std::cout << "\n=== Thread Local Storage ===\n";
    
    // Basic thread_local variable
    thread_local int thread_counter = 0;
    
    std::cout << "thread_local int thread_counter = 0;\n";
    std::cout << "Initial value in main thread: " << thread_counter << "\n";
    
    thread_counter++;
    std::cout << "After increment in main thread: " << thread_counter << "\n";
    
    // Demonstrating thread_local in multiple threads
    std::cout << "\nDemonstrating thread_local in multiple threads:\n";
    
    auto thread_func = [](int id) {
        std::cout << "Thread " << id << " initial thread_counter: " << thread_counter << "\n";
        
        // Each thread has its own copy
        thread_counter += id;
        
        std::cout << "Thread " << id << " after increment by " << id << ": " << thread_counter << "\n";
        
        // Sleep to allow other threads to run
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        std::cout << "Thread " << id << " final thread_counter: " << thread_counter << "\n";
    };
    
    std::thread t1(thread_func, 1);
    std::thread t2(thread_func, 2);
    std::thread t3(thread_func, 3);
    
    t1.join();
    t2.join();
    t3.join();
    
    std::cout << "Main thread thread_counter after all threads complete: " << thread_counter << "\n";
    
    // Comparing with static
    std::cout << "\nComparing with static:\n";
    
    static int static_counter = 0;
    
    std::cout << "static int static_counter = 0;\n";
    std::cout << "Initial static_counter: " << static_counter << "\n";
    
    auto static_thread_func = [](int id) {
        std::cout << "Thread trying to access static_counter: " << static_counter << "\n";
        static_counter += id;  // All threads modify the same variable
        std::cout << "Thread incremented static_counter to: " << static_counter << "\n";
    };
    
    std::thread st1(static_thread_func, 10);
    std::thread st2(static_thread_func, 20);
    
    st1.join();
    st2.join();
    
    std::cout << "Final static_counter: " << static_counter << "\n";
    
    // Thread local class instance
    std::cout << "\nThread local class instance:\n";
    
    class Counter {
    public:
        Counter() { std::cout << "Counter constructed in thread " << std::this_thread::get_id() << "\n"; }
        ~Counter() { std::cout << "Counter destructed in thread " << std::this_thread::get_id() << "\n"; }
        
        void increment() { ++count; }
        int get_count() const { return count; }
        
    private:
        int count = 0;
    };
    
    thread_local Counter tl_counter;
    
    std::cout << "thread_local Counter tl_counter;\n";
    std::cout << "Main thread counter: " << tl_counter.get_count() << "\n";
    tl_counter.increment();
    std::cout << "Main thread counter after increment: " << tl_counter.get_count() << "\n";
    
    auto counter_thread_func = [](int id) {
        std::cout << "Thread " << id << " counter initial: " << tl_counter.get_count() << "\n";
        for (int i = 0; i < id; ++i) {
            tl_counter.increment();
        }
        std::cout << "Thread " << id << " counter after " << id << " increments: " << tl_counter.get_count() << "\n";
    };
    
    std::thread ct1(counter_thread_func, 5);
    std::thread ct2(counter_thread_func, 3);
    
    ct1.join();
    ct2.join();
    
    std::cout << "Main thread counter after all threads complete: " << tl_counter.get_count() << "\n";
    
    // Thread local with dynamic initialization
    std::cout << "\nThread local with dynamic initialization:\n";
    
    auto get_thread_id = []() {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return oss.str();
    };
    
    thread_local std::string thread_id = get_thread_id();
    
    std::cout << "thread_local std::string thread_id = get_thread_id();\n";
    std::cout << "Main thread ID: " << thread_id << "\n";
    
    auto id_thread_func = []() {
        std::cout << "Thread ID: " << thread_id << "\n";
    };
    
    std::thread id1(id_thread_func);
    std::thread id2(id_thread_func);
    
    id1.join();
    id2.join();
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. thread_local variables have thread storage duration\n";
    std::cout << "2. Each thread has its own copy of thread_local variables\n";
    std::cout << "3. thread_local variables are initialized when a thread first accesses them\n";
    std::cout << "4. thread_local variables are destroyed when the thread exits\n";
    std::cout << "5. Use thread_local for thread-specific data that needs to persist across function calls\n";
    std::cout << "6. thread_local can be combined with static or global variables\n";
    std::cout << "7. thread_local can have performance implications due to TLS (Thread Local Storage) lookups\n";
}

/**
 * @brief Demonstrates delegating constructors
 * 
 * Interview Questions:
 * 1. What are delegating constructors and why are they useful?
 * 2. How do you implement delegating constructors?
 * 3. What are the limitations of delegating constructors?
 * 4. How do delegating constructors improve code maintainability?
 */
void demo_delegating_constructors() {
    std::cout << "\n=== Delegating Constructors ===\n";
    
    // Class with delegating constructors
    class Person {
    private:
        std::string name;
        int age;
        std::string address;
        
    public:
        // Primary constructor
        Person(const std::string& name, int age, const std::string& address)
            : name(name), age(age), address(address) {
            std::cout << "Primary constructor called\n";
        }
        
        // Delegating constructor 1
        Person(const std::string& name, int age)
            : Person(name, age, "Unknown") {
            std::cout << "Delegating constructor 1 called\n";
        }
        
        // Delegating constructor 2
        Person(const std::string& name)
            : Person(name, 0) {
            std::cout << "Delegating constructor 2 called\n";
        }
        
        // Default constructor delegating to the primary constructor
        Person()
            : Person("John Doe", 30, "123 Main St") {
            std::cout << "Default constructor called\n";
        }
        
        // Display person information
        void display() const {
            std::cout << "Person: name=" << name << ", age=" << age << ", address=" << address << "\n";
        }
    };
    
    std::cout << "Creating Person objects with different constructors:\n\n";
    
    std::cout << "Person p1(\"Alice\", 25, \"456 Oak St\");\n";
    Person p1("Alice", 25, "456 Oak St");
    p1.display();
    std::cout << "\n";
    
    std::cout << "Person p2(\"Bob\", 30);\n";
    Person p2("Bob", 30);
    p2.display();
    std::cout << "\n";
    
    std::cout << "Person p3(\"Charlie\");\n";
    Person p3("Charlie");
    p3.display();
    std::cout << "\n";
    
    std::cout << "Person p4;\n";
    Person p4;
    p4.display();
    
    // Class with constructor delegation chain
    std::cout << "\nClass with constructor delegation chain:\n";
    
    class Rectangle {
    private:
        double width;
        double height;
        std::string color;
        
    public:
        // Primary constructor
        Rectangle(double width, double height, const std::string& color)
            : width(width), height(height), color(color) {
            std::cout << "Rectangle primary constructor called\n";
        }
        
        // Delegating to primary with default color
        Rectangle(double width, double height)
            : Rectangle(width, height, "white") {
            std::cout << "Rectangle constructor with width and height called\n";
        }
        
        // Delegating to create a square
        Rectangle(double side)
            : Rectangle(side, side) {
            std::cout << "Rectangle square constructor called\n";
        }
        
        // Default constructor creates a unit square
        Rectangle()
            : Rectangle(1.0) {
            std::cout << "Rectangle default constructor called\n";
        }
        
        void display() const {
            std::cout << "Rectangle: width=" << width << ", height=" << height
                      << ", color=" << color << "\n";
        }
    };
    
    std::cout << "\nRectangle r1(5.0, 3.0, \"blue\");\n";
    Rectangle r1(5.0, 3.0, "blue");
    r1.display();
    std::cout << "\n";
    
    std::cout << "Rectangle r2(4.0, 2.0);\n";
    Rectangle r2(4.0, 2.0);
    r2.display();
    std::cout << "\n";
    
    std::cout << "Rectangle r3(3.0);\n";
    Rectangle r3(3.0);
    r3.display();
    std::cout << "\n";
    
    std::cout << "Rectangle r4;\n";
    Rectangle r4;
    r4.display();
    
    // Limitations and best practices
    std::cout << "\nLimitations and best practices:\n";
    std::cout << "1. A constructor cannot both delegate and use member initializer lists\n";
    std::cout << "2. Avoid circular delegation (will cause compilation error)\n";
    std::cout << "3. Delegating constructors can improve code maintainability\n";
    std::cout << "4. Common initialization code should be in the most comprehensive constructor\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Delegating constructors allow one constructor to call another constructor\n";
    std::cout << "2. They help eliminate code duplication in constructors\n";
    std::cout << "3. Delegating constructors improve maintainability by centralizing initialization logic\n";
    std::cout << "4. A constructor cannot both delegate and initialize members directly\n";
    std::cout << "5. Delegation must be the first thing in the constructor initializer list\n";
    std::cout << "6. Circular delegation will result in a compilation error\n";
    std::cout << "7. Delegating constructors are particularly useful for classes with many constructors\n";
}

/**
 * @brief Demonstrates explicit overrides and final specifiers
 * 
 * Interview Questions:
 * 1. What are the override and final specifiers and why are they useful?
 * 2. How do you use override and final to improve code safety?
 * 3. What problems can override and final help prevent?
 * 4. When would you use final on a class vs. a method?
 */
void demo_explicit_overrides() {
    std::cout << "\n=== Explicit Overrides and Final ===\n";
    
    // Base class with virtual functions
    class Base {
    public:
        virtual ~Base() = default; // Add virtual destructor
        
        virtual void foo() {
            std::cout << "Base::foo()\n";
        }
        
        virtual void bar(int x) {
            std::cout << "Base::bar(int): " << x << "\n";
        }
    };
    
    // Derived class with explicit overrides
    class Derived : public Base {
    public:
        // Explicitly marked as override
        void foo() override {
            std::cout << "Derived::foo()\n";
        }
        
        // Explicitly marked as override
        void bar(int x) override {
            std::cout << "Derived::bar(int): " << x << "\n";
        }
        
        // This would cause a compilation error - doesn't override anything
        // void baz() override { }
    };
    
    std::cout << "Base and Derived classes with override specifier:\n";
    std::cout << "class Base {\n";
    std::cout << "public:\n";
    std::cout << "    virtual void foo() { ... }\n";
    std::cout << "    virtual void bar(int x) { ... }\n";
    std::cout << "};\n\n";
    
    std::cout << "class Derived : public Base {\n";
    std::cout << "public:\n";
    std::cout << "    void foo() override { ... }\n";
    std::cout << "    void bar(int x) override { ... }\n";
    std::cout << "    // void baz() override { } // Error: doesn't override anything\n";
    std::cout << "};\n";
    
    // Demonstrate polymorphism
    std::cout << "\nDemonstrating polymorphism:\n";
    
    Base* b1 = new Base();
    Base* b2 = new Derived();
    
    b1->foo();  // Calls Base::foo()
    b2->foo();  // Calls Derived::foo()
    
    b1->bar(10);  // Calls Base::bar()
    b2->bar(20);  // Calls Derived::bar()
    
    delete b1;
    delete b2;
    
    // Common errors that override prevents
    std::cout << "\nCommon errors that 'override' prevents:\n";
    std::cout << "1. Different parameter types:\n";
    std::cout << "   virtual void method(int x);\n";
    std::cout << "   void method(double x) override; // Error: doesn't override\n\n";
    
    std::cout << "2. Different const qualification:\n";
    std::cout << "   virtual void method();\n";
    std::cout << "   void method() const override; // Error: doesn't override\n\n";
    
    std::cout << "3. Different return type:\n";
    std::cout << "   virtual int method();\n";
    std::cout << "   double method() override; // Error: doesn't override\n\n";
    
    std::cout << "4. Misspelled method name:\n";
    std::cout << "   virtual void process();\n";
    std::cout << "   void proccess() override; // Error: doesn't override\n";
    
    // Final specifier for methods
    class FinalMethodBase {
    public:
        virtual void foo() {
            std::cout << "FinalMethodBase::foo()\n";
        }
        
        virtual void bar() final {
            std::cout << "FinalMethodBase::bar() - marked final\n";
        }
    };
    
    class FinalMethodDerived : public FinalMethodBase {
    public:
        void foo() override {
            std::cout << "FinalMethodDerived::foo()\n";
        }
        
        // This would cause a compilation error - cannot override final method
        // void bar() override { }
    };
    
    std::cout << "\nFinal methods:\n";
    std::cout << "class FinalMethodBase {\n";
    std::cout << "public:\n";
    std::cout << "    virtual void foo() { ... }\n";
    std::cout << "    virtual void bar() final { ... }\n";
    std::cout << "};\n\n";
    
    std::cout << "class FinalMethodDerived : public FinalMethodBase {\n";
    std::cout << "public:\n";
    std::cout << "    void foo() override { ... }\n";
    std::cout << "    // void bar() override { } // Error: cannot override final method\n";
    std::cout << "};\n";
    
    // Demonstrate final methods
    std::cout << "\nDemonstrating final methods:\n";
    
    FinalMethodBase* fb = new FinalMethodDerived();
    fb->foo();  // Calls FinalMethodDerived::foo()
    fb->bar();  // Calls FinalMethodBase::bar()
    
    delete fb;
    
    // Final specifier for classes
    class FinalClass final {
    public:
        void method() {
            std::cout << "FinalClass::method()\n";
        }
    };
    
    // This would cause a compiler error:
    // class DerivedFromFinal : public FinalClass { };
    
    std::cout << "\nFinal classes:\n";
    std::cout << "class FinalClass final {\n";
    std::cout << "public:\n";
    std::cout << "    void method() { ... }\n";
    std::cout << "};\n\n";
    
    std::cout << "// class DerivedFromFinal : public FinalClass { }; // Error: cannot inherit from final class\n";
    
    // Real-world example: Template Method Pattern with safeguards
    std::cout << "\nReal-world example: Template Method Pattern with safeguards:\n";
    
    class Algorithm {
    public:
        // Template method - final to prevent override
        virtual void execute() final {
            initialize();
            step1();
            step2();
            cleanup();
        }
        
    protected:
        // Customizable steps
        virtual void initialize() {
            std::cout << "Algorithm::initialize()\n";
        }
        
        virtual void step1() = 0;  // Pure virtual
        
        virtual void step2() {
            std::cout << "Algorithm::step2()\n";
        }
        
        // Not meant to be overridden
        virtual void cleanup() final {
            std::cout << "Algorithm::cleanup()\n";
        }
    };
    
    class ConcreteAlgorithm : public Algorithm {
    protected:
        void initialize() override {
            std::cout << "ConcreteAlgorithm::initialize()\n";
        }
        
        void step1() override {
            std::cout << "ConcreteAlgorithm::step1()\n";
        }
        
        void step2() override {
            std::cout << "ConcreteAlgorithm::step2()\n";
        }
        
        // Cannot override cleanup() because it's final
    };
    
    std::cout << "Template Method Pattern with final methods:\n";
    
    ConcreteAlgorithm algo;
    algo.execute();
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. 'override' ensures a method actually overrides a base class method\n";
    std::cout << "2. 'final' prevents further overriding of methods or inheritance of classes\n";
    std::cout << "3. Both specifiers improve code safety and catch errors at compile time\n";
    std::cout << "4. Use 'override' for all overriding methods to prevent subtle bugs\n";
    std::cout << "5. Use 'final' for methods that must not be overridden for correctness or security\n";
    std::cout << "6. Use 'final' for classes that are not designed for inheritance\n";
    std::cout << "7. These specifiers help document design intent and enforce it at compile time\n";
}

/**
 * @file cpp11_basic_features.cpp
 * @brief Basic demonstration of C++11 features for interview preparation
 * 
 * This file demonstrates core C++11 features in a structured way with
 * practical examples that might be asked in interviews.
 * 
 * Compilation: g++ -std=c++11 -pthread -o cpp11_basic_features cpp11_basic_features.cpp
 * Usage: ./cpp11_basic_features
 */

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

// Forward declarations for all feature demonstration functions
void demo_auto_and_decltype();
void demo_nullptr();
void demo_range_based_for();
void demo_lambda_expressions();
void demo_move_semantics();
void demo_smart_pointers();

int main() {
    std::cout << "=== C++11 Features Demonstration ===\n";
    
    int choice = 0;
    bool exit_program = false;
    
    while (!exit_program) {
        std::cout << "\nSelect a feature to demonstrate:\n";
        std::cout << "1. Auto and Decltype\n";
        std::cout << "2. Nullptr\n";
        std::cout << "3. Range-based For Loop\n";
        std::cout << "4. Lambda Expressions\n";
        std::cout << "5. Move Semantics\n";
        std::cout << "6. Smart Pointers\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                exit_program = true;
                break;
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
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    std::cout << "\nThank you for exploring C++11 features!\n";
    return 0;
}

void demo_auto_and_decltype() {
    std::cout << "\n=== Auto and Decltype ===\n";
    
    // Auto basics
    std::cout << "Auto basics:\n";
    
    auto i = 42;                // int
    auto d = 42.5;              // double
    auto s = "hello";           // const char*
    auto v = std::vector<int>{1, 2, 3}; // std::vector<int>
    
    std::cout << "auto i = 42;                // Type: int, Value: " << i << "\n";
    std::cout << "auto d = 42.5;              // Type: double, Value: " << d << "\n";
    std::cout << "auto s = \"hello\";           // Type: const char*, Value: " << s << "\n";
    std::cout << "auto v = std::vector<int>{1, 2, 3}; // Type: std::vector<int>, Size: " << v.size() << "\n";
    
    // Auto with references and const
    std::cout << "\nAuto with references and const:\n";
    
    const auto& ri = i;           // const int&
    
    std::cout << "const auto& ri = i;           // Type: const int&, Value: " << ri << "\n";
    
    // Decltype basics
    std::cout << "\nDecltype basics:\n";
    
    int x = 10;
    decltype(x) y = 5;            // int
    decltype(x + 5.5) z = 10.5;   // double
    
    std::cout << "int x = 10;\n";
    std::cout << "decltype(x) y = 5;            // Type: int, Value: " << y << "\n";
    std::cout << "decltype(x + 5.5) z = 10.5;   // Type: double, Value: " << z << "\n";
    
    // Decltype with expressions
    std::cout << "\nDecltype with expressions:\n";
    
    const int c = 20;
    auto c1 = c;      // int, not const int
    
    std::cout << "const int c = 20;\n";
    std::cout << "auto c1 = c;      // Type: int, Value: " << c1 << "\n";
    
    decltype(c) c2 = 30; // const int
    std::cout << "decltype(c) c2 = 30; // Type: const int, Value: " << c2 << "\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. 'auto' deduces the type from the initializer\n";
    std::cout << "2. 'auto' drops references and cv-qualifiers (const/volatile)\n";
    std::cout << "3. 'decltype' gives you the exact type of an expression\n";
    std::cout << "4. Use 'auto' to simplify complex type declarations\n";
    std::cout << "5. Use 'decltype' when you need the exact type including references and cv-qualifiers\n";
}

void demo_nullptr() {
    std::cout << "\n=== Nullptr ===\n";
    
    // Traditional NULL pointer
    std::cout << "Traditional NULL pointer:\n";
    
    int* p1 = NULL;
    int* p2 = 0;
    
    std::cout << "int* p1 = NULL;\n";
    std::cout << "int* p2 = 0;\n";
    
    // nullptr
    std::cout << "\nC++11 nullptr:\n";
    
    int* p3 = nullptr;
    
    std::cout << "int* p3 = nullptr;\n";
    
    // Type safety with nullptr
    std::cout << "\nType safety with nullptr:\n";
    
    void* null_type = nullptr;
    char* charPtr = static_cast<char*>(null_type);
    double* doublePtr = nullptr;
    
    std::cout << "void* null_type = nullptr;\n";
    std::cout << "char* charPtr = static_cast<char*>(null_type);\n";
    std::cout << "double* doublePtr = nullptr;\n";
    
    // Function overloading with nullptr
    std::cout << "\nFunction overloading with nullptr:\n";
    
    auto overloaded = [](int i) { 
        std::cout << "Called with int: " << i << "\n"; 
    };
    
    auto overloaded2 = [](int* p) { 
        std::cout << "Called with pointer: " << (p ? "non-null" : "null") << "\n"; 
    };
    
    overloaded(5);
    overloaded2(nullptr);
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. nullptr is a keyword representing a null pointer literal\n";
    std::cout << "2. nullptr has type std::nullptr_t\n";
    std::cout << "3. nullptr can be implicitly converted to any pointer type\n";
    std::cout << "4. nullptr solves the NULL/0 ambiguity in function overloading\n";
    std::cout << "5. Always use nullptr instead of NULL or 0 for pointers\n";
}

void demo_range_based_for() {
    std::cout << "\n=== Range-based For Loop ===\n";
    
    // With C-style arrays
    std::cout << "With C-style arrays: ";
    
    int numbers[] = {1, 2, 3, 4, 5};
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // With STL containers
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};
    std::cout << "With STL containers: ";
    for (const auto& fruit : fruits) {
        std::cout << fruit << " ";
    }
    std::cout << std::endl;
    
    // Modifying elements
    std::cout << "Modifying vector elements: ";
    for (auto& fruit : fruits) {
        fruit = "modified_" + fruit;
    }
    
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
    
    std::cout << "With map: ";
    for (const auto& pair : ages) {
        std::cout << pair.first << "=" << pair.second << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Range-based for loop simplifies iteration over containers\n";
    std::cout << "2. Works with arrays, STL containers, and initializer lists\n";
    std::cout << "3. Use 'auto&' to modify elements, 'const auto&' for read-only access\n";
    std::cout << "4. More concise and less error-prone than traditional for loops\n";
    std::cout << "5. Internally uses begin() and end() functions\n";
}

void demo_lambda_expressions() {
    std::cout << "\n=== Lambda Expressions ===\n";
    
    // Basic lambda
    std::cout << "Basic lambda:\n";
    
    auto hello = []() { std::cout << "Hello, Lambda!\n"; };
    hello();
    
    // Lambda with parameters
    std::cout << "\nLambda with parameters:\n";
    
    auto add = [](int a, int b) { return a + b; };
    std::cout << "add(3, 4): " << add(3, 4) << "\n";
    
    // Lambda with capture
    std::cout << "\nLambda with capture:\n";
    
    int a = 5, b = 10, c = 15;
    
    auto capture_by_value = [a]() { return a * 2; };
    std::cout << "capture_by_value(): " << capture_by_value() << "\n";
    
    auto capture_by_ref = [&b]() { b = 20; return b; };
    std::cout << "capture_by_ref(): " << capture_by_ref() << "\n";
    std::cout << "b after capture_by_ref(): " << b << "\n";
    
    auto capture_all_val = [=]() { return a + b + c; };
    auto capture_all_ref = [&]() { a = 10; b = 20; c = 30; };
    
    auto mixed_capture = [a, &b]() { 
        // a is captured by value, b by reference
        return a + b;
    };
    
    // Lambda with mutable
    std::cout << "\nLambda with mutable:\n";
    
    int count = 0;
    auto counter = [&count]() mutable { return ++count; };
    std::cout << "counter(): " << counter() << "\n";
    std::cout << "counter(): " << counter() << "\n";
    std::cout << "counter(): " << counter() << "\n";
    
    // Lambda capturing this
    std::cout << "\nLambda capturing this:\n";
    
    struct MyClass {
        int value = 100;
        
        // Return the value directly
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
    std::cout << "\nUsing lambdas with algorithms:\n";
    
    std::cout << "Original vector: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    
    // Find first even number
    auto it = std::find_if(nums.begin(), nums.end(), [](int n) { return n % 2 == 0; });
    std::cout << "First even number: " << *it << "\n";
    
    // Count even numbers
    auto even_count = std::count_if(nums.begin(), nums.end(), [](int n) { return n % 2 == 0; });
    std::cout << "Count of even numbers: " << even_count << "\n";
    
    // Transform vector - multiply each element by 2
    std::transform(nums.begin(), nums.end(), nums.begin(), [](int n) { return n * 2; });
    
    std::cout << "After transform (multiply by 2): ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Lambda expressions create anonymous function objects\n";
    std::cout << "2. Syntax: [capture](parameters) -> return_type { body }\n";
    std::cout << "3. Capture clause can capture variables by value [=] or reference [&]\n";
    std::cout << "4. Use 'mutable' to modify captured variables\n";
    std::cout << "5. Lambdas are often used with STL algorithms\n";
    std::cout << "6. Lambdas can improve code locality and readability\n";
}

// Resource class for move semantics demonstration
class Resource {
private:
    int* data;
    int id;
    
public:
    // Constructor
    Resource(int id_val = 0) : id(id_val) {
        data = new int[100];
        std::cout << "Resource " << id << " constructed\n";
    }
    
    // Destructor
    ~Resource() {
        delete[] data;
        std::cout << "Resource " << id << " destroyed\n";
    }
    
    // Copy constructor
    Resource(const Resource& other) : id(other.id) {
        data = new int[100];
        std::copy(other.data, other.data + 100, data);
        std::cout << "Resource " << id << " copy constructed\n";
    }
    
    // Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data;
            data = new int[100];
            id = other.id;
            std::copy(other.data, other.data + 100, data);
            std::cout << "Resource " << id << " copy assigned\n";
        }
        return *this;
    }
    
    // Move constructor
    Resource(Resource&& other) noexcept : data(other.data), id(other.id) {
        other.data = nullptr;
        other.id = 0;
        std::cout << "Resource " << id << " move constructed\n";
    }
    
    // Move assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            id = other.id;
            other.data = nullptr;
            other.id = 0;
            std::cout << "Resource " << id << " move assigned\n";
        }
        return *this;
    }
    
    // Display resource info
    void display() const {
        std::cout << "Resource " << id << " at " << data << "\n";
    }
    
    int getValue() const { return id; }
};

void demo_move_semantics() {
    std::cout << "\n=== Move Semantics ===\n";
    
    // Creating a resource
    std::cout << "Creating a resource:\n";
    Resource r1(1);
    r1.display();
    
    // Copy semantics
    std::cout << "\nCopy semantics:\n";
    Resource r2 = r1;  // Copy constructor
    r2.display();
    
    Resource r3(3);
    r3 = r1;  // Copy assignment
    r3.display();
    
    // Move semantics
    std::cout << "\nMove semantics:\n";
    Resource r4 = std::move(r2);  // Move constructor
    r4.display();
    r2.display();  // r2 is now in a moved-from state
    
    r3 = std::move(r4);  // Move assignment
    r3.display();
    r4.display();  // r4 is now in a moved-from state
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Move semantics allow transferring resources instead of copying\n";
    std::cout << "2. std::move() converts an object to an rvalue reference\n";
    std::cout << "3. Move operations should leave the source object in a valid but unspecified state\n";
    std::cout << "4. Move semantics are particularly useful for large objects or unique resources\n";
    std::cout << "5. Rule of five: if you define one of destructor, copy constructor, copy assignment,\n";
    std::cout << "   move constructor, or move assignment, you should consider defining all five\n";
}

void demo_smart_pointers() {
    std::cout << "\n=== Smart Pointers ===\n";
    
    // unique_ptr
    std::cout << "unique_ptr:\n";
    
    std::unique_ptr<Resource> up1(new Resource(1));
    up1->display();
    
    // Cannot copy unique_ptr
    // std::unique_ptr<Resource> up2 = up1; // Error: copy constructor is deleted
    
    // Can move unique_ptr
    std::unique_ptr<Resource> up3 = std::move(up1);
    if (up3) {
        std::cout << "up3 is valid\n";
        up3->display();
    }
    
    if (!up1) {
        std::cout << "up1 is now nullptr after move\n";
    }
    
    // make_unique (C++14, mentioned for reference)
    std::cout << "\nmake_unique (C++14, mentioned for reference):\n";
    // auto up4 = std::make_unique<Resource>(4);
    
    // shared_ptr
    std::cout << "\nshared_ptr:\n";
    
    std::shared_ptr<Resource> sp1(new Resource(5));
    std::cout << "sp1 use count: " << sp1.use_count() << "\n";
    
    {
        std::shared_ptr<Resource> sp2 = sp1;
        std::cout << "sp1 use count after copy: " << sp1.use_count() << "\n";
        sp2->display();
    }
    
    std::cout << "sp1 use count after sp2 goes out of scope: " << sp1.use_count() << "\n";
    
    // make_shared
    std::cout << "\nmake_shared:\n";
    
    auto sp3 = std::make_shared<Resource>(6);
    std::cout << "sp3 use count: " << sp3.use_count() << "\n";
    
    // weak_ptr
    std::cout << "\nweak_ptr:\n";
    
    std::weak_ptr<Resource> wp1 = sp3;
    std::cout << "wp1 expired? " << std::boolalpha << wp1.expired() << "\n";
    
    if (auto sp4 = wp1.lock()) {
        std::cout << "Got shared_ptr from weak_ptr:\n";
        sp4->display();
    }
    
    sp3.reset();
    std::cout << "After sp3.reset(), wp1 expired? " << wp1.expired() << "\n";
    
    std::cout << "\nInterview Notes:\n";
    std::cout << "1. Smart pointers manage memory automatically\n";
    std::cout << "2. unique_ptr: exclusive ownership, cannot be copied, can be moved\n";
    std::cout << "3. shared_ptr: shared ownership with reference counting\n";
    std::cout << "4. weak_ptr: non-owning reference to shared_ptr, breaks circular references\n";
    std::cout << "5. Use make_shared for better performance (single allocation)\n";
    std::cout << "6. Smart pointers help prevent memory leaks and dangling pointers\n";
}

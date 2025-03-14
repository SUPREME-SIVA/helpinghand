/**
 * C++17 Basic Features
 * 
 * This file demonstrates the fundamental features introduced in C++17.
 * Each feature is explained with examples and comments to help with interview preparation.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include <string_view>
#include <algorithm>
#include <numeric>

// ======================================================================
// Structured Bindings
// ======================================================================
void demo_structured_bindings() {
    std::cout << "\n=== Structured Bindings ===\n";
    
    // 1. With arrays
    int array[3] = {1, 2, 3};
    auto [a, b, c] = array;
    std::cout << "Array elements: " << a << ", " << b << ", " << c << "\n";
    
    // 2. With std::pair
    std::pair<std::string, int> person = {"Alice", 30};
    auto [name, age] = person;
    std::cout << "Person: " << name << ", " << age << " years old\n";
    
    // 3. With std::tuple
    std::tuple<std::string, int, double> employee = {"Bob", 42, 7500.50};
    auto [emp_name, emp_age, salary] = employee;
    std::cout << "Employee: " << emp_name << ", " << emp_age << " years old, $" << salary << "\n";
    
    // 4. With custom struct
    struct Point { int x; int y; };
    Point p = {10, 20};
    auto [x, y] = p;
    std::cout << "Point: (" << x << ", " << y << ")\n";
    
    // 5. With map iterator
    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}, {"Charlie", 92}};
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << "\n";
    }
    
    // Interview tip: Structured bindings make code more readable by allowing
    // direct unpacking of tuples, pairs, arrays, and structs into named variables
}

// ======================================================================
// if/switch with Initialization
// ======================================================================
void demo_if_switch_init() {
    std::cout << "\n=== if/switch with Initialization ===\n";
    
    // 1. if with initialization
    if (int value = 42; value > 40) {
        std::cout << "Value " << value << " is greater than 40\n";
    } else {
        std::cout << "Value " << value << " is not greater than 40\n";
    }
    
    // 2. switch with initialization
    switch (int dice = rand() % 6 + 1; dice) {
        case 1:
            std::cout << "Rolled 1\n";
            break;
        case 6:
            std::cout << "Rolled 6\n";
            break;
        default:
            std::cout << "Rolled " << dice << "\n";
            break;
    }
    
    // 3. Practical example with map lookup
    std::map<std::string, int> ages = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};
    
    if (auto it = ages.find("Bob"); it != ages.end()) {
        std::cout << "Found Bob, age: " << it->second << "\n";
    } else {
        std::cout << "Bob not found\n";
    }
    
    // Interview tip: This feature helps limit the scope of variables used only for
    // conditional checks, reducing potential naming conflicts and improving code clarity
}

// ======================================================================
// Inline Variables
// ======================================================================
// Before C++17, you couldn't define non-const static variables in header files
// Now with inline variables, you can define them once in a header file
inline int globalCounter = 0;

class Widget {
public:
    // Inline static class member (no separate definition needed)
    inline static int objectCount = 0;
    
    Widget() { objectCount++; }
    ~Widget() { objectCount--; }
};

void demo_inline_variables() {
    std::cout << "\n=== Inline Variables ===\n";
    
    globalCounter++;
    std::cout << "Global counter: " << globalCounter << "\n";
    
    Widget w1, w2, w3;
    std::cout << "Widget count: " << Widget::objectCount << "\n";
    
    // Interview tip: Inline variables solve the "multiple definition" problem
    // for variables that need to be defined in header files and included in
    // multiple translation units
}

// ======================================================================
// std::string_view
// ======================================================================
void process_string_view(std::string_view sv) {
    std::cout << "Processing: " << sv << " (length: " << sv.length() << ")\n";
}

void demo_string_view() {
    std::cout << "\n=== std::string_view ===\n";
    
    // 1. Creating string_view from different sources
    const char* cstr = "C-style string";
    std::string cpp_str = "C++ string";
    
    std::string_view sv1 = cstr;
    std::string_view sv2 = cpp_str;
    
    process_string_view(sv1);
    process_string_view(sv2);
    process_string_view("Literal string");
    
    // 2. Substring without allocation
    std::string_view sv3 = "Hello, World!";
    std::string_view sv4 = sv3.substr(0, 5);  // No memory allocation
    std::cout << "Substring: " << sv4 << "\n";
    
    // 3. Performance comparison
    const std::string longString(1000000, 'x');
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string substr = longString.substr(10, 10);  // Allocates memory
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::string::substr time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() 
              << " ns\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::string_view subview = std::string_view(longString).substr(10, 10);  // No allocation
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::string_view::substr time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() 
              << " ns\n";
    
    // Interview tip: std::string_view provides a non-owning reference to string data,
    // avoiding unnecessary copies and allocations when you only need to read the string
}

// ======================================================================
// Nested Namespaces
// ======================================================================
// Before C++17:
// namespace A {
//     namespace B {
//         namespace C {
//             void func() {}
//         }
//     }
// }

// With C++17:
namespace A::B::C {
    void func() {
        std::cout << "Function in nested namespace A::B::C\n";
    }
}

void demo_nested_namespaces() {
    std::cout << "\n=== Nested Namespaces ===\n";
    A::B::C::func();
    
    // Interview tip: This is a syntactic improvement that makes code more concise
    // and readable when working with deeply nested namespaces
}

// ======================================================================
// __has_include preprocessor expression
// ======================================================================
void demo_has_include() {
    std::cout << "\n=== __has_include Preprocessor Expression ===\n";
    
#if __has_include(<optional>)
    std::cout << "<optional> header is available\n";
#else
    std::cout << "<optional> header is not available\n";
#endif

#if __has_include(<experimental/optional>)
    std::cout << "<experimental/optional> header is available\n";
#else
    std::cout << "<experimental/optional> header is not available\n";
#endif

    // Interview tip: __has_include allows for more portable code by checking
    // for the availability of headers at compile time, useful for feature detection
    // and backward compatibility
}

// ======================================================================
// Main function to demonstrate all features
// ======================================================================
int main() {
    std::cout << "===== C++17 Basic Features =====\n";
    
    demo_structured_bindings();
    demo_if_switch_init();
    demo_inline_variables();
    demo_string_view();
    demo_nested_namespaces();
    demo_has_include();
    
    std::cout << "\nAll basic C++17 features demonstrated successfully!\n";
    return 0;
}

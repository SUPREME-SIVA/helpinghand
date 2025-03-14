#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <type_traits>

// Forward declarations for all demo functions
void demo_relaxed_constexpr();
void demo_variable_templates();
void demo_aggregate_member_init();
void demo_deprecated_attribute();
void demo_sized_deallocation();

// Main menu function
void show_intermediate_features_menu() {
    int choice = 0;
    bool exit_menu = false;
    
    while (!exit_menu) {
        std::cout << "\n=== C++14 Intermediate Features ===\n";
        std::cout << "1. Relaxed constexpr Functions\n";
        std::cout << "2. Variable Templates\n";
        std::cout << "3. Aggregate Member Initialization\n";
        std::cout << "4. [[deprecated]] Attribute\n";
        std::cout << "5. Sized Deallocation\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                demo_relaxed_constexpr();
                break;
            case 2:
                demo_variable_templates();
                break;
            case 3:
                demo_aggregate_member_init();
                break;
            case 4:
                demo_deprecated_attribute();
                break;
            case 5:
                demo_sized_deallocation();
                break;
            case 0:
                exit_menu = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
        if (!exit_menu && choice >= 1 && choice <= 5) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
}

//==============================================================================
// Feature 1: Relaxed constexpr Functions
//==============================================================================
// C++11 constexpr function (very limited)
constexpr int factorial_c11(int n) {
    return n <= 1 ? 1 : (n * factorial_c11(n - 1));
}

// C++14 relaxed constexpr function (with local variables, loops, etc.)
constexpr int factorial_c14(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// C++14 constexpr function with conditionals and multiple returns
constexpr int max_c14(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// C++14 constexpr class with mutable methods
class ConstexprCounter {
private:
    int value;

public:
    constexpr ConstexprCounter(int v) : value(v) {}
    
    constexpr int get() const { return value; }
    
    constexpr int increment() const {
        return value + 1;  // Return incremented value instead of modifying
    }
    
    constexpr int add(int x) const {
        return value + x;  // Return sum instead of modifying
    }
};

void demo_relaxed_constexpr() {
    std::cout << "\n=== Relaxed constexpr Functions ===\n";
    std::cout << "C++14 relaxed the restrictions on constexpr functions, allowing more complex compile-time computations.\n\n";
    
    // Basic examples
    std::cout << "Example 1: Factorial calculation\n";
    std::cout << "constexpr int factorial_c14(int n) {\n";
    std::cout << "    int result = 1;\n";
    std::cout << "    for (int i = 1; i <= n; ++i) {\n";
    std::cout << "        result *= i;\n";
    std::cout << "    }\n";
    std::cout << "    return result;\n";
    std::cout << "}\n\n";
    
    constexpr int fact5 = factorial_c14(5);
    std::cout << "factorial_c14(5) = " << fact5 << " (computed at compile time)\n";
    
    // Comparison with C++11
    std::cout << "\nComparison with C++11:\n";
    std::cout << "C++11 constexpr functions could only contain a single return statement.\n";
    std::cout << "C++14 allows local variables, loops, conditionals, and multiple return statements.\n";
    
    // Constexpr class with mutable methods
    std::cout << "\nExample 2: Constexpr class with mutable methods\n";
    std::cout << "class ConstexprCounter {\n";
    std::cout << "    int value;\n";
    std::cout << "public:\n";
    std::cout << "    constexpr ConstexprCounter(int v) : value(v) {}\n";
    std::cout << "    constexpr int get() const { return value; }\n";
    std::cout << "    constexpr int increment() const { return value + 1; }\n";
    std::cout << "    constexpr int add(int x) const { return value + x; }\n";
    std::cout << "};\n\n";
    
    constexpr ConstexprCounter counter(10);
    constexpr int val1 = counter.get();  // Compile-time evaluation
    
    // This would be a compile-time error in C++11
    constexpr ConstexprCounter counter2(5);
    constexpr int val2 = counter2.increment();  // Mutable operation at compile time
    constexpr int val3 = counter2.add(10);      // Another mutable operation
    
    std::cout << "counter.get() = " << val1 << "\n";
    std::cout << "counter2.increment() = " << val2 << "\n";
    std::cout << "counter2.add(10) = " << val3 << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What are the key differences between constexpr in C++11 and C++14?\n";
    std::cout << "A1: In C++11, constexpr functions were limited to a single return statement and\n";
    std::cout << "    could not contain local variables, loops, conditionals, or multiple returns.\n";
    std::cout << "    C++14 relaxed these restrictions, allowing:\n";
    std::cout << "    - Local variable declarations\n";
    std::cout << "    - Multiple return statements\n";
    std::cout << "    - Loops (for, while, do-while)\n";
    std::cout << "    - Conditionals (if, switch)\n";
    std::cout << "    - Mutation of objects declared within the function\n";
    
    std::cout << "\nQ2: Can constexpr functions in C++14 have side effects?\n";
    std::cout << "A2: constexpr functions in C++14 can modify objects declared within the function,\n";
    std::cout << "    but they still cannot have external side effects. When used in a constant\n";
    std::cout << "    expression context, they must be evaluable at compile time, which prohibits\n";
    std::cout << "    external side effects. However, the same function can be used at runtime,\n";
    std::cout << "    where it can have side effects like any normal function.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use constexpr for functions that can benefit from compile-time evaluation.\n";
    std::cout << "2. Keep constexpr functions reasonably simple to ensure they can be evaluated at compile time.\n";
    std::cout << "3. Remember that constexpr is part of the interface, not just an implementation detail.\n";
    std::cout << "4. Use constexpr for performance-critical code that can be pre-computed.\n";
}

//==============================================================================
// Feature 2: Variable Templates
//==============================================================================
// Basic variable template
template<typename T>
constexpr T pi = T(3.1415926535897932385);

// Variable template with non-type parameter
template<int N>
constexpr double power_of_two = 1.0 * (1 << N);

// Variable template specialization
template<>
constexpr int pi<int> = 3;

// More complex variable template
template<typename T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;

void demo_variable_templates() {
    std::cout << "\n=== Variable Templates ===\n";
    std::cout << "C++14 introduced variable templates, allowing template parameters for variables.\n\n";
    
    // Basic examples
    std::cout << "Example 1: Template for pi with different types\n";
    std::cout << "template<typename T>\n";
    std::cout << "constexpr T pi = T(3.1415926535897932385);\n\n";
    
    std::cout << "pi<float> = " << pi<float> << "\n";
    std::cout << "pi<double> = " << pi<double> << "\n";
    std::cout << "pi<int> = " << pi<int> << " (specialized to exactly 3)\n";
    
    // Variable template with non-type parameter
    std::cout << "\nExample 2: Variable template with non-type parameter\n";
    std::cout << "template<int N>\n";
    std::cout << "constexpr double power_of_two = 1.0 * (1 << N);\n\n";
    
    std::cout << "power_of_two<0> = " << power_of_two<0> << "\n";
    std::cout << "power_of_two<1> = " << power_of_two<1> << "\n";
    std::cout << "power_of_two<8> = " << power_of_two<8> << "\n";
    
    // Type traits example
    std::cout << "\nExample 3: Type traits with variable templates\n";
    std::cout << "template<typename T>\n";
    std::cout << "constexpr bool is_pointer_v = std::is_pointer<T>::value;\n\n";
    
    std::cout << "is_pointer_v<int> = " << is_pointer_v<int> << "\n";
    std::cout << "is_pointer_v<int*> = " << is_pointer_v<int*> << "\n";
    std::cout << "is_pointer_v<std::string> = " << is_pointer_v<std::string> << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What are the advantages of variable templates over constexpr functions?\n";
    std::cout << "A1: Variable templates offer several advantages:\n";
    std::cout << "    1. More concise syntax for template-dependent constants\n";
    std::cout << "    2. Can be specialized for specific types\n";
    std::cout << "    3. Can be partially specialized (for class templates)\n";
    std::cout << "    4. Provide a cleaner way to create type-dependent constants\n";
    std::cout << "    5. Work well as shortcuts for template metaprogramming traits\n";
    
    std::cout << "\nQ2: How are variable templates used in the C++ standard library?\n";
    std::cout << "A2: In C++17 and later, variable templates are extensively used in the standard library,\n";
    std::cout << "    particularly for type traits. For example, instead of std::is_pointer<T>::value,\n";
    std::cout << "    you can use std::is_pointer_v<T>. This pattern is used for many type traits.\n";
    std::cout << "    While this was introduced in C++17, the concept originated in C++14.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use variable templates for type-dependent constants that need to be specialized.\n";
    std::cout << "2. Consider variable templates as shortcuts for frequently used template expressions.\n";
    std::cout << "3. Combine with constexpr for compile-time evaluation when possible.\n";
    std::cout << "4. Use naming conventions like _v suffix for value-based templates (similar to std library).\n";
}

//==============================================================================
// Feature 3: Aggregate Member Initialization
//==============================================================================
// C++11 style aggregate
struct PointC11 {
    int x;
    int y;
    // No in-class initializers in C++11
};

// C++14 style aggregate with member initializers
struct PointC14 {
    int x = 0;  // Default member initializer
    int y = 0;  // Default member initializer
};

// More complex example
struct Rectangle {
    int width = 1;
    int height = 1;
    std::string name = "Rectangle";
    bool filled = false;
    
    // Still an aggregate type despite having initializers
};

void demo_aggregate_member_init() {
    std::cout << "\n=== Aggregate Member Initialization ===\n";
    std::cout << "C++14 allows default member initializers in aggregate classes.\n\n";
    
    // Basic examples
    std::cout << "Example 1: Aggregate with default member initializers\n";
    std::cout << "struct PointC14 {\n";
    std::cout << "    int x = 0;  // Default member initializer\n";
    std::cout << "    int y = 0;  // Default member initializer\n";
    std::cout << "};\n\n";
    
    // Default initialization
    PointC14 p1;  // Uses default initializers: x=0, y=0
    std::cout << "Default initialization: PointC14 p1;\n";
    std::cout << "p1.x = " << p1.x << ", p1.y = " << p1.y << "\n";
    
    // Aggregate initialization (overrides defaults)
    PointC14 p2 = {10, 20};  // Overrides defaults: x=10, y=20
    std::cout << "\nAggregate initialization: PointC14 p2 = {10, 20};\n";
    std::cout << "p2.x = " << p2.x << ", p2.y = " << p2.y << "\n";
    
    // Partial initialization
    PointC14 p3 = {5};  // Partial override: x=5, y=0 (default)
    std::cout << "\nPartial initialization: PointC14 p3 = {5};\n";
    std::cout << "p3.x = " << p3.x << ", p3.y = " << p3.y << "\n";
    
    // More complex example
    std::cout << "\nExample 2: More complex aggregate\n";
    std::cout << "struct Rectangle {\n";
    std::cout << "    int width = 1;\n";
    std::cout << "    int height = 1;\n";
    std::cout << "    std::string name = \"Rectangle\";\n";
    std::cout << "    bool filled = false;\n";
    std::cout << "};\n\n";
    
    Rectangle r1;  // All default initializers
    std::cout << "Default initialization: Rectangle r1;\n";
    std::cout << "r1: width=" << r1.width << ", height=" << r1.height 
              << ", name=\"" << r1.name << "\", filled=" << std::boolalpha << r1.filled << "\n";
    
    Rectangle r2 = {10, 20, "Square", true};  // Override all defaults
    std::cout << "\nFull initialization: Rectangle r2 = {10, 20, \"Square\", true};\n";
    std::cout << "r2: width=" << r2.width << ", height=" << r2.height 
              << ", name=\"" << r2.name << "\", filled=" << r2.filled << "\n";
    
    Rectangle r3 = {5, 5};  // Partial override
    std::cout << "\nPartial initialization: Rectangle r3 = {5, 5};\n";
    std::cout << "r3: width=" << r3.width << ", height=" << r3.height 
              << ", name=\"" << r3.name << "\", filled=" << r3.filled << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: Does adding default member initializers change whether a class is an aggregate?\n";
    std::cout << "A1: In C++14, adding default member initializers does NOT change whether a class\n";
    std::cout << "    is an aggregate. A class can have default member initializers and still be an\n";
    std::cout << "    aggregate, allowing both default values and aggregate initialization.\n";
    
    std::cout << "\nQ2: How do default member initializers interact with aggregate initialization?\n";
    std::cout << "A2: When using aggregate initialization with braces {}, the explicitly provided values\n";
    std::cout << "    override the default member initializers. If fewer initializers are provided than\n";
    std::cout << "    there are members, the remaining members use their default initializers.\n";
    std::cout << "    This allows for flexible initialization where only some values need to be specified.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use default member initializers for values that are commonly used defaults.\n";
    std::cout << "2. Be aware that default member initializers are used in all constructors that don't explicitly initialize the member.\n";
    std::cout << "3. Remember that aggregate initialization still works and overrides the defaults.\n";
    std::cout << "4. Consider using designated initializers (C++20) for even clearer initialization of aggregates.\n";
}

//==============================================================================
// Feature 4: [[deprecated]] Attribute
//==============================================================================
// Deprecated function without reason
[[deprecated]]
void old_function() {
    std::cout << "This function is deprecated.\n";
}

// Deprecated function with reason
[[deprecated("Use new_function() instead")]]
void legacy_function() {
    std::cout << "This function is deprecated with a reason.\n";
}

// Non-deprecated replacement function
void new_function() {
    std::cout << "This is the new recommended function.\n";
}

// Deprecated enum
enum [[deprecated]] OldColors {
    RED, GREEN, BLUE
};

// Deprecated class
class [[deprecated("Use ModernClass instead")]] LegacyClass {
public:
    void method() {
        std::cout << "Method in deprecated class.\n";
    }
};

// Modern replacement class
class ModernClass {
public:
    void method() {
        std::cout << "Method in modern class.\n";
    }
};

void demo_deprecated_attribute() {
    std::cout << "\n=== [[deprecated]] Attribute ===\n";
    std::cout << "C++14 introduced the [[deprecated]] attribute to mark code as deprecated.\n\n";
    
    std::cout << "Example 1: Basic deprecated function\n";
    std::cout << "[[deprecated]]\n";
    std::cout << "void old_function() { ... }\n\n";
    
    std::cout << "Example 2: Deprecated function with reason\n";
    std::cout << "[[deprecated(\"Use new_function() instead\")]]\n";
    std::cout << "void legacy_function() { ... }\n\n";
    
    std::cout << "Example 3: Deprecated class\n";
    std::cout << "class [[deprecated(\"Use ModernClass instead\")]] LegacyClass { ... };\n\n";
    
    // Calling the functions (would generate compiler warnings)
    std::cout << "Calling deprecated functions (would generate compiler warnings):\n";
    old_function();
    legacy_function();
    new_function();
    
    // Using deprecated class (would generate compiler warnings)
    std::cout << "\nUsing deprecated class (would generate compiler warnings):\n";
    LegacyClass legacy;
    legacy.method();
    
    ModernClass modern;
    modern.method();
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What is the purpose of the [[deprecated]] attribute?\n";
    std::cout << "A1: The [[deprecated]] attribute is used to indicate that a function, class, enum,\n";
    std::cout << "    or other entity is deprecated and should not be used in new code. When code uses\n";
    std::cout << "    a deprecated entity, the compiler generates a warning, helping developers identify\n";
    std::cout << "    and update code that uses outdated APIs. This facilitates API evolution while\n";
    std::cout << "    maintaining backward compatibility.\n";
    
    std::cout << "\nQ2: How does [[deprecated]] differ from other ways of marking code as deprecated?\n";
    std::cout << "A2: Before C++14, developers used various non-standard approaches to mark code as\n";
    std::cout << "    deprecated, such as compiler-specific attributes or macros (e.g., __attribute__((deprecated))\n";
    std::cout << "    in GCC or __declspec(deprecated) in MSVC). The [[deprecated]] attribute provides a\n";
    std::cout << "    standard, portable way to mark code as deprecated across all C++14-compliant compilers.\n";
    std::cout << "    It also allows providing a reason string explaining why the code is deprecated and\n";
    std::cout << "    suggesting alternatives.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Always provide a reason string with [[deprecated]] explaining why it's deprecated and what to use instead.\n";
    std::cout << "2. Don't remove deprecated code immediately; give users time to migrate to new APIs.\n";
    std::cout << "3. Consider a deprecation policy with clear timelines for when deprecated code will be removed.\n";
    std::cout << "4. Use [[deprecated]] as part of a versioning strategy for your APIs.\n";
}

//==============================================================================
// Feature 5: Sized Deallocation
//==============================================================================
// Custom class with overloaded new and delete
class SizedDeallocExample {
private:
    int data[1000];  // Some data

public:
    SizedDeallocExample() {
        std::cout << "SizedDeallocExample constructed\n";
    }
    
    ~SizedDeallocExample() {
        std::cout << "SizedDeallocExample destructed\n";
    }
    
    // Overloaded new operator
    static void* operator new(std::size_t size) {
        std::cout << "Custom new called with size: " << size << " bytes\n";
        return ::operator new(size);
    }
    
    // Overloaded delete operator (C++14 sized version)
    static void operator delete(void* ptr, std::size_t size) {
        std::cout << "Custom delete called with size: " << size << " bytes\n";
        ::operator delete(ptr);
    }
    
    // C++11 style delete operator (still required for some cases)
    static void operator delete(void* ptr) {
        std::cout << "Custom delete called (unsized version)\n";
        ::operator delete(ptr);
    }
};

void demo_sized_deallocation() {
    std::cout << "\n=== Sized Deallocation ===\n";
    std::cout << "C++14 added support for sized deallocation, allowing delete to know the size of the memory being freed.\n\n";
    
    std::cout << "Example: Custom class with sized deallocation\n";
    std::cout << "class SizedDeallocExample {\n";
    std::cout << "    // ...\n";
    std::cout << "    static void* operator new(std::size_t size) { ... }\n";
    std::cout << "    static void operator delete(void* ptr, std::size_t size) { ... }\n";
    std::cout << "    static void operator delete(void* ptr) { ... }\n";
    std::cout << "};\n\n";
    
    // Create and delete an object to demonstrate sized deallocation
    std::cout << "Creating and deleting an object:\n";
    SizedDeallocExample* obj = new SizedDeallocExample();
    delete obj;  // Should call the sized version if compiler supports it
    
    std::cout << "\nExplanation:\n";
    std::cout << "1. In C++11, when delete was called, the memory allocator didn't know the size of the\n";
    std::cout << "   memory block being freed, which could lead to inefficiencies.\n";
    std::cout << "2. C++14 added a new overload of operator delete that includes the size parameter.\n";
    std::cout << "3. When an object is deleted, if the sized version of operator delete is available,\n";
    std::cout << "   the compiler will pass the size of the object to the delete operator.\n";
    std::cout << "4. This can improve performance in custom memory managers by avoiding the need to\n";
    std::cout << "   look up the size of the memory block being freed.\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What problem does sized deallocation solve?\n";
    std::cout << "A1: Sized deallocation solves a performance problem in memory management. Without sized\n";
    std::cout << "    deallocation, when an object is deleted, the memory allocator often needs to look up\n";
    std::cout << "    the size of the allocated block in its internal data structures. This lookup can be\n";
    std::cout << "    expensive. With sized deallocation, the compiler already knows the size of the object\n";
    std::cout << "    being deleted and can pass this information to the deallocator, potentially improving\n";
    std::cout << "    performance, especially in custom memory management systems.\n";
    
    std::cout << "\nQ2: Do you need to implement both sized and unsized versions of operator delete?\n";
    std::cout << "A2: Yes, it's generally recommended to implement both versions. The sized version\n";
    std::cout << "    (operator delete(void*, std::size_t)) will be called when the compiler knows the size,\n";
    std::cout << "    but the unsized version (operator delete(void*)) is still needed for cases where the\n";
    std::cout << "    size is not known or for compatibility with code compiled with older compilers.\n";
    std::cout << "    Additionally, placement delete and other special forms of deletion might still use\n";
    std::cout << "    the unsized version.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Implement both sized and unsized versions of operator delete for custom allocators.\n";
    std::cout << "2. Consider using sized deallocation in performance-critical custom memory managers.\n";
    std::cout << "3. Be aware that not all compilers may use the sized version, even if it's available.\n";
    std::cout << "4. Remember that sized deallocation is most beneficial for custom memory allocators.\n";
}

// This function will be called from the main cpp14_features.cpp file
void run_intermediate_features() {
    show_intermediate_features_menu();
}

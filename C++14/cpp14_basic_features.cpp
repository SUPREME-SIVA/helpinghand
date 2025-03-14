#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

// Forward declarations for all demo functions
void demo_binary_literals();
void demo_digit_separators();
void demo_return_type_deduction();
void demo_generic_lambdas();
void demo_capture_initializers();

// Main menu function
void show_basic_features_menu() {
    int choice = 0;
    bool exit_menu = false;
    
    while (!exit_menu) {
        std::cout << "\n=== C++14 Basic Features ===\n";
        std::cout << "1. Binary Literals\n";
        std::cout << "2. Digit Separators\n";
        std::cout << "3. Return Type Deduction\n";
        std::cout << "4. Generic Lambdas\n";
        std::cout << "5. Capture Initializers\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                demo_binary_literals();
                break;
            case 2:
                demo_digit_separators();
                break;
            case 3:
                demo_return_type_deduction();
                break;
            case 4:
                demo_generic_lambdas();
                break;
            case 5:
                demo_capture_initializers();
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
// Feature 1: Binary Literals
//==============================================================================
void demo_binary_literals() {
    std::cout << "\n=== Binary Literals ===\n";
    std::cout << "C++14 introduced a new way to represent binary literals using the 0b or 0B prefix.\n\n";
    
    // Binary literals examples
    int binary1 = 0b1010;  // Binary representation of 10
    int binary2 = 0B1100;  // Binary representation of 12
    
    std::cout << "Example 1: int binary1 = 0b1010; // Value: " << binary1 << "\n";
    std::cout << "Example 2: int binary2 = 0B1100; // Value: " << binary2 << "\n";
    
    // Practical example: Bit manipulation
    unsigned char flags = 0b00101010;  // Set specific bits
    std::cout << "Example 3: unsigned char flags = 0b00101010; // Value: " << static_cast<int>(flags) << "\n";
    
    // Bit operations
    std::cout << "\nBit Operations:\n";
    std::cout << "Original flags: " << static_cast<int>(flags) << " (0b00101010)\n";
    
    // Set a bit (using OR)
    unsigned char new_flags = flags | 0b00010000;
    std::cout << "After setting bit 4: " << static_cast<int>(new_flags) << " (0b00111010)\n";
    
    // Clear a bit (using AND with complement)
    new_flags = new_flags & ~0b00100000;
    std::cout << "After clearing bit 5: " << static_cast<int>(new_flags) << " (0b00011010)\n";
    
    // Toggle a bit (using XOR)
    new_flags = new_flags ^ 0b00000010;
    std::cout << "After toggling bit 1: " << static_cast<int>(new_flags) << " (0b00011000)\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What is the advantage of using binary literals over hexadecimal or decimal literals?\n";
    std::cout << "A1: Binary literals make bit manipulation code more readable and less error-prone.\n";
    std::cout << "    When working with individual bits, binary representation makes it clear which\n";
    std::cout << "    bits are being set, cleared, or toggled.\n";
    
    std::cout << "\nQ2: Can binary literals be used with all integer types in C++14?\n";
    std::cout << "A2: Yes, binary literals can be used with all integer types (int, long, unsigned, etc.)\n";
    std::cout << "    and can be combined with other C++ integer literal suffixes like 'u', 'l', 'll', etc.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use binary literals when working with bit flags or bit manipulation for clarity.\n";
    std::cout << "2. Consider using named constants for bit flags to improve code readability.\n";
    std::cout << "3. For larger binary numbers, use digit separators (another C++14 feature) to improve readability.\n";
}

//==============================================================================
// Feature 2: Digit Separators
//==============================================================================
void demo_digit_separators() {
    std::cout << "\n=== Digit Separators ===\n";
    std::cout << "C++14 introduced digit separators using single quotes (') to improve readability of numeric literals.\n\n";
    
    // Digit separator examples
    int decimal = 1'000'000;  // One million
    int binary = 0b0001'0000'0000;  // 256 in binary
    int hex = 0xFF'FF'FF'FF;  // Max 32-bit unsigned int
    double pi = 3.141'592'653'589'793;
    
    std::cout << "Example 1: int decimal = 1'000'000; // Value: " << decimal << "\n";
    std::cout << "Example 2: int binary = 0b0001'0000'0000; // Value: " << binary << "\n";
    std::cout << "Example 3: int hex = 0xFF'FF'FF'FF; // Value: " << hex << "\n";
    std::cout << "Example 4: double pi = 3.141'592'653'589'793; // Value: " << pi << "\n";
    
    // Practical example: Financial calculations
    double amount = 1'234'567.89;
    double interest_rate = 0.034'25;  // 3.425%
    double interest = amount * interest_rate;
    
    std::cout << "\nFinancial Calculation Example:\n";
    std::cout << "Principal amount: $" << amount << "\n";
    std::cout << "Interest rate: " << interest_rate * 100 << "%\n";
    std::cout << "Annual interest: $" << interest << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: Where can digit separators be placed in numeric literals?\n";
    std::cout << "A1: Digit separators can be placed between any two digits in numeric literals,\n";
    std::cout << "    including decimal, hexadecimal, binary, and floating-point literals.\n";
    std::cout << "    However, they cannot be placed at the beginning or end of a number,\n";
    std::cout << "    adjacent to the decimal point, or adjacent to the prefix (0x, 0b).\n";
    
    std::cout << "\nQ2: Do digit separators affect the value of the number?\n";
    std::cout << "A2: No, digit separators are ignored by the compiler and do not affect the value\n";
    std::cout << "    of the number. They are solely for improving human readability.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use digit separators for large numbers to improve readability.\n";
    std::cout << "2. Consider grouping digits in a way that makes sense for the domain (e.g., thousands for decimal,\n";
    std::cout << "   bytes for hexadecimal, nibbles for binary).\n";
    std::cout << "3. Be consistent with your use of digit separators throughout your codebase.\n";
}

//==============================================================================
// Feature 3: Return Type Deduction
//==============================================================================
// C++11 style: explicit return type
template<typename T>
auto multiply_c11(T a, T b) -> decltype(a * b) {
    return a * b;
}

// C++14 style: return type deduction
template<typename T>
auto multiply_c14(T a, T b) {
    return a * b;
}

// Function that returns different types based on condition
auto get_value(bool condition) {
    if (condition) {
        return 42;  // Returns int
    } else {
        return 42;  // Returns int
    }
    // This would be a compile error in C++11 without explicit return type
}

void demo_return_type_deduction() {
    std::cout << "\n=== Return Type Deduction ===\n";
    std::cout << "C++14 allows auto return type deduction for functions, simplifying code.\n\n";
    
    // Basic examples
    std::cout << "Example 1: auto multiply_c14(T a, T b) { return a * b; }\n";
    std::cout << "Result with ints: multiply_c14(5, 3) = " << multiply_c14(5, 3) << "\n";
    std::cout << "Result with doubles: multiply_c14(2.5, 3.0) = " << multiply_c14(2.5, 3.0) << "\n";
    
    // Comparison with C++11
    std::cout << "\nComparison with C++11:\n";
    std::cout << "C++11: auto multiply_c11(T a, T b) -> decltype(a * b) { return a * b; }\n";
    std::cout << "C++14: auto multiply_c14(T a, T b) { return a * b; }\n";
    
    // Conditional return types
    std::cout << "\nConditional Return Types:\n";
    std::cout << "auto get_value(bool condition) {\n";
    std::cout << "    if (condition) return 42; // int\n";
    std::cout << "    else return 42; // int\n";
    std::cout << "}\n";
    
    std::cout << "get_value(true) = " << get_value(true) << " (type: int)\n";
    std::cout << "get_value(false) = " << get_value(false) << " (type: int)\n";
    
    // Limitations
    std::cout << "\nLimitations:\n";
    std::cout << "1. All return statements must deduce to the same type, or be convertible to a common type.\n";
    std::cout << "2. The function must be fully defined before it's called (not just declared).\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What's the difference between auto return type deduction in C++14 and decltype(auto)?\n";
    std::cout << "A1: auto strips references and cv-qualifiers, while decltype(auto) preserves them.\n";
    std::cout << "    For example, if a function returns a reference, auto would return by value,\n";
    std::cout << "    while decltype(auto) would return by reference.\n";
    
    std::cout << "\nQ2: What are the limitations of return type deduction in C++14?\n";
    std::cout << "A2: The main limitations are:\n";
    std::cout << "    - All return statements must deduce to the same type or be convertible to a common type.\n";
    std::cout << "    - The function must be fully defined before it's called, not just declared.\n";
    std::cout << "    - Recursive functions need explicit return types unless the recursion is in a lambda.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use return type deduction to simplify code when the return type is obvious.\n";
    std::cout << "2. Avoid using return type deduction when the return type is complex or non-obvious.\n";
    std::cout << "3. Be careful with functions that have multiple return statements to ensure type consistency.\n";
    std::cout << "4. Consider using decltype(auto) when you need to preserve references and cv-qualifiers.\n";
}

//==============================================================================
// Feature 4: Generic Lambdas
//==============================================================================
void demo_generic_lambdas() {
    std::cout << "\n=== Generic Lambdas ===\n";
    std::cout << "C++14 introduced generic lambdas with auto parameters, allowing type-agnostic lambda expressions.\n\n";
    
    // C++11 style: Type-specific lambda
    auto add_c11 = [](int a, int b) { return a + b; };
    
    // C++14 style: Generic lambda with auto parameters
    auto add_c14 = [](auto a, auto b) { return a + b; };
    
    std::cout << "Example 1: auto add_c14 = [](auto a, auto b) { return a + b; }\n";
    std::cout << "Result with ints: add_c14(5, 3) = " << add_c14(5, 3) << "\n";
    std::cout << "Result with doubles: add_c14(2.5, 3.0) = " << add_c14(2.5, 3.0) << "\n";
    std::cout << "Result with mixed types: add_c14(10, 3.5) = " << add_c14(10, 3.5) << "\n";
    
    // Generic lambda with multiple auto parameters of different types
    auto process = [](auto container, auto func) {
        for (auto& item : container) {
            func(item);
        }
    };
    
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "\nExample 2: Processing a container with a generic lambda\n";
    std::cout << "auto process = [](auto container, auto func) { ... }\n";
    std::cout << "Original vector: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";
    
    // Using the generic lambda to process the container
    process(numbers, [](auto& n) { n *= 2; });
    
    std::cout << "After doubling: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";
    
    // Generic lambda with perfect forwarding
    auto forward_to_func = [](auto&& func, auto&&... args) {
        return func(std::forward<decltype(args)>(args)...);
    };
    
    std::cout << "\nExample 3: Generic lambda with perfect forwarding\n";
    std::cout << "auto forward_to_func = [](auto&& func, auto&&... args) { return func(std::forward<decltype(args)>(args)...); }\n";
    std::cout << "Result: forward_to_func(add_c14, 10, 20) = " << forward_to_func(add_c14, 10, 20) << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: How do generic lambdas in C++14 differ from templates?\n";
    std::cout << "A1: Generic lambdas are essentially a shorthand for creating a function object with a\n";
    std::cout << "    templated operator() method. Under the hood, the compiler generates a template\n";
    std::cout << "    for each auto parameter. The main difference is syntax simplicity and the fact that\n";
    std::cout << "    lambdas can capture variables from their surrounding scope.\n";
    
    std::cout << "\nQ2: Can generic lambdas in C++14 use perfect forwarding?\n";
    std::cout << "A2: Yes, generic lambdas can use perfect forwarding with the pattern:\n";
    std::cout << "    [](auto&& param) { return func(std::forward<decltype(param)>(param)); }\n";
    std::cout << "    This allows preserving value categories (lvalue/rvalue) of the arguments.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use generic lambdas when you need the same operation on different types.\n";
    std::cout << "2. Consider using decltype and std::forward for perfect forwarding in generic lambdas.\n";
    std::cout << "3. Be aware that auto parameters might lead to unintended type conversions.\n";
    std::cout << "4. Use generic lambdas for algorithm customization points to make code more flexible.\n";
}

//==============================================================================
// Feature 5: Capture Initializers
//==============================================================================
void demo_capture_initializers() {
    std::cout << "\n=== Capture Initializers ===\n";
    std::cout << "C++14 allows creating variables in lambda captures that don't exist in the enclosing scope.\n\n";
    
    int x = 10;
    
    // C++11 style: can only capture existing variables
    auto lambda_c11 = [x]() { return x * 2; };
    
    // C++14 style: can create new variables in the capture
    auto lambda_c14 = [y = x + 5]() { return y * 2; };
    
    std::cout << "Example 1: auto lambda_c14 = [y = x + 5]() { return y * 2; }\n";
    std::cout << "Result: lambda_c14() = " << lambda_c14() << "\n";
    
    // Move-only objects in captures
    auto unique_ptr_lambda = [ptr = std::make_unique<int>(42)]() {
        return *ptr;
    };
    
    std::cout << "\nExample 2: Capturing move-only objects\n";
    std::cout << "auto unique_ptr_lambda = [ptr = std::make_unique<int>(42)]() { return *ptr; }\n";
    std::cout << "Result: unique_ptr_lambda() = " << unique_ptr_lambda() << "\n";
    
    // Capturing the result of an expression
    auto compute_lambda = [value = x * x + 20]() {
        return value;
    };
    
    std::cout << "\nExample 3: Capturing the result of an expression\n";
    std::cout << "auto compute_lambda = [value = x * x + 20]() { return value; }\n";
    std::cout << "Result: compute_lambda() = " << compute_lambda() << "\n";
    
    // Mutable lambda with initialized capture
    auto counter_lambda = [count = 0]() mutable {
        return ++count;
    };
    
    std::cout << "\nExample 4: Mutable lambda with initialized capture\n";
    std::cout << "auto counter_lambda = [count = 0]() mutable { return ++count; }\n";
    std::cout << "First call: counter_lambda() = " << counter_lambda() << "\n";
    std::cout << "Second call: counter_lambda() = " << counter_lambda() << "\n";
    std::cout << "Third call: counter_lambda() = " << counter_lambda() << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What problem do capture initializers solve in C++14?\n";
    std::cout << "A1: Capture initializers solve several problems:\n";
    std::cout << "    1. They allow capturing variables that don't exist in the enclosing scope.\n";
    std::cout << "    2. They enable capturing move-only types like std::unique_ptr.\n";
    std::cout << "    3. They allow transforming variables during capture.\n";
    std::cout << "    4. They provide a way to initialize counters or state variables within the lambda.\n";
    
    std::cout << "\nQ2: How do capture initializers interact with move semantics?\n";
    std::cout << "A2: Capture initializers enable moving objects into lambda captures, which was not\n";
    std::cout << "    possible in C++11. This is particularly useful for move-only types like\n";
    std::cout << "    std::unique_ptr or std::future. The syntax [ptr = std::move(original_ptr)]()\n";
    std::cout << "    moves the resource ownership into the lambda.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use capture initializers to simplify lambdas that need computed values.\n";
    std::cout << "2. Leverage capture initializers for move-only types to transfer ownership to lambdas.\n";
    std::cout << "3. Use the mutable keyword with state variables in capture initializers when the state needs to change.\n";
    std::cout << "4. Prefer capture initializers over complex captures when transforming variables during capture.\n";
}

// This function will be called from the main cpp14_features.cpp file
void run_basic_features() {
    show_basic_features_menu();
}

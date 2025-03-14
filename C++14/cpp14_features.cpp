#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <map>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <random>
#include <regex>
#include <thread>
#include <complex>  // Added for std::complex

// Forward declarations for feature demo functions
// These are implemented in the respective files
void run_basic_features();
void run_intermediate_features();

// Forward declarations for advanced features
void demo_shared_timed_mutex();
void demo_heterogeneous_lookup();
void demo_standard_user_defined_literals();
void demo_improved_type_traits();
void demo_tuple_addressing_by_type();

// Forward declaration for advanced features menu
void show_advanced_features_menu();

// Main menu function
void show_main_menu() {
    int choice = 0;
    bool exit_program = false;
    
    while (!exit_program) {
        std::cout << "\n=== C++14 Features Guide ===\n";
        std::cout << "1. Basic Features\n";
        std::cout << "2. Intermediate Features\n";
        std::cout << "3. Advanced Features\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                run_basic_features();
                break;
            case 2:
                run_intermediate_features();
                break;
            case 3:
                show_advanced_features_menu();
                break;
            case 0:
                exit_program = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Advanced features menu
void show_advanced_features_menu() {
    int choice = 0;
    bool exit_menu = false;
    
    while (!exit_menu) {
        std::cout << "\n=== C++14 Advanced Features ===\n";
        std::cout << "1. Shared Timed Mutex\n";
        std::cout << "2. Heterogeneous Lookup\n";
        std::cout << "3. Standard User-Defined Literals\n";
        std::cout << "4. Improved Type Traits\n";
        std::cout << "5. Tuple Addressing by Type\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                demo_shared_timed_mutex();
                break;
            case 2:
                demo_heterogeneous_lookup();
                break;
            case 3:
                demo_standard_user_defined_literals();
                break;
            case 4:
                demo_improved_type_traits();
                break;
            case 5:
                demo_tuple_addressing_by_type();
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
// Feature 1: Shared Timed Mutex
//==============================================================================
class ThreadSafeCounter {
private:
    mutable std::shared_timed_mutex mutex_;
    int value_ = 0;

public:
    // Multiple threads can read the counter's value at the same time.
    int get() const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        return value_;
    }
    
    // Only one thread can increment or reset the counter at a time.
    void increment() {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        ++value_;
    }
    
    // Only one thread can increment or reset the counter at a time.
    void reset() {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        value_ = 0;
    }
    
    // Try to acquire lock with timeout
    bool try_increment_for(const std::chrono::milliseconds& timeout) {
        if (mutex_.try_lock_for(timeout)) {
            ++value_;
            mutex_.unlock();
            return true;
        }
        return false;
    }
};

void demo_shared_timed_mutex() {
    std::cout << "\n=== Shared Timed Mutex ===\n";
    std::cout << "C++14 introduced std::shared_timed_mutex, a reader-writer lock with timeout capabilities.\n\n";
    
    ThreadSafeCounter counter;
    
    std::cout << "Example: Thread-safe counter with shared_timed_mutex\n";
    std::cout << "class ThreadSafeCounter {\n";
    std::cout << "private:\n";
    std::cout << "    mutable std::shared_timed_mutex mutex_;\n";
    std::cout << "    int value_ = 0;\n";
    std::cout << "public:\n";
    std::cout << "    int get() const {\n";
    std::cout << "        std::shared_lock<std::shared_timed_mutex> lock(mutex_);\n";
    std::cout << "        return value_;\n";
    std::cout << "    }\n";
    std::cout << "    void increment() {\n";
    std::cout << "        std::unique_lock<std::shared_timed_mutex> lock(mutex_);\n";
    std::cout << "        ++value_;\n";
    std::cout << "    }\n";
    std::cout << "    // ... other methods\n";
    std::cout << "};\n\n";
    
    // Demonstrate basic usage
    std::cout << "Initial counter value: " << counter.get() << "\n";
    counter.increment();
    counter.increment();
    std::cout << "After two increments: " << counter.get() << "\n";
    
    // Demonstrate timeout functionality
    std::cout << "\nDemonstrating timeout functionality:\n";
    bool success = counter.try_increment_for(std::chrono::milliseconds(100));
    std::cout << "Increment with 100ms timeout: " << (success ? "succeeded" : "failed") << "\n";
    std::cout << "Counter value: " << counter.get() << "\n";
    
    std::cout << "\nExplanation:\n";
    std::cout << "1. std::shared_timed_mutex allows multiple readers (shared locks) or a single writer (unique lock).\n";
    std::cout << "2. It extends the functionality of std::shared_mutex by adding timeout capabilities.\n";
    std::cout << "3. The try_lock_for() and try_lock_until() methods allow attempts to acquire the lock with a timeout.\n";
    std::cout << "4. This is useful in scenarios where you want to avoid indefinite blocking.\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What's the difference between std::mutex, std::shared_mutex, and std::shared_timed_mutex?\n";
    std::cout << "A1: - std::mutex: Allows only exclusive (write) access, one thread at a time.\n";
    std::cout << "    - std::shared_mutex: Allows multiple readers or a single writer (C++17).\n";
    std::cout << "    - std::shared_timed_mutex: Same as shared_mutex but with timeout capabilities (C++14).\n";
    
    std::cout << "\nQ2: When would you use std::shared_timed_mutex over std::mutex?\n";
    std::cout << "A2: Use std::shared_timed_mutex when:\n";
    std::cout << "    1. You have a resource that's read frequently but written to infrequently.\n";
    std::cout << "    2. You want to allow multiple threads to read simultaneously.\n";
    std::cout << "    3. You need timeout capabilities to avoid deadlocks or long waits.\n";
    std::cout << "    Examples include caches, configuration stores, or any read-heavy data structures.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use std::shared_lock for read operations and std::unique_lock for write operations.\n";
    std::cout << "2. Consider the performance impact of reader-writer locks, which are more complex than simple mutexes.\n";
    std::cout << "3. Use timeouts judiciously to prevent deadlocks or resource starvation.\n";
    std::cout << "4. For C++17 and later, consider using std::shared_mutex if you don't need timeout capabilities.\n";
}

//==============================================================================
// Feature 2: Heterogeneous Lookup
//==============================================================================
// Custom comparator that allows heterogeneous lookup
struct StringCompare {
    using is_transparent = std::true_type;  // Enables heterogeneous lookup
    
    bool operator()(const std::string& left, const std::string& right) const {
        return left < right;
    }
    
    // Overloads for heterogeneous lookup
    bool operator()(const std::string& left, const char* right) const {
        return left < right;
    }
    
    bool operator()(const char* left, const std::string& right) const {
        return left < right;
    }
};

void demo_heterogeneous_lookup() {
    std::cout << "\n=== Heterogeneous Lookup ===\n";
    std::cout << "C++14 added support for heterogeneous lookup in associative containers.\n\n";
    
    // Standard map with strings (C++11 style)
    std::map<std::string, int> regular_map;
    regular_map["apple"] = 1;
    regular_map["banana"] = 2;
    regular_map["cherry"] = 3;
    
    // Map with heterogeneous lookup (C++14 style)
    std::map<std::string, int, std::less<>> hetero_map;
    hetero_map["apple"] = 1;
    hetero_map["banana"] = 2;
    hetero_map["cherry"] = 3;
    
    // Custom map with heterogeneous lookup
    std::map<std::string, int, StringCompare> custom_hetero_map;
    custom_hetero_map["apple"] = 1;
    custom_hetero_map["banana"] = 2;
    custom_hetero_map["cherry"] = 3;
    
    std::cout << "Example 1: Standard map vs. Heterogeneous lookup map\n";
    std::cout << "// C++11 style\n";
    std::cout << "std::map<std::string, int> regular_map;\n";
    std::cout << "// C++14 style with heterogeneous lookup\n";
    std::cout << "std::map<std::string, int, std::less<>> hetero_map;\n\n";
    
    // Lookup in regular map (creates temporary std::string)
    std::cout << "Lookup in regular map:\n";
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        regular_map.find("banana");  // Creates temporary std::string
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed1 = end1 - start1;
    std::cout << "Time for 1,000,000 lookups: " << elapsed1.count() << " ms\n";
    
    // Lookup in heterogeneous map (no temporary std::string)
    std::cout << "\nLookup in heterogeneous map:\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        hetero_map.find("banana");  // No temporary std::string created
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed2 = end2 - start2;
    std::cout << "Time for 1,000,000 lookups: " << elapsed2.count() << " ms\n";
    
    std::cout << "\nExample 2: Custom comparator with heterogeneous lookup\n";
    std::cout << "struct StringCompare {\n";
    std::cout << "    using is_transparent = std::true_type;  // Enables heterogeneous lookup\n";
    std::cout << "    bool operator()(const std::string& left, const std::string& right) const { ... }\n";
    std::cout << "    bool operator()(const std::string& left, const char* right) const { ... }\n";
    std::cout << "    bool operator()(const char* left, const std::string& right) const { ... }\n";
    std::cout << "};\n";
    std::cout << "std::map<std::string, int, StringCompare> custom_hetero_map;\n\n";
    
    std::cout << "Lookup result for \"cherry\": " << custom_hetero_map.find("cherry")->second << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What problem does heterogeneous lookup solve?\n";
    std::cout << "A1: Heterogeneous lookup solves the performance problem of unnecessary temporary\n";
    std::cout << "    object creation during lookups in associative containers. In C++11 and earlier,\n";
    std::cout << "    looking up a key in a map<string, T> using a string literal like \"key\" would\n";
    std::cout << "    create a temporary std::string object. With heterogeneous lookup in C++14,\n";
    std::cout << "    you can look up using the string literal directly without creating a temporary\n";
    std::cout << "    object, improving performance and reducing memory allocations.\n";
    
    std::cout << "\nQ2: How do you enable heterogeneous lookup in a custom comparator?\n";
    std::cout << "A2: To enable heterogeneous lookup in a custom comparator, you need to:\n";
    std::cout << "    1. Add a type alias 'using is_transparent = std::true_type;' to your comparator.\n";
    std::cout << "    2. Provide overloaded comparison operators that can compare different types.\n";
    std::cout << "    The 'is_transparent' type alias signals to the container that the comparator\n";
    std::cout << "    supports heterogeneous lookup, and the overloaded operators enable the actual\n";
    std::cout << "    comparisons between different types.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use std::less<> instead of std::less<Key> to enable heterogeneous lookup in standard containers.\n";
    std::cout << "2. For custom comparators, always include the 'is_transparent' type alias.\n";
    std::cout << "3. Ensure that all comparison operations between different types maintain the same ordering semantics.\n";
    std::cout << "4. Use heterogeneous lookup in performance-critical code to avoid unnecessary object construction.\n";
}

//==============================================================================
// Feature 3: Standard User-Defined Literals
//==============================================================================
void demo_standard_user_defined_literals() {
    std::cout << "\n=== Standard User-Defined Literals ===\n";
    std::cout << "C++14 introduced several standard library user-defined literals.\n\n";
    
    using namespace std::literals;  // For access to all standard literals
    
    // String literals
    std::cout << "Example 1: String literals\n";
    auto str1 = "Regular string";                  // const char*
    auto str2 = "String literal"s;                 // std::string
    auto str3 = u8"UTF-8 string"s;                 // std::string (UTF-8)
    auto str4 = u"UTF-16 string"s;                 // std::u16string
    auto str5 = U"UTF-32 string"s;                 // std::u32string
    auto str6 = L"Wide string"s;                   // std::wstring
    
    std::cout << "Regular string: " << str1 << " (type: const char*)\n";
    std::cout << "String literal: " << str2 << " (type: std::string)\n";
    
    // Chrono literals
    std::cout << "\nExample 2: Chrono literals\n";
    auto hour = 1h;                                // std::chrono::hours
    auto minute = 30min;                           // std::chrono::minutes
    auto second = 15s;                             // std::chrono::seconds
    auto millisecond = 100ms;                      // std::chrono::milliseconds
    auto microsecond = 100us;                      // std::chrono::microseconds
    auto nanosecond = 100ns;                       // std::chrono::nanoseconds
    
    std::cout << "1h in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(hour).count() << "s\n";
    std::cout << "30min in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(minute).count() << "s\n";
    std::cout << "15s + 100ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(second + millisecond).count() << "ms\n";
    
    // Complex literals
    std::cout << "\nExample 3: Complex literals\n";
    auto c1 = std::complex<double>(3.0, 4.0);  // std::complex<double>
    auto c2 = std::complex<float>(3.0f, 4.0f);  // std::complex<float>
    
    std::cout << "Complex number 3.0 + 4.0i:\n";
    std::cout << "Real part: " << c1.real() << "\n";
    std::cout << "Imaginary part: " << c1.imag() << "\n";
    std::cout << "Magnitude: " << std::abs(c1) << "\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What are the advantages of using standard user-defined literals?\n";
    std::cout << "A1: Standard user-defined literals offer several advantages:\n";
    std::cout << "    1. Improved code readability by making the type explicit in the literal\n";
    std::cout << "    2. Reduced need for explicit type conversions\n";
    std::cout << "    3. More intuitive representation of domain-specific values\n";
    std::cout << "    4. Compile-time type safety for literals\n";
    std::cout << "    5. Consistency across different codebases using standard library types\n";
    
    std::cout << "\nQ2: How do standard user-defined literals differ from custom user-defined literals?\n";
    std::cout << "A2: Standard user-defined literals are provided by the C++ standard library and use\n";
    std::cout << "    reserved syntax (no underscore prefix). Custom user-defined literals must begin\n";
    std::cout << "    with an underscore followed by an alphanumeric character. For example:\n";
    std::cout << "    - Standard: 42s (seconds), \"text\"s (string)\n";
    std::cout << "    - Custom: 42_km, 3.14_rad\n";
    std::cout << "    This restriction ensures that future standard library literals won't conflict\n";
    std::cout << "    with user-defined ones.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use standard literals to improve code readability and reduce type conversion errors.\n";
    std::cout << "2. Include the appropriate namespace (std::literals or its sub-namespaces) to access the literals.\n";
    std::cout << "3. Be consistent in your use of literals throughout your codebase.\n";
    std::cout << "4. For custom literals, always follow the naming convention (underscore prefix).\n";
}

//==============================================================================
// Feature 4: Improved Type Traits
//==============================================================================
// Example class hierarchy
class Base {};
class Derived : public Base {};

// Example class with different constructors
class ExampleClass {
public:
    ExampleClass() = default;
    ExampleClass(int) {}
    ExampleClass(const ExampleClass&) = default;
    ExampleClass(ExampleClass&&) = default;
};

// Example of a final class
class FinalClass final {};

void demo_improved_type_traits() {
    std::cout << "\n=== Improved Type Traits ===\n";
    std::cout << "C++14 enhanced type traits with more convenient aliases and new type traits.\n\n";
    
    std::cout << "Example 1: Type property queries\n";
    std::cout << "std::is_final<FinalClass>::value: " << std::is_final<FinalClass>::value << "\n";
    std::cout << "std::is_final<Base>::value: " << std::is_final<Base>::value << "\n";
    
    std::cout << "\nExample 2: Type relationship queries\n";
    std::cout << "std::is_base_of<Base, Derived>::value: " << std::is_base_of<Base, Derived>::value << "\n";
    std::cout << "std::is_base_of<Derived, Base>::value: " << std::is_base_of<Derived, Base>::value << "\n";
    
    std::cout << "\nExample 3: Type transformations\n";
    using RemoveRefType = std::remove_reference<int&>::type;  // int
    using AddConstType = std::add_const<int>::type;           // const int
    
    std::cout << "std::is_same<RemoveRefType, int>::value: " << std::is_same<RemoveRefType, int>::value << "\n";
    std::cout << "std::is_same<AddConstType, const int>::value: " << std::is_same<AddConstType, const int>::value << "\n";
    
    std::cout << "\nExample 4: Type trait aliases (C++17, but concept from C++14)\n";
    std::cout << "// Instead of std::is_integral<T>::value, you can use std::is_integral_v<T>\n";
    std::cout << "// Instead of std::remove_reference<T>::type, you can use std::remove_reference_t<T>\n";
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What improvements did C++14 make to type traits?\n";
    std::cout << "A1: C++14 added several new type traits and improved existing ones:\n";
    std::cout << "    1. Added new type traits like std::is_final, std::is_null_pointer\n";
    std::cout << "    2. Enhanced existing type traits for better SFINAE support\n";
    std::cout << "    3. Laid the groundwork for the _t and _v suffixes (fully implemented in C++17)\n";
    std::cout << "    4. Improved consistency and completeness of the type traits library\n";
    
    std::cout << "\nQ2: How are type traits used in template metaprogramming?\n";
    std::cout << "A2: Type traits are fundamental to template metaprogramming for several purposes:\n";
    std::cout << "    1. Compile-time type inspection (e.g., is_integral, is_class)\n";
    std::cout << "    2. Type transformations (e.g., remove_reference, add_const)\n";
    std::cout << "    3. SFINAE (Substitution Failure Is Not An Error) for template specialization\n";
    std::cout << "    4. Conditional compilation based on type properties\n";
    std::cout << "    5. Optimizing code paths based on type characteristics\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Use type traits to make your templates more robust and type-safe.\n";
    std::cout << "2. Prefer the _t and _v suffixes (in C++17) for more concise and readable code.\n";
    std::cout << "3. Combine type traits with static_assert for clear compile-time error messages.\n";
    std::cout << "4. Use type traits for SFINAE to enable/disable function templates based on type properties.\n";
}

//==============================================================================
// Feature 5: Tuple Addressing by Type
//==============================================================================
// Example of a tuple with different types
using PersonInfo = std::tuple<std::string, int, double>;  // name, age, height

// Helper function to print a person's info
void print_person(const PersonInfo& person) {
    std::cout << "Name: " << std::get<0>(person) << "\n";
    std::cout << "Age: " << std::get<1>(person) << "\n";
    std::cout << "Height: " << std::get<2>(person) << " m\n";
}

// Helper function to print a person's info using type-based access (C++14)
void print_person_by_type(const PersonInfo& person) {
    std::cout << "Name: " << std::get<std::string>(person) << "\n";
    std::cout << "Age: " << std::get<int>(person) << "\n";
    std::cout << "Height: " << std::get<double>(person) << " m\n";
}

void demo_tuple_addressing_by_type() {
    std::cout << "\n=== Tuple Addressing by Type ===\n";
    std::cout << "C++14 allows accessing tuple elements by their type, not just by index.\n\n";
    
    // Create a tuple
    PersonInfo person{"John Doe", 30, 1.85};
    
    std::cout << "Example 1: Accessing tuple elements by index (C++11)\n";
    std::cout << "std::get<0>(person): " << std::get<0>(person) << "\n";
    std::cout << "std::get<1>(person): " << std::get<1>(person) << "\n";
    std::cout << "std::get<2>(person): " << std::get<2>(person) << "\n";
    
    std::cout << "\nExample 2: Accessing tuple elements by type (C++14)\n";
    std::cout << "std::get<std::string>(person): " << std::get<std::string>(person) << "\n";
    std::cout << "std::get<int>(person): " << std::get<int>(person) << "\n";
    std::cout << "std::get<double>(person): " << std::get<double>(person) << "\n";
    
    std::cout << "\nExample 3: Comparing the two approaches\n";
    std::cout << "// Index-based access (C++11):\n";
    print_person(person);
    std::cout << "\n// Type-based access (C++14):\n";
    print_person_by_type(person);
    
    std::cout << "\nLimitation: Each type must appear only once in the tuple.\n";
    // This would cause a compile error:
    // using AmbiguousTuple = std::tuple<int, int, double>;
    // AmbiguousTuple t{1, 2, 3.14};
    // std::get<int>(t);  // Error: multiple ints in the tuple
    
    std::cout << "\nInterview Questions:\n";
    std::cout << "Q1: What problem does tuple addressing by type solve?\n";
    std::cout << "A1: Tuple addressing by type solves several problems:\n";
    std::cout << "    1. Improves code readability by making the intent clearer\n";
    std::cout << "    2. Reduces errors when tuple structure changes (reordering elements)\n";
    std::cout << "    3. Makes code more self-documenting by using types instead of magic numbers\n";
    std::cout << "    4. Enables more generic code that doesn't depend on specific tuple indices\n";
    
    std::cout << "\nQ2: What are the limitations of tuple addressing by type?\n";
    std::cout << "A2: The main limitation is that each type must appear only once in the tuple.\n";
    std::cout << "    If a tuple contains multiple elements of the same type, using std::get<Type>\n";
    std::cout << "    will result in a compile-time error. In such cases, you must use index-based\n";
    std::cout << "    access or create wrapper types to make the types unique.\n";
    
    std::cout << "\nBest Practices:\n";
    std::cout << "1. Prefer type-based access when each type appears only once in the tuple.\n";
    std::cout << "2. Use meaningful types in tuples to make type-based access more intuitive.\n";
    std::cout << "3. Consider using std::pair or custom structs instead of tuples for better readability.\n";
    std::cout << "4. If you need multiple elements of the same type, consider using wrapper types or tagged types.\n";
}

// Main function
int main() {
    std::cout << "Welcome to the C++14 Features Guide!\n";
    std::cout << "This program demonstrates various C++14 features with examples and explanations.\n";
    
    show_main_menu();
    
    std::cout << "\nThank you for using the C++14 Features Guide!\n";
    return 0;
}

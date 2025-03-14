/**
 * C++17 Intermediate Features
 * 
 * This file demonstrates the more complex features introduced in C++17.
 * Each feature is explained with examples and comments to help with interview preparation.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <optional>
#include <variant>
#include <any>
#include <memory>
#include <numeric>

// ======================================================================
// Fold Expressions
// ======================================================================
template<typename... Args>
auto sum(Args... args) {
    return (args + ...); // Unary right fold
}

template<typename... Args>
auto sum_with_init(Args... args) {
    return (0 + ... + args); // Binary right fold with initial value
}

template<typename... Args>
void print_all(Args... args) {
    (std::cout << ... << args) << '\n'; // Unary left fold
}

template<typename T, typename... Args>
bool are_all_same(T first, Args... rest) {
    return ((first == rest) && ...); // Unary right fold with logical AND
}

void demo_fold_expressions() {
    std::cout << "\n=== Fold Expressions ===\n";
    
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "Sum with init: " << sum_with_init(1, 2, 3, 4, 5) << "\n";
    
    print_all("C++", 17, " is ", "awesome!");
    
    std::cout << "Are all same? " << std::boolalpha 
              << are_all_same(42, 42, 42, 42) << "\n";
    std::cout << "Are all same? " << std::boolalpha 
              << are_all_same(42, 42, 43, 42) << "\n";
    
    // Interview tip: Fold expressions simplify variadic template programming by
    // providing a concise syntax for applying binary operators to parameter packs
}

// ======================================================================
// constexpr if
// ======================================================================
template<typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>) {
        // This branch is only instantiated when T is a pointer type
        return *t;
    } else if constexpr (std::is_array_v<T>) {
        // This branch is only instantiated when T is an array type
        return t[0];
    } else {
        // This branch is only instantiated when T is neither a pointer nor an array
        return t;
    }
}

void demo_constexpr_if() {
    std::cout << "\n=== constexpr if ===\n";
    
    int i = 42;
    int* p = &i;
    int arr[3] = {1, 2, 3};
    
    std::cout << "Value from int: " << get_value(i) << "\n";
    std::cout << "Value from pointer: " << get_value(p) << "\n";
    std::cout << "Value from array: " << get_value(arr) << "\n";
    
    // Interview tip: constexpr if enables compile-time conditional compilation
    // within templates, eliminating the need for SFINAE or tag dispatching
    // for template specialization
}

// ======================================================================
// Template Argument Deduction for Class Templates
// ======================================================================
void demo_template_argument_deduction() {
    std::cout << "\n=== Template Argument Deduction for Class Templates ===\n";
    
    // Before C++17:
    std::pair<std::string, int> p1("hello", 42);
    auto p2 = std::make_pair("hello", 42);
    
    // With C++17:
    std::pair p3("hello", 42); // Deduces std::pair<const char*, int>
    
    // Before C++17:
    std::vector<int> v1{1, 2, 3, 4, 5};
    
    // With C++17:
    std::vector v2{1, 2, 3, 4, 5}; // Deduces std::vector<int>
    
    // With different types (requires explicit template arguments)
    // std::vector v3 = {1, 2, 3, 4.5}; // Error: can't deduce element type - commented out as it causes a compilation error
    std::vector<double> v4 = {1, 2, 3, 4.5}; // OK: explicit template argument
    
    std::cout << "p3: (" << p3.first << ", " << p3.second << ")\n";
    std::cout << "v2 size: " << v2.size() << ", elements: ";
    for (auto i : v2) std::cout << i << " ";
    std::cout << "\n";
    
    // Interview tip: Class template argument deduction reduces verbosity
    // by allowing the compiler to deduce template arguments from constructor arguments
}

// ======================================================================
// std::invoke
// ======================================================================
struct Foo {
    void bar(int x) {
        std::cout << "Foo::bar called with " << x << "\n";
    }
    
    static void static_bar(int x) {
        std::cout << "Foo::static_bar called with " << x << "\n";
    }
};

void free_function(int x) {
    std::cout << "free_function called with " << x << "\n";
}

void demo_invoke() {
    std::cout << "\n=== std::invoke ===\n";
    
    // Invoke a free function
    std::invoke(free_function, 42);
    
    // Invoke a member function
    Foo foo;
    std::invoke(&Foo::bar, foo, 42);
    
    // Invoke a member function through pointer
    std::invoke(&Foo::bar, &foo, 42);
    
    // Invoke a static member function
    std::invoke(&Foo::static_bar, 42);
    
    // Invoke a lambda
    auto lambda = [](int x) { std::cout << "Lambda called with " << x << "\n"; };
    std::invoke(lambda, 42);
    
    // Invoke a function object
    struct Functor {
        void operator()(int x) {
            std::cout << "Functor called with " << x << "\n";
        }
    };
    Functor functor;
    std::invoke(functor, 42);
    
    // Interview tip: std::invoke provides a uniform way to call any callable object,
    // which is especially useful in generic code that needs to handle different
    // types of callables consistently
}

// ======================================================================
// std::apply
// ======================================================================
template<typename F, typename Tuple>
auto my_apply(F&& f, Tuple&& t) {
    return std::apply(std::forward<F>(f), std::forward<Tuple>(t));
}

void demo_apply() {
    std::cout << "\n=== std::apply ===\n";
    
    auto add = [](int a, int b, int c) { return a + b + c; };
    
    std::tuple<int, int, int> t{1, 2, 3};
    int sum = std::apply(add, t);
    std::cout << "Sum of tuple elements: " << sum << "\n";
    
    // Using with std::make_from_tuple
    struct Point {
        int x, y, z;
        Point(int a, int b, int c) : x(a), y(b), z(c) {}
        void print() const { std::cout << "Point(" << x << ", " << y << ", " << z << ")\n"; }
    };
    
    auto point_tuple = std::make_tuple(10, 20, 30);
    auto point = std::make_from_tuple<Point>(point_tuple);
    point.print();
    
    // Interview tip: std::apply allows you to call a callable with arguments
    // unpacked from a tuple, which is useful for generic programming and
    // when working with tuple-like objects
}

// ======================================================================
// Guaranteed Copy/Move Elision
// ======================================================================
class Heavy {
private:
    int* data;
    size_t size;
    
public:
    Heavy(size_t n) : size(n) {
        std::cout << "Constructor called\n";
        data = new int[size];
    }
    
    ~Heavy() {
        std::cout << "Destructor called\n";
        delete[] data;
    }
    
    // Copy constructor
    Heavy(const Heavy& other) : size(other.size) {
        std::cout << "Copy constructor called\n";
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
    
    // Move constructor
    Heavy(Heavy&& other) noexcept : data(other.data), size(other.size) {
        std::cout << "Move constructor called\n";
        other.data = nullptr;
        other.size = 0;
    }
};

Heavy create_heavy() {
    return Heavy(1000000); // Pre-C++17: might call move constructor
                          // C++17: guaranteed to elide the copy/move
}

void demo_copy_elision() {
    std::cout << "\n=== Guaranteed Copy/Move Elision ===\n";
    
    std::cout << "Creating heavy object directly:\n";
    Heavy h1(10);
    
    std::cout << "\nCreating heavy object from function:\n";
    Heavy h2 = create_heavy();
    
    // Interview tip: C++17 guarantees copy elision in certain scenarios,
    // which can significantly improve performance by eliminating unnecessary
    // copy and move operations, especially for large objects
}

// ======================================================================
// Main function to demonstrate all features
// ======================================================================
int main() {
    std::cout << "===== C++17 Intermediate Features =====\n";
    
    demo_fold_expressions();
    demo_constexpr_if();
    demo_template_argument_deduction();
    demo_invoke();
    demo_apply();
    demo_copy_elision();
    
    std::cout << "\nAll intermediate C++17 features demonstrated successfully!\n";
    return 0;
}

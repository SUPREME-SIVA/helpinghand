#include <iostream>
#include <type_traits>
#include <chrono>

// A compile-time optimized math operations class
template<typename T>
class MathOperations {
public:
    // Optimized power calculation using constexpr if
    static T power(T base, unsigned int exponent) {
        if constexpr (std::is_integral_v<T>) {
            // For integral types, use bit manipulation when possible
            if (exponent == 0) return 1;
            if (exponent == 1) return base;
            if (exponent == 2) return base * base;
            
            // For powers of 2, use left shift
            if ((exponent & (exponent - 1)) == 0) {
                unsigned int shift = 0;
                while (exponent > 1) {
                    exponent >>= 1;
                    shift++;
                }
                return base << shift;
            }
        }
        
        // For floating point or non-power-of-2 cases
        T result = 1;
        while (exponent > 0) {
            if (exponent & 1) result *= base;
            base *= base;
            exponent >>= 1;
        }
        return result;
    }

    // Optimized absolute value calculation
    static T abs(T value) {
        if constexpr (std::is_unsigned_v<T>) {
            // For unsigned types, no operation needed
            return value;
        } else if constexpr (std::is_floating_point_v<T>) {
            // For floating point, use std::abs
            return std::abs(value);
        } else {
            // For signed integers
            return value < 0 ? -value : value;
        }
    }
};

int main() {
    // Test with different types
    std::cout << "Integer power (2^10): " << MathOperations<int>::power(2, 10) << std::endl;
    std::cout << "Double power (2.5^3): " << MathOperations<double>::power(2.5, 3) << std::endl;
    
    // Test absolute value
    std::cout << "Absolute of -42: " << MathOperations<int>::abs(-42) << std::endl;
    std::cout << "Absolute of unsigned 42: " << MathOperations<unsigned int>::abs(42) << std::endl;
    std::cout << "Absolute of -42.5: " << MathOperations<double>::abs(-42.5) << std::endl;

    return 0;
}

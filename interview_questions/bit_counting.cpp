/**
 * @file bit_counting.cpp
 * @brief Modern C++ implementation of efficient bit counting algorithms
 *
 * This file demonstrates various approaches for counting the number of set bits (1s)
 * in an integer, with a focus on optimization, performance, and modern C++ features.
 */

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <chrono>
#include <array>
#include <iomanip>
#include <cstdint>
#include <functional>
#include <algorithm>

/**
 * @brief Namespace containing bit manipulation utilities and algorithms
 */
namespace bits {

/**
 * @brief Count the number of set bits (1s) using Brian Kernighan's algorithm
 *
 * This approach uses the fact that n & (n-1) clears the least significant bit.
 * Time Complexity: O(number of set bits)
 * Space Complexity: O(1)
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
constexpr unsigned int count_kernighan(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    unsigned int count = 0;
    
    // Continue until n becomes 0
    while (n) {
        n &= (n - 1);  // Clear the least significant bit
        count++;
    }
    
    return count;
}

/**
 * @brief Count the number of set bits (1s) using a naive loop
 *
 * This approach checks each bit by shifting and ANDing with 1.
 * Time Complexity: O(number of bits in the data type)
 * Space Complexity: O(1)
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
constexpr unsigned int count_naive(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    constexpr size_t bits = sizeof(T) * 8;
    unsigned int count = 0;
    
    // Check each bit
    for (size_t i = 0; i < bits; i++) {
        if (n & (static_cast<T>(1) << i)) {
            count++;
        }
    }
    
    return count;
}

/**
 * @brief Count the number of set bits (1s) using lookup table
 *
 * This approach uses a precomputed lookup table for counting bits in bytes.
 * Time Complexity: O(1) for fixed-size integers
 * Space Complexity: O(256) for the lookup table
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
unsigned int count_lookup(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    // Precomputed lookup table for number of bits in a byte
    static const std::array<uint8_t, 256> bit_count_table = []() {
        std::array<uint8_t, 256> table{};
        for (size_t i = 0; i < 256; i++) {
            table[i] = static_cast<uint8_t>(count_kernighan<uint8_t>(i));
        }
        return table;
    }();
    
    unsigned int count = 0;
    constexpr size_t bytes = sizeof(T);
    
    // Count bits in each byte
    for (size_t i = 0; i < bytes; i++) {
        count += bit_count_table[(n >> (i * 8)) & 0xFF];
    }
    
    return count;
}

/**
 * @brief Count the number of set bits (1s) using SWAR (SIMD Within A Register)
 *
 * This approach uses parallel bit counting techniques.
 * Time Complexity: O(1) for fixed-size integers
 * Space Complexity: O(1)
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
unsigned int count_parallel(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    // For 8-bit integers
    if constexpr (sizeof(T) == 1) {
        n = (n & 0x55) + ((n >> 1) & 0x55);
        n = (n & 0x33) + ((n >> 2) & 0x33);
        n = (n & 0x0F) + ((n >> 4) & 0x0F);
        return static_cast<unsigned int>(n);
    }
    // For 16-bit integers
    else if constexpr (sizeof(T) == 2) {
        n = (n & 0x5555) + ((n >> 1) & 0x5555);
        n = (n & 0x3333) + ((n >> 2) & 0x3333);
        n = (n & 0x0F0F) + ((n >> 4) & 0x0F0F);
        n = (n & 0x00FF) + ((n >> 8) & 0x00FF);
        return static_cast<unsigned int>(n);
    }
    // For 32-bit integers
    else if constexpr (sizeof(T) == 4) {
        n = n - ((n >> 1) & 0x55555555);
        n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
        n = (n + (n >> 4)) & 0x0F0F0F0F;
        n = n + (n >> 8);
        n = n + (n >> 16);
        return static_cast<unsigned int>(n & 0x3F);
    }
    // For 64-bit integers
    else if constexpr (sizeof(T) == 8) {
        n = n - ((n >> 1) & 0x5555555555555555ULL);
        n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
        n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
        n = n + (n >> 8);
        n = n + (n >> 16);
        n = n + (n >> 32);
        return static_cast<unsigned int>(n & 0x7F);
    }
    // Fallback for other sizes
    else {
        return count_kernighan(n);
    }
}

/**
 * @brief Count the number of set bits (1s) using std::bitset
 *
 * This approach uses the C++ standard library's bitset.
 * Time Complexity: O(1) for fixed-size integers
 * Space Complexity: O(1)
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
unsigned int count_bitset(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    constexpr size_t bits = sizeof(T) * 8;
    std::bitset<bits> b(n);
    return static_cast<unsigned int>(b.count());
}

/**
 * @brief Count the number of set bits (1s) using compiler intrinsics
 *
 * This approach uses compiler-specific intrinsics for bit counting.
 * Note: This is not portable across all compilers.
 * Time Complexity: O(1) for fixed-size integers
 * Space Complexity: O(1)
 *
 * @tparam T Integer type
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
template <typename T>
unsigned int count_intrinsic(T n) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    // For 32-bit integers
    if constexpr (sizeof(T) == 4) {
        #if defined(__GNUC__) || defined(__clang__)
            return __builtin_popcount(static_cast<unsigned int>(n));
        #else
            return count_parallel(n);
        #endif
    }
    // For 64-bit integers
    else if constexpr (sizeof(T) == 8) {
        #if defined(__GNUC__) || defined(__clang__)
            return __builtin_popcountll(static_cast<unsigned long long>(n));
        #else
            return count_parallel(n);
        #endif
    }
    // Fallback for other sizes
    else {
        return count_parallel(n);
    }
}

/**
 * @brief Get the binary representation of a number as a string
 *
 * @tparam T Integer type
 * @param n The number to convert
 * @param group_size Number of bits per group (for readability)
 * @return Binary representation as a string
 */
template <typename T>
std::string to_binary_string(T n, size_t group_size = 8) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    
    constexpr size_t bits = sizeof(T) * 8;
    std::string result;
    result.reserve(bits + (bits / group_size));
    
    for (int i = bits - 1; i >= 0; i--) {
        result.push_back((n & (static_cast<T>(1) << i)) ? '1' : '0');
        if (i % group_size == 0 && i > 0) {
            result.push_back(' ');
        }
    }
    
    return result;
}

} // namespace bits

/**
 * @brief Benchmark a bit counting algorithm
 * 
 * @tparam T Integer type
 * @tparam Func Function type
 * @param n The number to count bits in
 * @param count_func The bit counting function to benchmark
 * @param name The name of the algorithm
 * @return The number of set bits and the time taken in microseconds
 */
template <typename T, typename Func>
std::pair<unsigned int, double> benchmark_bit_count(T n, Func count_func, const std::string& name) {
    // Warm up the cache
    volatile unsigned int dummy = count_func(n);
    (void)dummy;
    
    // Measure execution time
    constexpr int iterations = 10000;
    unsigned int result = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        result = count_func(n);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    // Return the result and the average time per call in microseconds
    return {result, static_cast<double>(duration.count()) / iterations / 1000.0};
}

/**
 * @brief Run all bit counting benchmarks for a given number
 * 
 * @tparam T Integer type
 * @param n The number to count bits in
 */
template <typename T>
void run_benchmarks(T n) {
    std::cout << "Number: " << n << " (0x" << std::hex << n << std::dec << ")\n";
    std::cout << "Binary: " << bits::to_binary_string(n) << "\n\n";
    
    // Define the algorithms to benchmark
    struct Algorithm {
        std::string name;
        std::function<unsigned int(T)> func;
    };
    
    std::vector<Algorithm> algorithms = {
        {"Naive Loop", bits::count_naive<T>},
        {"Kernighan", bits::count_kernighan<T>},
        {"Lookup Table", bits::count_lookup<T>},
        {"Parallel Bits", bits::count_parallel<T>},
        {"std::bitset", bits::count_bitset<T>},
        {"Intrinsic", bits::count_intrinsic<T>}
    };
    
    // Run benchmarks
    std::vector<std::pair<std::string, std::pair<unsigned int, double>>> results;
    
    for (const auto& algo : algorithms) {
        auto [count, time] = benchmark_bit_count(n, algo.func, algo.name);
        results.push_back({algo.name, {count, time}});
    }
    
    // Verify all methods returned the same result
    bool consistent = true;
    unsigned int expected = results[0].second.first;
    
    for (size_t i = 1; i < results.size(); i++) {
        if (results[i].second.first != expected) {
            consistent = false;
            break;
        }
    }
    
    if (!consistent) {
        std::cout << "Error: Inconsistent results detected!\n\n";
    }
    
    // Print results in a table
    std::cout << std::left << std::setw(15) << "Algorithm" 
              << std::setw(10) << "Bits" 
              << "Time (μs)\n";
    std::cout << std::string(40, '-') << "\n";
    
    for (const auto& [name, result] : results) {
        auto [count, time] = result;
        std::cout << std::left << std::setw(15) << name 
                  << std::setw(10) << count 
                  << std::fixed << std::setprecision(3) << time << "\n";
    }
    
    std::cout << "\n";
}

/**
 * @brief The entry point of the program
 */
int main() {
    std::cout << "=== C++ Bit Counting Algorithms ===\n\n";
    
    // Test with some interesting values
    run_benchmarks<uint64_t>(0);                        // No bits set
    run_benchmarks<uint64_t>(1);                        // Only one bit set
    run_benchmarks<uint64_t>(0xFFFFFFFFFFFFFFFFULL);    // All bits set
    run_benchmarks<uint64_t>(0x5555555555555555ULL);    // Alternating bits
    run_benchmarks<uint64_t>(0x1234567890ABCDEFULL);    // Complex pattern
    
    // Random number
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    uint64_t random_num = (static_cast<uint64_t>(std::rand()) << 32) | std::rand();
    run_benchmarks<uint64_t>(random_num);
    
    // Demonstrate with different integer types
    std::cout << "=== Testing Different Integer Types ===\n\n";
    
    uint8_t  val8  = 0xA5;                          // 10100101
    uint16_t val16 = 0xA55A;                        // 1010010101011010
    uint32_t val32 = 0xA55A5AA5;                    // 10100101010110100101101010100101
    
    std::cout << "8-bit integer:\n";
    run_benchmarks<uint8_t>(val8);
    
    std::cout << "16-bit integer:\n";
    run_benchmarks<uint16_t>(val16);
    
    std::cout << "32-bit integer:\n";
    run_benchmarks<uint32_t>(val32);
    
    return 0;
}

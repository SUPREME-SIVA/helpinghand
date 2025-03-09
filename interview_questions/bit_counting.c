/**
 * @file bit_counting.c
 * @brief Implementation of efficient bit counting algorithms
 *
 * This file demonstrates various approaches for counting the number of set bits (1s)
 * in an integer, with a focus on optimization and performance.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/**
 * @brief Count the number of set bits (1s) using Brian Kernighan's algorithm
 *
 * This approach uses the fact that n & (n-1) clears the least significant bit.
 * Time Complexity: O(number of set bits)
 * Space Complexity: O(1)
 *
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
unsigned int count_bits_kernighan(uint64_t n) {
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
 * Time Complexity: O(number of bits in the data type) = O(64) for uint64_t
 * Space Complexity: O(1)
 *
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
unsigned int count_bits_naive(uint64_t n) {
    unsigned int count = 0;
    
    // Check each bit
    for (int i = 0; i < 64; i++) {
        if (n & 1ULL) {
            count++;
        }
        n >>= 1;
    }
    
    return count;
}

/**
 * @brief Count the number of set bits (1s) using lookup table
 *
 * This approach uses a precomputed lookup table for counting bits in bytes.
 * Time Complexity: O(1) for 64-bit integers
 * Space Complexity: O(256) for the lookup table
 *
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
unsigned int count_bits_lookup(uint64_t n) {
    // Precomputed lookup table for number of bits in a byte
    static const unsigned char bit_count_table[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };
    
    // Count bits in each byte
    return bit_count_table[n & 0xff] +
           bit_count_table[(n >> 8) & 0xff] +
           bit_count_table[(n >> 16) & 0xff] +
           bit_count_table[(n >> 24) & 0xff] +
           bit_count_table[(n >> 32) & 0xff] +
           bit_count_table[(n >> 40) & 0xff] +
           bit_count_table[(n >> 48) & 0xff] +
           bit_count_table[(n >> 56) & 0xff];
}

/**
 * @brief Count the number of set bits (1s) using SWAR (SIMD Within A Register)
 *
 * This approach uses parallel bit counting techniques.
 * Time Complexity: O(1) for 64-bit integers
 * Space Complexity: O(1)
 *
 * @param n The number to count bits in
 * @return Number of set bits (1s)
 */
unsigned int count_bits_parallel(uint64_t n) {
    // Count bits in parallel
    n = n - ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    n = n + (n >> 8);
    n = n + (n >> 16);
    n = n + (n >> 32);
    return n & 0x7F;
}

/**
 * @brief Benchmark the different bit counting algorithms
 */
void benchmark_bit_counting(uint64_t n) {
    printf("Number: %llu (0x%llx)\n", (unsigned long long)n, (unsigned long long)n);
    
    // Measure naive approach
    clock_t start = clock();
    unsigned int naive_count = count_bits_naive(n);
    clock_t end = clock();
    double naive_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000000.0;
    
    // Measure Kernighan's algorithm
    start = clock();
    unsigned int kernighan_count = count_bits_kernighan(n);
    end = clock();
    double kernighan_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000000.0;
    
    // Measure lookup table approach
    start = clock();
    unsigned int lookup_count = count_bits_lookup(n);
    end = clock();
    double lookup_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000000.0;
    
    // Measure parallel bit counting
    start = clock();
    unsigned int parallel_count = count_bits_parallel(n);
    end = clock();
    double parallel_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000000.0;
    
    // Print results
    printf("Naive approach:        %2u bits in %.2f μs\n", naive_count, naive_time);
    printf("Kernighan's algorithm: %2u bits in %.2f μs\n", kernighan_count, kernighan_time);
    printf("Lookup table approach: %2u bits in %.2f μs\n", lookup_count, lookup_time);
    printf("Parallel bit counting: %2u bits in %.2f μs\n", parallel_count, parallel_time);
    
    // Verify all methods returned the same result
    if (naive_count != kernighan_count || naive_count != lookup_count || naive_count != parallel_count) {
        printf("Error: Inconsistent results detected!\n");
    }
    
    // Print binary representation
    printf("Binary: ");
    for (int i = 63; i >= 0; i--) {
        printf("%c", (n & (1ULL << i)) ? '1' : '0');
        if (i % 8 == 0) printf(" ");
    }
    printf("\n\n");
}

/**
 * @brief The entry point of the program
 */
int main() {
    printf("=== C Bit Counting Algorithms ===\n\n");
    
    // Test with some interesting values
    benchmark_bit_counting(0);                        // No bits set
    benchmark_bit_counting(1);                        // Only one bit set
    benchmark_bit_counting(0xFFFFFFFFFFFFFFFFULL);    // All bits set
    benchmark_bit_counting(0x5555555555555555ULL);    // Alternating bits
    benchmark_bit_counting(0x1234567890ABCDEFULL);    // Complex pattern
    
    // Random number
    srand(time(NULL));
    uint64_t random_num = ((uint64_t)rand() << 32) | rand();
    benchmark_bit_counting(random_num);
    
    return 0;
}

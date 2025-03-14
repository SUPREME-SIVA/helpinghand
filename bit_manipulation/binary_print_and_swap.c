/**
 * @file binary_print_and_swap.c
 * @brief Program to print binary representation of a number and swap bits at given positions
 * 
 * This program demonstrates:
 * 1. Converting a decimal number to its binary representation
 * 2. Swapping two bits at specified positions
 * 3. Verifying that swapping the same bits again returns to the original number
 * 
 * Compilation: gcc -o binary_print_and_swap binary_print_and_swap.c
 * Usage: ./binary_print_and_swap
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * @brief Print the binary representation of a number
 * 
 * @param num The number to convert to binary
 * @param bits Number of bits to print (e.g., 8, 16, 32)
 */
void print_binary(unsigned int num, int bits) {
    printf("Binary representation of %u (%d bits): ", num, bits);
    
    // Create a mask for the most significant bit
    unsigned int mask = 1u << (bits - 1);
    
    // Print each bit
    for (int i = 0; i < bits; i++) {
        // Print '1' if bit is set, '0' otherwise
        printf("%c", (num & mask) ? '1' : '0');
        
        // Group bits in sets of 4 for readability
        if ((i + 1) % 4 == 0 && i < bits - 1) {
            printf(" ");
        }
        
        // Shift mask to the right
        mask >>= 1;
    }
    printf("\n");
}

/**
 * @brief Swap bits at two specified positions in a number
 * 
 * @param num The original number
 * @param pos1 Position of first bit (0-indexed from right)
 * @param pos2 Position of second bit (0-indexed from right)
 * @return unsigned int The number after swapping bits
 */
unsigned int swap_bits(unsigned int num, int pos1, int pos2) {
    // Check if positions are valid
    if (pos1 < 0 || pos2 < 0 || pos1 >= sizeof(unsigned int) * CHAR_BIT || 
        pos2 >= sizeof(unsigned int) * CHAR_BIT) {
        fprintf(stderr, "Error: Invalid bit positions. Must be between 0 and %lu\n", 
                sizeof(unsigned int) * CHAR_BIT - 1);
        return num; // Return original number if positions are invalid
    }
    
    // Get the bits at the specified positions
    unsigned int bit1 = (num >> pos1) & 1u;
    unsigned int bit2 = (num >> pos2) & 1u;
    
    // If the bits are different, swap them
    if (bit1 != bit2) {
        // Create a mask with 1s at the positions to be swapped
        unsigned int mask = (1u << pos1) | (1u << pos2);
        
        // XOR with the mask to flip the bits at those positions
        num ^= mask;
    }
    
    return num;
}

/**
 * @brief Main function to demonstrate binary printing and bit swapping
 */
int main() {
    unsigned int num;
    int pos1, pos2, bits;
    
    printf("Enter a positive integer: ");
    if (scanf("%u", &num) != 1) {
        fprintf(stderr, "Error: Invalid input for number\n");
        return EXIT_FAILURE;
    }
    
    printf("How many bits to display (8, 16, 32): ");
    if (scanf("%d", &bits) != 1 || (bits != 8 && bits != 16 && bits != 32)) {
        fprintf(stderr, "Error: Please enter 8, 16, or 32 for bits\n");
        return EXIT_FAILURE;
    }
    
    // Print original binary representation
    printf("\n=== Original Number ===\n");
    print_binary(num, bits);
    
    // Get positions for bit swapping
    printf("Enter first bit position to swap (0-%d): ", bits - 1);
    if (scanf("%d", &pos1) != 1 || pos1 < 0 || pos1 >= bits) {
        fprintf(stderr, "Error: Invalid bit position\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter second bit position to swap (0-%d): ", bits - 1);
    if (scanf("%d", &pos2) != 1 || pos2 < 0 || pos2 >= bits) {
        fprintf(stderr, "Error: Invalid bit position\n");
        return EXIT_FAILURE;
    }
    
    // Swap bits and print result
    unsigned int result = swap_bits(num, pos1, pos2);
    printf("\n=== After First Swap ===\n");
    printf("After swapping bits at positions %d and %d:\n", pos1, pos2);
    print_binary(result, bits);
    printf("Decimal value: %u\n", result);
    
    // Swap bits again and verify it returns to the original number
    unsigned int final_result = swap_bits(result, pos1, pos2);
    printf("\n=== After Second Swap (Should be the same as original) ===\n");
    printf("After swapping bits at positions %d and %d again:\n", pos1, pos2);
    print_binary(final_result, bits);
    printf("Decimal value: %u\n", final_result);
    
    // Verify that the final result matches the original number
    if (final_result == num) {
        printf("\nVerification: SUCCESS - The number returned to its original value after two swaps.\n");
    } else {
        printf("\nVerification: FAILED - The number did not return to its original value after two swaps.\n");
        printf("This is unexpected and indicates a bug in the implementation.\n");
    }
    
    return EXIT_SUCCESS;
}

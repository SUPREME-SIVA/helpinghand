/**
 * @file binary_print_and_swap.cpp
 * @brief Program to print binary representation of a number and swap bits at given positions
 * 
 * This program demonstrates:
 * 1. Converting a decimal number to its binary representation
 * 2. Swapping two bits at specified positions
 * 3. Verifying that swapping the same bits again returns to the original number
 * 
 * Compilation: g++ -std=c++17 -o binary_print_and_swap binary_print_and_swap.cpp
 * Usage: ./binary_print_and_swap
 */

#include <iostream>
#include <string>
#include <bitset>
#include <limits>
#include <stdexcept>

/**
 * @brief Print the binary representation of a number using std::bitset
 * 
 * @tparam Bits Number of bits to print (e.g., 8, 16, 32)
 * @param num The number to convert to binary
 */
template<size_t Bits>
void print_binary(unsigned int num) {
    std::bitset<Bits> bits(num);
    std::cout << "Binary representation of " << num << " (" << Bits << " bits): ";
    
    std::string binary = bits.to_string();
    
    // Add spaces for readability (every 4 bits)
    for (size_t i = 0; i < binary.length(); ++i) {
        std::cout << binary[i];
        if ((i + 1) % 4 == 0 && i < binary.length() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Swap bits at two specified positions in a number
 * 
 * @param num The original number
 * @param pos1 Position of first bit (0-indexed from right)
 * @param pos2 Position of second bit (0-indexed from right)
 * @return unsigned int The number after swapping bits
 * @throws std::invalid_argument if positions are invalid
 */
unsigned int swap_bits(unsigned int num, int pos1, int pos2) {
    // Check if positions are valid
    const size_t max_bits = sizeof(unsigned int) * 8;
    if (pos1 < 0 || pos2 < 0 || pos1 >= static_cast<int>(max_bits) || 
        pos2 >= static_cast<int>(max_bits)) {
        throw std::invalid_argument("Invalid bit positions. Must be between 0 and " + 
                                   std::to_string(max_bits - 1));
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
    
    try {
        std::cout << "Enter a positive integer: ";
        if (!(std::cin >> num)) {
            throw std::invalid_argument("Invalid input for number");
        }
        
        std::cout << "How many bits to display (8, 16, 32): ";
        if (!(std::cin >> bits) || (bits != 8 && bits != 16 && bits != 32)) {
            throw std::invalid_argument("Please enter 8, 16, or 32 for bits");
        }
        
        // Print original binary representation
        std::cout << "\n=== Original Number ===" << std::endl;
        switch (bits) {
            case 8:
                print_binary<8>(num);
                break;
            case 16:
                print_binary<16>(num);
                break;
            case 32:
                print_binary<32>(num);
                break;
        }
        
        // Get positions for bit swapping
        std::cout << "Enter first bit position to swap (0-" << (bits - 1) << "): ";
        if (!(std::cin >> pos1) || pos1 < 0 || pos1 >= bits) {
            throw std::invalid_argument("Invalid bit position");
        }
        
        std::cout << "Enter second bit position to swap (0-" << (bits - 1) << "): ";
        if (!(std::cin >> pos2) || pos2 < 0 || pos2 >= bits) {
            throw std::invalid_argument("Invalid bit position");
        }
        
        // Swap bits and print result
        unsigned int result = swap_bits(num, pos1, pos2);
        std::cout << "\n=== After First Swap ===" << std::endl;
        std::cout << "After swapping bits at positions " << pos1 << " and " << pos2 << ":" << std::endl;
        
        switch (bits) {
            case 8:
                print_binary<8>(result);
                break;
            case 16:
                print_binary<16>(result);
                break;
            case 32:
                print_binary<32>(result);
                break;
        }
        
        std::cout << "Decimal value: " << result << std::endl;
        
        // Swap bits again and verify it returns to the original number
        unsigned int final_result = swap_bits(result, pos1, pos2);
        std::cout << "\n=== After Second Swap (Should be the same as original) ===" << std::endl;
        std::cout << "After swapping bits at positions " << pos1 << " and " << pos2 << " again:" << std::endl;
        
        switch (bits) {
            case 8:
                print_binary<8>(final_result);
                break;
            case 16:
                print_binary<16>(final_result);
                break;
            case 32:
                print_binary<32>(final_result);
                break;
        }
        
        std::cout << "Decimal value: " << final_result << std::endl;
        
        // Verify that the final result matches the original number
        std::cout << std::endl;
        if (final_result == num) {
            std::cout << "Verification: SUCCESS - The number returned to its original value after two swaps." << std::endl;
        } else {
            std::cout << "Verification: FAILED - The number did not return to its original value after two swaps." << std::endl;
            std::cout << "This is unexpected and indicates a bug in the implementation." << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

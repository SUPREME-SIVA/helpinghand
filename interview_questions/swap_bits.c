#include <stdio.h>
#include <stdint.h>

// Function to swap 1st and 4th bits of a number
uint32_t swapBits(uint32_t num) {
    // Create masks for 1st and 4th bits
    uint32_t bit1_mask = 0x01;      // 00000001
    uint32_t bit4_mask = 0x08;      // 00001000

    // Check if 1st bit is set
    uint32_t bit1 = num & bit1_mask;
    
    // Check if 4th bit is set
    uint32_t bit4 = num & bit4_mask;

    // If both bits are same, no need to swap
    if ((bit1 && bit4) || (!bit1 && !bit4)) {
        return num;
    }

    // Toggle both bits
    num ^= bit1_mask;
    num ^= bit4_mask;

    return num;
}

int main() {
    uint32_t num = 0x12345678;  // Example number
    printf("Original number: 0x%08X\n", num);
    
    uint32_t result = swapBits(num);
    printf("After swapping 1st and 4th bits: 0x%08X\n", result);
    
    return 0;
}

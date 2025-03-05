#include <stdio.h>
#include <assert.h>

// Function to flip the nth bit of a number (from bit_flip.c)
int flipBit(int num, int position) {
    return num ^ (1 << position);
}

// Helper function to check if a specific bit is set in a number
int isBitSet(int num, int position) {
    return (num >> position) & 1;
}

// Helper function to print binary representation of a number (for debugging)
void printBinary(int num) {
    printf("Binary: ");
    for(int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

void test_flip_bit() {
    printf("Testing bit flip functionality...\n");
    
    // Test case 1: Flip 4th bit (0-based indexing) of 0
    int num1 = 0;
    int position1 = 3; // 4th bit (0-based)
    int expected1 = 8; // Binary: 1000
    int result1 = flipBit(num1, position1);
    
    assert(result1 == expected1);
    assert(isBitSet(result1, position1) == 1);
    
    // Test case 2: Flip 4th bit of a number where that bit is already set
    int num2 = 8; // Binary: 1000
    int position2 = 3; // 4th bit (0-based)
    int expected2 = 0; // Binary: 0000
    int result2 = flipBit(num2, position2);
    
    assert(result2 == expected2);
    assert(isBitSet(result2, position2) == 0);
    
    // Test case 3: Flip 0th bit (least significant bit)
    int num3 = 10; // Binary: 1010
    int position3 = 0; // 1st bit (0-based)
    int expected3 = 11; // Binary: 1011
    int result3 = flipBit(num3, position3);
    
    assert(result3 == expected3);
    assert(isBitSet(result3, position3) == 1);
    
    // Test case 4: Flip most significant bit (31st bit)
    int num4 = 0;
    int position4 = 31; // 32nd bit (0-based)
    int expected4 = (1 << 31); // Binary: 10000000 00000000 00000000 00000000
    int result4 = flipBit(num4, position4);
    
    assert(result4 == expected4);
    assert(isBitSet(result4, position4) == 1);
    
    // Test case 5: Flip multiple bits sequentially
    int num5 = 0;
    int result5 = num5;
    
    // Flip bits 0, 2, 4, 6
    for (int i = 0; i < 8; i += 2) {
        result5 = flipBit(result5, i);
    }
    
    int expected5 = 85; // Binary: 01010101
    assert(result5 == expected5);
    
    printf("All bit flip tests passed!\n");
}

int main() {
    test_flip_bit();
    
    printf("\nAll bit flip tests passed successfully!\n");
    return 0;
}

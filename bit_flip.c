#include <stdio.h>

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);
    
    // Method 1: Using XOR with bit position
    // 1 << 3 creates a number with 1 at 4th position (0-based indexing)
    // XOR (^) with this number will flip the bit at 4th position
    int result = num ^ (1 << 3);
    
    printf("Original number: %d\n", num);
    printf("Number after flipping 4th bit: %d\n", result);
    
    // To see the binary representation
    printf("Original number in binary: ");
    for(int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\nResult in binary: ");
    for(int i = 31; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf("\n");
    
    return 0;
}

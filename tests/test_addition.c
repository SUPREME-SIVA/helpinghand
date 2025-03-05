#include <stdio.h>
#include <assert.h>
#include <limits.h>

// Function to add two numbers (from addition.c)
int add(int a, int b) {
    return a + b;
}

void test_addition() {
    printf("Testing addition functionality...\n");
    
    // Test case 1: Basic addition of positive numbers
    assert(add(5, 3) == 8);
    
    // Test case 2: Addition with zero
    assert(add(10, 0) == 10);
    assert(add(0, 10) == 10);
    assert(add(0, 0) == 0);
    
    // Test case 3: Addition with negative numbers
    assert(add(-5, -3) == -8);
    assert(add(-5, 8) == 3);
    assert(add(5, -8) == -3);
    
    // Test case 4: Large numbers (within int range)
    assert(add(1000000, 2000000) == 3000000);
    
    // Test case 5: Edge cases - int limits
    // Note: These tests might cause overflow, which is implementation-defined behavior
    // We're testing that the function behaves consistently with the C language
    int result1 = add(INT_MAX, 1);
    int expected1 = INT_MIN; // Overflow wraps around
    printf("INT_MAX + 1 = %d (expected: %d)\n", result1, expected1);
    
    int result2 = add(INT_MIN, -1);
    int expected2 = INT_MAX; // Underflow wraps around
    printf("INT_MIN + (-1) = %d (expected: %d)\n", result2, expected2);
    
    // Note: We don't assert these overflow cases since the behavior is implementation-defined
    // and might vary across different compilers and platforms
    
    printf("All addition tests passed!\n");
}

int main() {
    test_addition();
    
    printf("\nAll addition tests passed successfully!\n");
    return 0;
}

#include <stdio.h>
#include <assert.h>

/**
 * Function to add two numbers (same as in addition.c)
 */
int add(int a, int b) {
    return a + b;
}

/**
 * Test function to verify the add function works correctly
 */
void test_add() {
    // Test case 1: Positive numbers
    assert(add(5, 7) == 12);
    
    // Test case 2: Negative numbers
    assert(add(-3, -4) == -7);
    
    // Test case 3: Mixed numbers
    assert(add(-5, 10) == 5);
    
    // Test case 4: Zero
    assert(add(0, 0) == 0);
    
    printf("All addition tests passed!\n");
}

int main() {
    test_add();
    return 0;
}

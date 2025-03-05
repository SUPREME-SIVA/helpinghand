#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Functions from array_rotation.c
void rotateOnePositionLeft(int arr[], int size) {
    int temp = arr[0];
    for (int i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    arr[size - 1] = temp;
}

void rotateOnePositionRight(int arr[], int size) {
    int temp = arr[size - 1];
    for (int i = size - 1; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    arr[0] = temp;
}

void rotateArray(int arr[], int size, int rotations, void (*rotateFunc)(int[], int)) {
    rotations = rotations % size; // Handle cases where rotations > size
    for (int i = 0; i < rotations; i++) {
        rotateFunc(arr, size);
    }
}

// Helper function to compare arrays
int areArraysEqual(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }
    return 1;
}

// Helper function to print an array (for debugging)
void printArray(int arr[], int size, const char* message) {
    printf("%s: ", message);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void test_rotate_one_position_left() {
    printf("Testing rotate one position left...\n");
    
    // Test case 1: Simple array
    int arr1[] = {1, 2, 3, 4, 5};
    int expected1[] = {2, 3, 4, 5, 1};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    rotateOnePositionLeft(arr1, size1);
    assert(areArraysEqual(arr1, expected1, size1));
    
    // Test case 2: Array with duplicates
    int arr2[] = {1, 2, 2, 3, 1};
    int expected2[] = {2, 2, 3, 1, 1};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    rotateOnePositionLeft(arr2, size2);
    assert(areArraysEqual(arr2, expected2, size2));
    
    // Test case 3: Single element array
    int arr3[] = {42};
    int expected3[] = {42};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    rotateOnePositionLeft(arr3, size3);
    assert(areArraysEqual(arr3, expected3, size3));
    
    printf("All rotate one position left tests passed!\n");
}

void test_rotate_one_position_right() {
    printf("Testing rotate one position right...\n");
    
    // Test case 1: Simple array
    int arr1[] = {1, 2, 3, 4, 5};
    int expected1[] = {5, 1, 2, 3, 4};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    rotateOnePositionRight(arr1, size1);
    assert(areArraysEqual(arr1, expected1, size1));
    
    // Test case 2: Array with duplicates
    int arr2[] = {1, 2, 2, 3, 1};
    int expected2[] = {1, 1, 2, 2, 3};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    rotateOnePositionRight(arr2, size2);
    assert(areArraysEqual(arr2, expected2, size2));
    
    // Test case 3: Single element array
    int arr3[] = {42};
    int expected3[] = {42};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    rotateOnePositionRight(arr3, size3);
    assert(areArraysEqual(arr3, expected3, size3));
    
    printf("All rotate one position right tests passed!\n");
}

void test_rotate_array_left() {
    printf("Testing rotate array left...\n");
    
    // Test case 1: Rotate by 2
    int arr1[] = {1, 2, 3, 4, 5};
    int expected1[] = {3, 4, 5, 1, 2};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    rotateArray(arr1, size1, 2, rotateOnePositionLeft);
    assert(areArraysEqual(arr1, expected1, size1));
    
    // Test case 2: Rotate by size (should be same as original)
    int arr2[] = {1, 2, 3, 4, 5};
    int expected2[] = {1, 2, 3, 4, 5};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    rotateArray(arr2, size2, size2, rotateOnePositionLeft);
    assert(areArraysEqual(arr2, expected2, size2));
    
    // Test case 3: Rotate by more than size
    int arr3[] = {1, 2, 3, 4, 5};
    int expected3[] = {3, 4, 5, 1, 2}; // Same as rotating by 2
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    rotateArray(arr3, size3, size3 + 2, rotateOnePositionLeft);
    assert(areArraysEqual(arr3, expected3, size3));
    
    printf("All rotate array left tests passed!\n");
}

void test_rotate_array_right() {
    printf("Testing rotate array right...\n");
    
    // Test case 1: Rotate by 2
    int arr1[] = {1, 2, 3, 4, 5};
    int expected1[] = {4, 5, 1, 2, 3};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    rotateArray(arr1, size1, 2, rotateOnePositionRight);
    assert(areArraysEqual(arr1, expected1, size1));
    
    // Test case 2: Rotate by size (should be same as original)
    int arr2[] = {1, 2, 3, 4, 5};
    int expected2[] = {1, 2, 3, 4, 5};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    rotateArray(arr2, size2, size2, rotateOnePositionRight);
    assert(areArraysEqual(arr2, expected2, size2));
    
    // Test case 3: Rotate by more than size
    int arr3[] = {1, 2, 3, 4, 5};
    int expected3[] = {4, 5, 1, 2, 3}; // Same as rotating by 2
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    rotateArray(arr3, size3, size3 + 2, rotateOnePositionRight);
    assert(areArraysEqual(arr3, expected3, size3));
    
    printf("All rotate array right tests passed!\n");
}

int main() {
    test_rotate_one_position_left();
    test_rotate_one_position_right();
    test_rotate_array_left();
    test_rotate_array_right();
    
    printf("\nAll array rotation tests passed successfully!\n");
    return 0;
}

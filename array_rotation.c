#include <stdio.h>
#include <stdlib.h>

// Single Responsibility: Each function handles one specific rotation operation
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

// Open/Closed: Generic rotation function that can be extended with different rotation strategies
void rotateArray(int arr[], int size, int rotations, void (*rotateFunc)(int[], int)) {
    rotations = rotations % size; // Handle cases where rotations > size
    for (int i = 0; i < rotations; i++) {
        rotateFunc(arr, size);
    }
}

// Interface Segregation: Separate printing functionality
void printArray(const int arr[], int size, const char* message) {
    printf("%s: ", message);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Dependency Inversion: Main logic depends on abstract rotation operation
void demonstrateRotation(int arr[], int size, int rotations, void (*rotateFunc)(int[], int), const char* direction) {
    printf("\nDemonstrating %d rotations %s:\n", rotations, direction);
    printf("Original array: ");
    printArray(arr, size, "Before rotation");
    rotateArray(arr, size, rotations, rotateFunc);
    printArray(arr, size, "After rotation");
}

int main() {
    // Example 1: Left rotation
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    printf("Example 1: Left Rotation\n");
    printf("----------------------\n");
    demonstrateRotation(arr1, size1, 4, rotateOnePositionLeft, "left");
    
    // Example 2: Right rotation
    int arr2[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    printf("\nExample 2: Right Rotation\n");
    printf("-----------------------\n");
    demonstrateRotation(arr2, size2, 4, rotateOnePositionRight, "right");
    
    return 0;
}

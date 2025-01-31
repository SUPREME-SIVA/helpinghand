#include <stdio.h>

// Function to rotate array left by given number of positions
void rotateLeft(int arr[], int size, int positions) {
    // Temporary array to store rotated elements
    int temp[size];
    
    // Calculate effective rotation positions
    positions = positions % size;
    
    // Copy elements to temporary array after rotation
    for(int i = 0; i < size; i++) {
        int newPos = (i + size - positions) % size;
        temp[newPos] = arr[i];
    }
    
    // Copy back to original array
    for(int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }
}

// Function to rotate array right by given number of positions
void rotateRight(int arr[], int size, int positions) {
    // Temporary array to store rotated elements
    int temp[size];
    
    // Calculate effective rotation positions
    positions = positions % size;
    
    // Copy elements to temporary array after rotation
    for(int i = 0; i < size; i++) {
        int newPos = (i + positions) % size;
        temp[newPos] = arr[i];
    }
    
    // Copy back to original array
    for(int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }
}

// Function to print array
void printArray(int arr[], int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    // Example 1: Left rotation
    printf("\nExample 1: Left Rotation\n");
    printf("----------------------\n");
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    printf("Original array: ");
    printArray(arr1, size1);
    
    rotateLeft(arr1, size1, 4);
    printf("After rotating left 4 times: ");
    printArray(arr1, size1);
    
    // Example 2: Right rotation
    printf("\nExample 2: Right Rotation\n");
    printf("-----------------------\n");
    int arr2[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    printf("Original array: ");
    printArray(arr2, size2);
    
    rotateRight(arr2, size2, 4);
    printf("After rotating right 4 times: ");
    printArray(arr2, size2);
    
    return 0;
}

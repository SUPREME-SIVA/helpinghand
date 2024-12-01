#include <stdio.h>
#include <string.h>

// Function to reverse string using a temporary array
void reverseString1(char str[]) {
    int length = strlen(str);
    char temp[length];
    int i;
    
    // Copy string in reverse order
    for(i = 0; i < length; i++) {
        temp[i] = str[length-1-i];
    }
    temp[length] = '\0';
    
    // Copy back to original string
    strcpy(str, temp);
}

// Function to reverse string by swapping characters
void reverseString2(char str[]) {
    int length = strlen(str);
    int i;
    char temp;
    
    for(i = 0; i < length/2; i++) {
        temp = str[i];
        str[i] = str[length-1-i];
        str[length-1-i] = temp;
    }
}

int main() {
    char str1[100], str2[100];
    
    printf("Enter a string to reverse: ");
    fgets(str1, sizeof(str1), stdin);
    str1[strcspn(str1, "\n")] = 0; // Remove newline
    
    // Copy str1 to str2 for demonstrating second method
    strcpy(str2, str1);
    
    printf("\nOriginal string: %s\n", str1);
    
    // Method 1: Using temporary array
    reverseString1(str1);
    printf("Reversed using method 1: %s\n", str1);
    
    // Method 2: Using character swapping
    reverseString2(str2);
    printf("Reversed using method 2: %s\n", str2);
    
    return 0;
}
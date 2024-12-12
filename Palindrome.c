#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to check if a number is palindrome
int isNumberPalindrome(int num) {
    int reversed = 0, original = num;
    
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    
    return original == reversed;
}

// Function to check if a string is palindrome
int isStringPalindrome(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;
    
    while (left < right) {
        // Skip non-alphanumeric characters from left
        while (left < right && !isalnum(str[left])) {
            left++;
        }
        // Skip non-alphanumeric characters from right
        while (left < right && !isalnum(str[right])) {
            right--;
        }
        
        // Compare characters (case-insensitive)
        if (tolower(str[left]) != tolower(str[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

int main() {
    // Number palindrome test
    int num;
    printf("Enter a number to check if it's a palindrome: ");
    scanf("%d", &num);
    
    if (isNumberPalindrome(num)) {
        printf("%d is a palindrome!\n", num);
    } else {
        printf("%d is not a palindrome.\n", num);
    }
    
    // Clear input buffer
    while (getchar() != '\n');
    
    // String palindrome test
    char str[100];
    printf("\nEnter a string to check if it's a palindrome: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0; // Remove newline
    
    if (isStringPalindrome(str)) {
        printf("'%s' is a palindrome!\n", str);
    } else {
        printf("'%s' is not a palindrome.\n", str);
    }
    
    return 0;
}
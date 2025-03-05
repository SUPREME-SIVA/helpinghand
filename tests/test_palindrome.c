#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// Function to check if a number is palindrome (from Palindrome.c)
int isNumberPalindrome(int num) {
    int reversed = 0, original = num;
    
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    
    return original == reversed;
}

// Function to check if a string is palindrome (from Palindrome.c)
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

void test_number_palindrome() {
    printf("Testing number palindrome function...\n");
    
    // Test case 1: Single digit (always a palindrome)
    assert(isNumberPalindrome(5) == 1);
    
    // Test case 2: Simple palindrome
    assert(isNumberPalindrome(121) == 1);
    
    // Test case 3: Larger palindrome
    assert(isNumberPalindrome(12321) == 1);
    
    // Test case 4: Non-palindrome
    assert(isNumberPalindrome(123) == 0);
    
    // Test case 5: Another non-palindrome
    assert(isNumberPalindrome(12345) == 0);
    
    printf("All number palindrome tests passed!\n");
}

void test_string_palindrome() {
    printf("Testing string palindrome function...\n");
    
    // Test case 1: Simple palindrome
    char str1[] = "level";
    assert(isStringPalindrome(str1) == 1);
    
    // Test case 2: Palindrome with spaces
    char str2[] = "A man a plan a canal Panama";
    assert(isStringPalindrome(str2) == 1);
    
    // Test case 3: Palindrome with punctuation
    char str3[] = "Madam, I'm Adam.";
    assert(isStringPalindrome(str3) == 1);
    
    // Test case 4: Non-palindrome
    char str4[] = "hello";
    assert(isStringPalindrome(str4) == 0);
    
    // Test case 5: Empty string (considered palindrome)
    char str5[] = "";
    assert(isStringPalindrome(str5) == 1);
    
    printf("All string palindrome tests passed!\n");
}

int main() {
    test_number_palindrome();
    test_string_palindrome();
    
    printf("\nAll palindrome tests passed successfully!\n");
    return 0;
}

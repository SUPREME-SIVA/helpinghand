#include <stdio.h>
#include <string.h>
#include <assert.h>

// Function to reverse string using a temporary array (from StringRevers.c)
void reverseString1(char str[]) {
    int length = strlen(str);
    char temp[length + 1]; // +1 for null terminator
    int i;
    
    // Copy string in reverse order
    for(i = 0; i < length; i++) {
        temp[i] = str[length-1-i];
    }
    temp[length] = '\0';
    
    // Copy back to original string
    strcpy(str, temp);
}

// Function to reverse string by swapping characters (from StringRevers.c)
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

// Helper function to check if a string is correctly reversed
int isCorrectlyReversed(const char* original, const char* reversed) {
    int len = strlen(original);
    for (int i = 0; i < len; i++) {
        if (original[i] != reversed[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

void test_reverse_string_method1() {
    printf("Testing string reverse method 1 (using temporary array)...\n");
    
    // Test case 1: Simple string
    char str1[] = "hello";
    char expected1[] = "olleh";
    reverseString1(str1);
    assert(strcmp(str1, expected1) == 0);
    
    // Test case 2: Palindrome
    char str2[] = "level";
    char expected2[] = "level";
    reverseString1(str2);
    assert(strcmp(str2, expected2) == 0);
    
    // Test case 3: String with spaces
    char str3[] = "hello world";
    char expected3[] = "dlrow olleh";
    reverseString1(str3);
    assert(strcmp(str3, expected3) == 0);
    
    // Test case 4: Empty string
    char str4[] = "";
    char expected4[] = "";
    reverseString1(str4);
    assert(strcmp(str4, expected4) == 0);
    
    // Test case 5: Single character
    char str5[] = "a";
    char expected5[] = "a";
    reverseString1(str5);
    assert(strcmp(str5, expected5) == 0);
    
    printf("All string reverse method 1 tests passed!\n");
}

void test_reverse_string_method2() {
    printf("Testing string reverse method 2 (using character swapping)...\n");
    
    // Test case 1: Simple string
    char str1[] = "hello";
    char expected1[] = "olleh";
    reverseString2(str1);
    assert(strcmp(str1, expected1) == 0);
    
    // Test case 2: Palindrome
    char str2[] = "level";
    char expected2[] = "level";
    reverseString2(str2);
    assert(strcmp(str2, expected2) == 0);
    
    // Test case 3: String with spaces
    char str3[] = "hello world";
    char expected3[] = "dlrow olleh";
    reverseString2(str3);
    assert(strcmp(str3, expected3) == 0);
    
    // Test case 4: Empty string
    char str4[] = "";
    char expected4[] = "";
    reverseString2(str4);
    assert(strcmp(str4, expected4) == 0);
    
    // Test case 5: Single character
    char str5[] = "a";
    char expected5[] = "a";
    reverseString2(str5);
    assert(strcmp(str5, expected5) == 0);
    
    printf("All string reverse method 2 tests passed!\n");
}

// Test that both methods produce the same results
void test_methods_equivalence() {
    printf("Testing equivalence of both string reverse methods...\n");
    
    char test_strings[][50] = {
        "hello",
        "level",
        "hello world",
        "",
        "a",
        "This is a longer test string with multiple words!"
    };
    
    for (int i = 0; i < 6; i++) {
        char str1[50], str2[50];
        strcpy(str1, test_strings[i]);
        strcpy(str2, test_strings[i]);
        
        reverseString1(str1);
        reverseString2(str2);
        
        assert(strcmp(str1, str2) == 0);
    }
    
    printf("Both methods produce equivalent results for all test cases!\n");
}

int main() {
    test_reverse_string_method1();
    test_reverse_string_method2();
    test_methods_equivalence();
    
    printf("\nAll string reverse tests passed successfully!\n");
    return 0;
}

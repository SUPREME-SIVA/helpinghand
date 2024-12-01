#include <stdio.h>
#include <string.h>

// Function to reverse individual word
void reverseWord(char *start, char *end) {
    char temp;
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

// Function to reverse words in a string
void reverseWords(char *str) {
    char *wordStart = str;
    char *temp = str;
    
    // Reverse individual words
    while (*temp) {
        temp++;
        if (*temp == '\0') {
            reverseWord(wordStart, temp - 1);
        }
        else if (*temp == ' ') {
            reverseWord(wordStart, temp - 1);
            wordStart = temp + 1;
        }
    }
    
    // Reverse the entire string
    reverseWord(str, temp - 1);
}

int main() {
    char str[1000];
    
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    
    // Remove newline character if present
    int len = strlen(str);
    if (str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }
    
    printf("Original string: %s\n", str);
    
    // Reverse the words
    reverseWords(str);
    
    printf("String with reversed words: %s\n", str);
    
    return 0;
}
/**
 * @file substring_search.c
 * @brief Implementation of efficient substring search algorithms
 *
 * This file demonstrates various approaches to finding and counting occurrences
 * of a substring within a larger string, with a focus on optimization and proper
 * memory management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief Find all occurrences of a substring in a string (naive approach)
 *
 * Time Complexity: O(n*m) where n is the length of text and m is the length of pattern
 * Space Complexity: O(1) constant extra space
 *
 * @param text The text to search in
 * @param pattern The substring to search for
 * @param count Pointer to store the count of occurrences
 * @return Dynamically allocated array of positions (caller must free)
 */
int* naive_substring_search(const char* text, const char* pattern, int* count) {
    if (!text || !pattern || !count) {
        return NULL;
    }
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    // Handle edge cases
    if (pattern_len == 0 || pattern_len > text_len) {
        *count = 0;
        return NULL;
    }
    
    // Allocate initial space for results (resize if needed)
    int capacity = 10;
    int* positions = (int*)malloc(capacity * sizeof(int));
    if (!positions) {
        *count = 0;
        return NULL;
    }
    
    *count = 0;
    
    // Naive sliding window search
    for (int i = 0; i <= text_len - pattern_len; i++) {
        bool match = true;
        
        // Check if pattern matches at current position
        for (int j = 0; j < pattern_len; j++) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            // Expand array if needed
            if (*count >= capacity) {
                capacity *= 2;
                int* temp = (int*)realloc(positions, capacity * sizeof(int));
                if (!temp) {
                    // Handle reallocation failure
                    free(positions);
                    *count = 0;
                    return NULL;
                }
                positions = temp;
            }
            
            positions[(*count)++] = i;
        }
    }
    
    return positions;
}

/**
 * @brief KMP (Knuth-Morris-Pratt) substring search algorithm
 *
 * An efficient substring search with O(n+m) time complexity.
 * 
 * Time Complexity: O(n+m) where n is the length of text and m is the length of pattern
 * Space Complexity: O(m) for the LPS (Longest Prefix Suffix) array
 *
 * @param text The text to search in
 * @param pattern The substring to search for
 * @param count Pointer to store the count of occurrences
 * @return Dynamically allocated array of positions (caller must free)
 */
int* kmp_substring_search(const char* text, const char* pattern, int* count) {
    if (!text || !pattern || !count) {
        return NULL;
    }
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    // Handle edge cases
    if (pattern_len == 0 || pattern_len > text_len) {
        *count = 0;
        return NULL;
    }
    
    // Allocate initial space for results (resize if needed)
    int capacity = 10;
    int* positions = (int*)malloc(capacity * sizeof(int));
    if (!positions) {
        *count = 0;
        return NULL;
    }
    
    *count = 0;
    
    // Compute LPS (Longest Prefix Suffix) array for the pattern
    int* lps = (int*)calloc(pattern_len, sizeof(int));
    if (!lps) {
        free(positions);
        *count = 0;
        return NULL;
    }
    
    // Preprocess the pattern
    int len = 0;  // Length of previous longest prefix suffix
    int i = 1;
    
    while (i < pattern_len) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
    }
    
    // Search for pattern using KMP algorithm
    i = 0;  // Index for text
    int j = 0;  // Index for pattern
    
    while (i < text_len) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        
        if (j == pattern_len) {
            // Found a match
            if (*count >= capacity) {
                capacity *= 2;
                int* temp = (int*)realloc(positions, capacity * sizeof(int));
                if (!temp) {
                    // Handle reallocation failure
                    free(positions);
                    free(lps);
                    *count = 0;
                    return NULL;
                }
                positions = temp;
            }
            
            positions[(*count)++] = i - j;
            j = lps[j - 1];
        } else if (i < text_len && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    free(lps);  // Clean up the LPS array
    return positions;
}

/**
 * @brief Demonstrate and compare substring search algorithms
 */
void benchmark_substring_search(const char* text, const char* pattern) {
    printf("Text: \"%s\"\n", text);
    printf("Pattern: \"%s\"\n", pattern);
    
    // Benchmark naive approach
    int count_naive = 0;
    clock_t start = clock();
    int* positions_naive = naive_substring_search(text, pattern, &count_naive);
    clock_t end = clock();
    double time_naive = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nNaive Approach:\n");
    printf("Found %d occurrences in %.3f ms\n", count_naive, time_naive);
    if (count_naive > 0) {
        printf("Positions: ");
        for (int i = 0; i < count_naive; i++) {
            printf("%d%s", positions_naive[i], (i < count_naive - 1) ? ", " : "");
        }
        printf("\n");
    }
    
    // Benchmark KMP approach
    int count_kmp = 0;
    start = clock();
    int* positions_kmp = kmp_substring_search(text, pattern, &count_kmp);
    end = clock();
    double time_kmp = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    printf("\nKMP Approach:\n");
    printf("Found %d occurrences in %.3f ms\n", count_kmp, time_kmp);
    if (count_kmp > 0) {
        printf("Positions: ");
        for (int i = 0; i < count_kmp; i++) {
            printf("%d%s", positions_kmp[i], (i < count_kmp - 1) ? ", " : "");
        }
        printf("\n");
    }
    
    // Compare results
    if (time_naive > 0 && time_kmp > 0) {
        printf("\nKMP is %.2fx %s than naive approach\n", 
            (time_naive > time_kmp) ? (time_naive / time_kmp) : (time_kmp / time_naive),
            (time_naive > time_kmp) ? "faster" : "slower");
    }
    
    // Clean up
    free(positions_naive);
    free(positions_kmp);
}

/**
 * @brief The entry point of the program
 */
int main() {
    printf("=== C Substring Search Algorithms ===\n\n");
    
    // Test case 1: Multiple occurrences
    benchmark_substring_search(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        "or"
    );
    
    printf("\n---\n");
    
    // Test case 2: Edge case with no occurrences
    benchmark_substring_search(
        "The quick brown fox jumps over the lazy dog",
        "zebra"
    );
    
    printf("\n---\n");
    
    // Test case 3: Pattern with repeating characters (KMP advantage)
    benchmark_substring_search(
        "ABABDABACDABABCABAB",
        "ABABCABAB"
    );
    
    return 0;
}

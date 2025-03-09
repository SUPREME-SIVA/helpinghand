/**
 * @file substring_search.cpp
 * @brief Modern C++ implementation of efficient substring search algorithms
 *
 * This file demonstrates various approaches to finding and counting occurrences
 * of a substring within a larger string, with a focus on optimization, proper
 * memory management, and modern C++ practices.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <memory>

/**
 * @brief Namespace containing substring search algorithms
 */
namespace substring {

/**
 * @brief Find all occurrences of a substring in a string (naive approach)
 *
 * Time Complexity: O(n*m) where n is the length of text and m is the length of pattern
 * Space Complexity: O(k) where k is the number of occurrences
 *
 * @tparam StringType Type of string (std::string, std::string_view, etc.)
 * @param text The text to search in
 * @param pattern The substring to search for
 * @return Vector of positions where pattern occurs in text
 */
template <typename StringType>
std::vector<size_t> naive_search(const StringType& text, const StringType& pattern) {
    std::vector<size_t> positions;
    
    // Handle edge cases
    if (pattern.empty() || pattern.size() > text.size()) {
        return positions;
    }
    
    // Naive sliding window search
    for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
        bool match = true;
        
        // Check if pattern matches at current position
        for (size_t j = 0; j < pattern.size(); ++j) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            positions.push_back(i);
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
 * Space Complexity: O(m+k) where m is the pattern size and k is the number of occurrences
 *
 * @tparam StringType Type of string (std::string, std::string_view, etc.)
 * @param text The text to search in
 * @param pattern The substring to search for
 * @return Vector of positions where pattern occurs in text
 */
template <typename StringType>
std::vector<size_t> kmp_search(const StringType& text, const StringType& pattern) {
    std::vector<size_t> positions;
    
    // Handle edge cases
    if (pattern.empty() || pattern.size() > text.size()) {
        return positions;
    }
    
    const size_t pattern_len = pattern.size();
    const size_t text_len = text.size();
    
    // Compute LPS (Longest Prefix Suffix) array for the pattern
    std::vector<size_t> lps(pattern_len, 0);
    
    // Preprocess the pattern
    size_t len = 0;  // Length of previous longest prefix suffix
    size_t i = 1;
    
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
    size_t j = 0;  // Index for pattern
    
    while (i < text_len) {
        if (pattern[j] == text[i]) {
            ++i;
            ++j;
        }
        
        if (j == pattern_len) {
            // Found a match
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < text_len && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }
    
    return positions;
}

/**
 * @brief Boyer-Moore substring search algorithm
 *
 * An efficient substring search that skips characters based on bad character and good suffix rules.
 * This is a simplified implementation focusing on the bad character rule.
 * 
 * Time Complexity: O(n+m) average and best case, O(n*m) worst case
 * Space Complexity: O(k+256) where k is the number of occurrences
 *
 * @tparam StringType Type of string (std::string, std::string_view, etc.)
 * @param text The text to search in
 * @param pattern The substring to search for
 * @return Vector of positions where pattern occurs in text
 */
template <typename StringType>
std::vector<size_t> boyer_moore_search(const StringType& text, const StringType& pattern) {
    std::vector<size_t> positions;
    
    // Handle edge cases
    if (pattern.empty() || pattern.size() > text.size()) {
        return positions;
    }
    
    const size_t pattern_len = pattern.size();
    const size_t text_len = text.size();
    
    // Preprocess: Build bad character table
    // For each character, store its rightmost position in the pattern
    std::vector<int> bad_char(256, -1);
    
    for (size_t i = 0; i < pattern_len; ++i) {
        bad_char[static_cast<unsigned char>(pattern[i])] = static_cast<int>(i);
    }
    
    // Search using Boyer-Moore algorithm (bad character rule)
    size_t shift = 0;
    
    while (shift <= text_len - pattern_len) {
        int j = static_cast<int>(pattern_len) - 1;
        
        // Match pattern from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            --j;
        }
        
        if (j < 0) {
            // Pattern found
            positions.push_back(shift);
            
            // Move to the next possible match
            shift += (shift + pattern_len < text_len) ? 
                     pattern_len - bad_char[static_cast<unsigned char>(text[shift + pattern_len])] : 1;
        } else {
            // Calculate shift using bad character rule
            int bc_shift = j - bad_char[static_cast<unsigned char>(text[shift + j])];
            shift += std::max(1, bc_shift);
        }
    }
    
    return positions;
}

/**
 * @brief Rabin-Karp substring search algorithm using rolling hash
 *
 * Uses a hash function to find matches efficiently.
 * 
 * Time Complexity: O(n+m) average case, O(n*m) worst case
 * Space Complexity: O(k) where k is the number of occurrences
 *
 * @tparam StringType Type of string (std::string, std::string_view, etc.)
 * @param text The text to search in
 * @param pattern The substring to search for
 * @return Vector of positions where pattern occurs in text
 */
template <typename StringType>
std::vector<size_t> rabin_karp_search(const StringType& text, const StringType& pattern) {
    std::vector<size_t> positions;
    
    // Handle edge cases
    if (pattern.empty() || pattern.size() > text.size()) {
        return positions;
    }
    
    const size_t pattern_len = pattern.size();
    const size_t text_len = text.size();
    
    // Parameters for hash calculation
    const size_t prime = 101;  // A prime number
    const size_t base = 256;   // Number of possible characters
    
    // Calculate hash value for pattern and first window of text
    size_t pattern_hash = 0;
    size_t text_hash = 0;
    size_t h = 1;  // h = base^(pattern_len-1) % prime
    
    // Calculate h
    for (size_t i = 0; i < pattern_len - 1; ++i) {
        h = (h * base) % prime;
    }
    
    // Calculate initial hash values
    for (size_t i = 0; i < pattern_len; ++i) {
        pattern_hash = (base * pattern_hash + static_cast<unsigned char>(pattern[i])) % prime;
        text_hash = (base * text_hash + static_cast<unsigned char>(text[i])) % prime;
    }
    
    // Slide the pattern over text one by one
    for (size_t i = 0; i <= text_len - pattern_len; ++i) {
        // Check if hash values match
        if (pattern_hash == text_hash) {
            // Verify character by character
            bool match = true;
            for (size_t j = 0; j < pattern_len; ++j) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                positions.push_back(i);
            }
        }
        
        // Calculate hash for next window
        if (i < text_len - pattern_len) {
            // Remove leading digit, add trailing digit
            text_hash = (base * (text_hash - static_cast<unsigned char>(text[i]) * h) + 
                        static_cast<unsigned char>(text[i + pattern_len])) % prime;
            
            // Handle negative hash values
            if (text_hash < 0) {
                text_hash += prime;
            }
        }
    }
    
    return positions;
}

} // namespace substring

/**
 * @brief Benchmark a substring search algorithm
 * 
 * @tparam Func Type of search function
 * @param text The text to search in
 * @param pattern The substring to search for
 * @param search_func The search function to benchmark
 * @param name The name of the algorithm
 */
template <typename Func>
void benchmark_search(const std::string& text, const std::string& pattern, 
                     Func search_func, const std::string& name) {
    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    auto positions = search_func(text, pattern);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Print results
    std::cout << name << ":\n";
    std::cout << "  Found " << positions.size() << " occurrences in " 
              << duration.count() << " μs\n";
    
    if (!positions.empty() && positions.size() <= 10) {
        std::cout << "  Positions: ";
        for (size_t i = 0; i < positions.size(); ++i) {
            std::cout << positions[i];
            if (i < positions.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    
    std::cout << std::endl;
}

/**
 * @brief The entry point of the program
 */
int main() {
    std::cout << "=== C++ Substring Search Algorithms ===\n\n";
    
    // Test cases
    struct TestCase {
        std::string text;
        std::string pattern;
        std::string description;
    };
    
    std::vector<TestCase> test_cases = {
        {
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
            "or",
            "Multiple occurrences"
        },
        {
            "The quick brown fox jumps over the lazy dog",
            "zebra",
            "No occurrences"
        },
        {
            "ABABDABACDABABCABAB",
            "ABABCABAB",
            "Pattern with repeating characters"
        },
        {
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaa",
            "Repeated characters (worst case for naive)"
        }
    };
    
    // Run benchmarks for each test case
    for (const auto& test : test_cases) {
        std::cout << "Test: " << test.description << "\n";
        std::cout << "Text: \"" << test.text << "\"\n";
        std::cout << "Pattern: \"" << test.pattern << "\"\n\n";
        
        // Benchmark each algorithm
        benchmark_search(test.text, test.pattern, 
                        substring::naive_search<std::string>, "Naive Search");
        
        benchmark_search(test.text, test.pattern, 
                        substring::kmp_search<std::string>, "KMP Search");
        
        benchmark_search(test.text, test.pattern, 
                        substring::boyer_moore_search<std::string>, "Boyer-Moore Search");
        
        benchmark_search(test.text, test.pattern, 
                        substring::rabin_karp_search<std::string>, "Rabin-Karp Search");
        
        std::cout << "---\n\n";
    }
    
    // Test with a larger text
    std::string large_text;
    large_text.reserve(10000);
    
    // Generate a larger text
    const std::string lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                             "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ";
    
    while (large_text.size() < 10000) {
        large_text += lorem;
    }
    
    std::cout << "Large Text Test (Size: " << large_text.size() << " characters)\n";
    std::cout << "Pattern: \"dolor\"\n\n";
    
    // Benchmark with large text
    benchmark_search(large_text, "dolor", 
                    substring::naive_search<std::string>, "Naive Search");
    
    benchmark_search(large_text, "dolor", 
                    substring::kmp_search<std::string>, "KMP Search");
    
    benchmark_search(large_text, "dolor", 
                    substring::boyer_moore_search<std::string>, "Boyer-Moore Search");
    
    benchmark_search(large_text, "dolor", 
                    substring::rabin_karp_search<std::string>, "Rabin-Karp Search");
    
    return 0;
}

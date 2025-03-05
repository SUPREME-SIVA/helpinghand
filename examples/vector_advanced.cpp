#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    // Initialize vector with values
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Sorting a vector
    std::sort(numbers.begin(), numbers.end());
    
    std::cout << "Sorted vector: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Reverse the vector
    std::reverse(numbers.begin(), numbers.end());
    
    std::cout << "Reversed vector: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Finding elements
    auto it = std::find(numbers.begin(), numbers.end(), 4);
    if (it != numbers.end()) {
        std::cout << "Found 4 at position: " << (it - numbers.begin()) << std::endl;
    }
    
    // Sum of all elements
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum of all elements: " << sum << std::endl;
    
    // Vector of vectors (2D vector)
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    std::cout << "\nMatrix:" << std::endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    
    // Inserting elements at specific positions
    std::vector<int> data = {10, 20, 30, 40, 50};
    
    // Insert 25 after 20
    auto pos = std::find(data.begin(), data.end(), 20);
    if (pos != data.end()) {
        data.insert(pos + 1, 25);
    }
    
    std::cout << "\nAfter insertion: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // Removing elements that satisfy a condition (all even numbers)
    data.erase(
        std::remove_if(data.begin(), data.end(), 
                      [](int x) { return x % 2 == 0; }),
        data.end()
    );
    
    std::cout << "After removing even numbers: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // Vector with custom initial size and value
    std::vector<int> repeated(5, 10);  // 5 elements, all with value 10
    
    std::cout << "\nVector with repeated values: ";
    for (int val : repeated) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    return 0;
}

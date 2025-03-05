#include <iostream>
#include <list>
#include <string>
#include <algorithm>

// Example 1: Basic List Operations
// This demonstrates common list operations and shows why lists
// are useful for frequent insertions and deletions

int main() {
    // Create an empty list of integers
    std::list<int> numbers;
    
    // Add elements to the back (similar to push_back in vector)
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);
    
    // Add elements to the front - O(1) operation
    // This would be O(n) for vectors but is efficient for lists
    numbers.push_front(5);
    numbers.push_front(1);
    
    // Display the list elements
    std::cout << "List after push_front and push_back operations:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Insert an element in the middle
    auto it = std::find(numbers.begin(), numbers.end(), 10);
    if (it != numbers.end()) {
        // Inserting before position 10, which is efficient in a list - O(1)
        numbers.insert(it, 7);
    }
    
    std::cout << "List after inserting 7 before 10:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Remove an element from the middle - O(1) after finding the position
    it = std::find(numbers.begin(), numbers.end(), 20);
    if (it != numbers.end()) {
        // Removing from middle, efficient in list - O(1)
        numbers.erase(it);
    }
    
    std::cout << "List after removing 20:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Demonstrate splicing (transferring elements from one list to another)
    std::list<int> otherList = {100, 200, 300};
    
    // Splice entire otherList into numbers at position of 30
    it = std::find(numbers.begin(), numbers.end(), 30);
    if (it != numbers.end()) {
        // Splicing is highly efficient with lists - O(1)
        numbers.splice(it, otherList);
    }
    
    std::cout << "List after splicing in {100, 200, 300} before 30:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    std::cout << "otherList after splicing (should be empty):" << std::endl;
    for (const auto& num : otherList) {
        std::cout << num << " ";
    }
    std::cout << (otherList.empty() ? "empty" : "not empty") << std::endl;
    
    // Sort the list - O(n log n) but involves expensive pointer adjustments
    numbers.sort();
    
    std::cout << "List after sorting:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Size and other operations
    std::cout << "List size: " << numbers.size() << std::endl;
    std::cout << "First element: " << numbers.front() << std::endl;
    std::cout << "Last element: " << numbers.back() << std::endl;
    
    // Clear the list
    numbers.clear();
    std::cout << "List size after clear(): " << numbers.size() << std::endl;
    
    return 0;
}

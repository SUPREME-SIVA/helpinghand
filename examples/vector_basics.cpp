#include <iostream>
#include <vector>

int main() {
    // Creating a vector
    std::vector<int> numbers;
    
    // Adding elements to the vector
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);
    
    // Accessing elements
    std::cout << "First element: " << numbers[0] << std::endl;
    std::cout << "Second element: " << numbers[1] << std::endl;
    std::cout << "Third element: " << numbers[2] << std::endl;
    
    // Size of the vector
    std::cout << "Vector size: " << numbers.size() << std::endl;
    
    // Iterating through the vector
    std::cout << "Vector elements: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Modifying elements
    numbers[1] = 25;
    std::cout << "After modification: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Removing the last element
    numbers.pop_back();
    std::cout << "After pop_back: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Clearing the vector
    numbers.clear();
    std::cout << "Vector size after clear: " << numbers.size() << std::endl;
    
    return 0;
}

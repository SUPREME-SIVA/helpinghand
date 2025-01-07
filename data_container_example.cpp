#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

using namespace std::string_literals;  // For string literal "s"

template<typename T>
class DataContainer {
    std::vector<T> data;
    std::string label;

public:
    // Constructor that takes initial data
    DataContainer(std::vector<T> initial_data, std::string name)
        : data(std::move(initial_data)), label(std::move(name)) {}

    // Add data to container
    void add(const T& value) {
        data.push_back(value);
    }

    // Print statistics about the data
    void printStats() const {
        std::cout << "Statistics for " << label << ":\n";
        std::cout << "Number of elements: " << data.size() << "\n";
        
        if (data.empty()) return;

        // Calculate sum and average for numeric types
        if constexpr (std::is_arithmetic_v<T>) {
            T sum = T();
            for (const auto& val : data) {
                sum += val;
            }
            std::cout << "Sum: " << sum << "\n";
            std::cout << "Average: " << sum / data.size() << "\n";
        }
        
        std::cout << "First element: " << data.front() << "\n";
        std::cout << "Last element: " << data.back() << "\n\n";
    }
};

int main() {
    // C++17 style - Template argument deduction
    std::cout << "C++17 Style:\n";
    DataContainer temperatures(std::vector{23.5, 24.0, 22.8, 25.2, 23.1}, "Temperature Readings");
    DataContainer names(std::vector{"Alice"s, "Bob"s, "Charlie"s}, "Student Names");
    
    // C++11/14 style - Explicit template arguments required
    std::cout << "C++11/14 Style:\n";
    DataContainer<double> old_temperatures({23.5, 24.0, 22.8, 25.2, 23.1}, "Temperature Readings");
    DataContainer<std::string> old_names(std::vector<std::string>{"Alice", "Bob", "Charlie"}, "Student Names");
    
    // Using both containers
    temperatures.add(24.5);
    names.add("David");
    
    // Print statistics
    temperatures.printStats();
    names.printStats();
    
    return 0;
}

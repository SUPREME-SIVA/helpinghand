#include <iostream>
#include <map>
#include <string>
#include <type_traits>

// Class representing a user database
class UserDatabase {
    std::map<int, std::string> users;

public:
    UserDatabase() {
        users[1] = "John";
        users[2] = "Alice";
        users[3] = "Bob";
    }

    // C++17 style with if initialization
    void findUserModern(int id) {
        std::cout << "\nC++17 approach:" << std::endl;
        if (auto iter = users.find(id); iter != users.end()) {
            std::cout << "User found: " << iter->second << std::endl;
        } else {
            std::cout << "User not found!" << std::endl;
        }
        // iter is not accessible here - better scope control
    }

    // C++11/14 style
    void findUserLegacy(int id) {
        std::cout << "\nC++11/14 approach:" << std::endl;
        auto iter = users.find(id);  // Variable declared outside if
        if (iter != users.end()) {
            std::cout << "User found: " << iter->second << std::endl;
        } else {
            std::cout << "User not found!" << std::endl;
        }
        // iter is still accessible here - potential scope pollution
    }
};

// Generic printer class that handles different types at compile-time
template<typename T>
class SmartPrinter {
public:
    void print(const T& value) {
        if constexpr (std::is_arithmetic_v<T>) {
            std::cout << "Numeric value: " << value << std::endl;
            // Compile-time check for floating point
            if constexpr (std::is_floating_point_v<T>) {
                std::cout << "With precision: " << value * 1.0 << std::endl;
            }
        } 
        else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "String value: \"" << value << "\"" << std::endl;
        }
        else {
            std::cout << "Unsupported type" << std::endl;
        }
    }
};

int main() {
    UserDatabase db;
    
    // Test with existing user
    db.findUserModern(1);
    db.findUserLegacy(1);
    
    // Test with non-existing user
    db.findUserModern(5);
    db.findUserLegacy(5);

    SmartPrinter<int> intPrinter;
    SmartPrinter<double> doublePrinter;
    SmartPrinter<std::string> stringPrinter;

    intPrinter.print(42);
    doublePrinter.print(3.14159);
    stringPrinter.print("Hello, constexpr if!");

    return 0;
}

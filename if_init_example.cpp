#include <iostream>
#include <map>
#include <string>

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

int main() {
    UserDatabase db;
    
    // Test with existing user
    db.findUserModern(1);
    db.findUserLegacy(1);
    
    // Test with non-existing user
    db.findUserModern(5);
    db.findUserLegacy(5);
    
    return 0;
}

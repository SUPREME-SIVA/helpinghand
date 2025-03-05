#include <iostream>
#include <vector>
#include <string>

// Custom class
class Student {
private:
    std::string name;
    int id;
    float gpa;

public:
    // Constructor
    Student(std::string n, int i, float g) : name(n), id(i), gpa(g) {}
    
    // Display method
    void display() const {
        std::cout << "Name: " << name << ", ID: " << id << ", GPA: " << gpa << std::endl;
    }
    
    // Getter for GPA to use in sorting
    float getGPA() const {
        return gpa;
    }
};

int main() {
    // Creating a vector of Student objects
    std::vector<Student> students;
    
    // Adding students to the vector
    students.push_back(Student("Alice", 1001, 3.8));
    students.push_back(Student("Bob", 1002, 3.5));
    students.push_back(Student("Charlie", 1003, 3.9));
    
    // Displaying all students
    std::cout << "All students:" << std::endl;
    for (const Student& student : students) {
        student.display();
    }
    
    // Using vector capacity and reserve
    std::cout << "\nVector size: " << students.size() << std::endl;
    std::cout << "Vector capacity: " << students.capacity() << std::endl;
    
    // Reserving space for more students
    students.reserve(10);
    std::cout << "After reserve(10), capacity: " << students.capacity() << std::endl;
    
    // Adding more students
    students.push_back(Student("David", 1004, 3.7));
    students.push_back(Student("Eve", 1005, 3.6));
    
    // Using emplace_back for in-place construction
    students.emplace_back("Frank", 1006, 3.4);
    
    std::cout << "\nAfter adding more students:" << std::endl;
    for (const Student& student : students) {
        student.display();
    }
    
    // Accessing elements using at() with bounds checking
    try {
        std::cout << "\nAccessing student at index 2:" << std::endl;
        students.at(2).display();
        
        std::cout << "Trying to access out of bounds:" << std::endl;
        students.at(10).display();  // This will throw an exception
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    return 0;
}

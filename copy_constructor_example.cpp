#include <iostream>
#include <string>

class Student {
private:
    std::string* name;
    int* age;
    double* gpa;

public:
    // Regular constructor
    Student(const std::string& studentName, int studentAge, double studentGpa) {
        name = new std::string(studentName);
        age = new int(studentAge);
        gpa = new double(studentGpa);
    }

    // Copy constructor - performs deep copy
    Student(const Student& other) {
        // Allocate new memory and copy the values
        name = new std::string(*other.name);
        age = new int(*other.age);
        gpa = new double(*other.gpa);
    }

    // Destructor to free allocated memory
    ~Student() {
        delete name;
        delete age;
        delete gpa;
    }

    // Assignment operator
    Student& operator=(const Student& other) {
        if (this != &other) {  // Check for self-assignment
            // Delete existing memory
            delete name;
            delete age;
            delete gpa;

            // Allocate new memory and copy values
            name = new std::string(*other.name);
            age = new int(*other.age);
            gpa = new double(*other.gpa);
        }
        return *this;
    }

    // Method to display student information
    void displayInfo() const {
        std::cout << "Name: " << *name << std::endl;
        std::cout << "Age: " << *age << std::endl;
        std::cout << "GPA: " << *gpa << std::endl;
    }
};

int main() {
    // Create a student using regular constructor
    Student student1("John Doe", 20, 3.8);
    std::cout << "Student 1 Information:" << std::endl;
    student1.displayInfo();

    // Create a second student using copy constructor
    Student student2 = student1;  // Copy constructor is called
    std::cout << "\nStudent 2 Information (after copy):" << std::endl;
    student2.displayInfo();

    // Create a third student and use assignment operator
    Student student3("Jane Smith", 22, 3.9);
    student3 = student1;  // Assignment operator is called
    std::cout << "\nStudent 3 Information (after assignment):" << std::endl;
    student3.displayInfo();

    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Custom class to demonstrate vector with objects
class Student {
    string name;
    int age;
public:
    Student(string n, int a) : name(n), age(a) {}
    string getName() const { return name; }
};

// Function to demonstrate vector basics
void demonstrateVectorBasics() {
    cout << "\n1. Vector Basics:" << endl;
    cout << "---------------" << endl;
    
    vector<int> numbers;
    
    cout << "Initial capacity: " << numbers.capacity() << endl;
    cout << "Initial size: " << numbers.size() << endl;
    cout << "Maximum theoretical size: " << numbers.max_size() << endl;
    
    // Adding elements and watching capacity growth
    cout << "\nAdding elements and observing capacity growth:" << endl;
    for(int i = 0; i < 10; i++) {
        numbers.push_back(i);
        cout << "Size: " << numbers.size() 
             << ", Capacity: " << numbers.capacity() << endl;
    }
}

// Function to demonstrate vector with reserve
void demonstrateVectorReserve() {
    cout << "\n2. Vector with Reserve:" << endl;
    cout << "--------------------" << endl;
    
    vector<int> numbers;
    numbers.reserve(10);  // Pre-allocate space for 10 elements
    
    cout << "After reserve(10):" << endl;
    cout << "Capacity: " << numbers.capacity() << endl;
    cout << "Size: " << numbers.size() << endl;
}

// Function to demonstrate vector resize
void demonstrateVectorResize() {
    cout << "\n3. Vector Resize:" << endl;
    cout << "---------------" << endl;
    
    vector<int> numbers(5, 1);  // Initialize with 5 elements of value 1
    cout << "Initial size: " << numbers.size() << endl;
    
    numbers.resize(10);  // Resize to 10 elements
    cout << "After resize(10), size: " << numbers.size() << endl;
    
    numbers.resize(3);   // Shrink to 3 elements
    cout << "After resize(3), size: " << numbers.size() << endl;
}

// Function to demonstrate common vector operations
void demonstrateVectorOperations() {
    cout << "\n4. Common Vector Operations:" << endl;
    cout << "-------------------------" << endl;
    
    vector<Student> students;
    
    // Adding elements
    students.push_back(Student("Alice", 20));
    students.push_back(Student("Bob", 22));
    
    // Accessing elements
    cout << "First student: " << students[0].getName() << endl;
    cout << "Second student: " << students.at(1).getName() << endl;
    
    // Remove last element
    students.pop_back();
    cout << "After pop_back(), size: " << students.size() << endl;
    
    // Clear the vector
    students.clear();
    cout << "After clear(), size: " << students.size() << endl;
}

// Function to demonstrate vector best practices
void demonstrateVectorBestPractices() {
    cout << "\n5. Vector Best Practices:" << endl;
    cout << "----------------------" << endl;
    
    // Bad practice: Not reserving space when size is known
    vector<int> withoutReserve;
    cout << "Without reserve - ";
    for(int i = 0; i < 1000; i++) {
        withoutReserve.push_back(i);
        if(i == 999) {
            cout << "Final capacity: " << withoutReserve.capacity() << endl;
        }
    }
    
    // Good practice: Reserve space when size is known
    vector<int> withReserve;
    withReserve.reserve(1000);
    cout << "With reserve - ";
    for(int i = 0; i < 1000; i++) {
        withReserve.push_back(i);
        if(i == 999) {
            cout << "Final capacity: " << withReserve.capacity() << endl;
        }
    }
}

int main() {
    cout << "Vector Capacity and Usage Demo" << endl;
    cout << "============================" << endl;

    demonstrateVectorBasics();
    demonstrateVectorReserve();
    demonstrateVectorResize();
    demonstrateVectorOperations();
    demonstrateVectorBestPractices();

    cout << "\nWhen to Use Vector:" << endl;
    cout << "1. Dynamic size requirements" << endl;
    cout << "2. Frequent insertions/deletions at the end" << endl;
    cout << "3. Random access to elements needed" << endl;
    cout << "4. Contiguous memory storage required" << endl;
    cout << "5. Automatic memory management desired" << endl;

    cout << "\nWhen NOT to Use Vector:" << endl;
    cout << "1. Fixed size known at compile time (use array)" << endl;
    cout << "2. Frequent insertions/deletions at beginning/middle (use list)" << endl;
    cout << "3. Memory is extremely constrained" << endl;
    cout << "4. Non-contiguous storage needed" << endl;

    return 0;
}

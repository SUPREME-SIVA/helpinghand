#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>

// Redirect cout for testing
class CoutRedirect {
private:
    std::streambuf* old_buffer;
    std::ostringstream stream_buffer;

public:
    CoutRedirect() {
        old_buffer = std::cout.rdbuf();
        std::cout.rdbuf(stream_buffer.rdbuf());
    }

    ~CoutRedirect() {
        std::cout.rdbuf(old_buffer);
    }

    std::string get_output() {
        return stream_buffer.str();
    }
};

// Student class from vector_capacity_demo.cpp
class Student {
    std::string name;
    int age;
public:
    Student(std::string n, int a) : name(n), age(a) {}
    std::string getName() const { return name; }
    int getAge() const { return age; }
};

// Test functions
void test_vector_basics() {
    std::cout << "Testing vector basics..." << std::endl;
    
    std::vector<int> numbers;
    
    // Test initial state
    assert(numbers.size() == 0);
    assert(numbers.capacity() >= 0);
    
    // Test adding elements
    numbers.push_back(1);
    assert(numbers.size() == 1);
    assert(numbers.capacity() >= 1);
    
    numbers.push_back(2);
    assert(numbers.size() == 2);
    assert(numbers.capacity() >= 2);
    
    // Test element access
    assert(numbers[0] == 1);
    assert(numbers[1] == 2);
    
    std::cout << "Vector basics test passed!" << std::endl;
}

void test_vector_reserve() {
    std::cout << "Testing vector reserve..." << std::endl;
    
    std::vector<int> numbers;
    size_t initial_capacity = numbers.capacity();
    
    numbers.reserve(20);
    assert(numbers.capacity() >= 20);
    assert(numbers.size() == 0);  // Size should not change
    
    // Adding elements should not change capacity until we exceed 20
    for (int i = 0; i < 20; i++) {
        numbers.push_back(i);
    }
    assert(numbers.size() == 20);
    assert(numbers.capacity() >= 20);
    
    std::cout << "Vector reserve test passed!" << std::endl;
}

void test_vector_resize() {
    std::cout << "Testing vector resize..." << std::endl;
    
    std::vector<int> numbers(5, 10);  // 5 elements with value 10
    assert(numbers.size() == 5);
    
    // Test resize to larger size
    numbers.resize(10);
    assert(numbers.size() == 10);
    
    // First 5 elements should still be 10
    for (int i = 0; i < 5; i++) {
        assert(numbers[i] == 10);
    }
    
    // Next 5 elements should be default-initialized (0 for int)
    for (int i = 5; i < 10; i++) {
        assert(numbers[i] == 0);
    }
    
    // Test resize to smaller size
    numbers.resize(3);
    assert(numbers.size() == 3);
    
    // All 3 remaining elements should be 10
    for (int i = 0; i < 3; i++) {
        assert(numbers[i] == 10);
    }
    
    std::cout << "Vector resize test passed!" << std::endl;
}

void test_vector_operations() {
    std::cout << "Testing vector operations..." << std::endl;
    
    std::vector<Student> students;
    
    // Test push_back
    students.push_back(Student("Alice", 20));
    students.push_back(Student("Bob", 22));
    assert(students.size() == 2);
    
    // Test element access
    assert(students[0].getName() == "Alice");
    assert(students[0].getAge() == 20);
    assert(students[1].getName() == "Bob");
    assert(students[1].getAge() == 22);
    
    // Test at() method
    assert(students.at(0).getName() == "Alice");
    assert(students.at(1).getName() == "Bob");
    
    // Test pop_back
    students.pop_back();
    assert(students.size() == 1);
    assert(students[0].getName() == "Alice");
    
    // Test clear
    students.clear();
    assert(students.size() == 0);
    assert(students.empty());
    
    std::cout << "Vector operations test passed!" << std::endl;
}

void test_vector_capacity_growth() {
    std::cout << "Testing vector capacity growth..." << std::endl;
    
    std::vector<int> numbers;
    size_t last_capacity = numbers.capacity();
    
    // Add elements and verify capacity grows
    for (int i = 0; i < 100; i++) {
        numbers.push_back(i);
        
        // Capacity should either stay the same or increase
        assert(numbers.capacity() >= last_capacity);
        
        // Size should always be i+1
        assert(numbers.size() == i + 1);
        
        last_capacity = numbers.capacity();
    }
    
    std::cout << "Vector capacity growth test passed!" << std::endl;
}

void test_vector_reserve_efficiency() {
    std::cout << "Testing vector reserve efficiency..." << std::endl;
    
    // Count capacity changes without reserve
    std::vector<int> without_reserve;
    size_t capacity_changes_without = 0;
    size_t last_capacity = without_reserve.capacity();
    
    for (int i = 0; i < 1000; i++) {
        without_reserve.push_back(i);
        if (without_reserve.capacity() != last_capacity) {
            capacity_changes_without++;
            last_capacity = without_reserve.capacity();
        }
    }
    
    // Count capacity changes with reserve
    std::vector<int> with_reserve;
    with_reserve.reserve(1000);
    size_t capacity_changes_with = 0;
    last_capacity = with_reserve.capacity();
    
    for (int i = 0; i < 1000; i++) {
        with_reserve.push_back(i);
        if (with_reserve.capacity() != last_capacity) {
            capacity_changes_with++;
            last_capacity = with_reserve.capacity();
        }
    }
    
    // Vector with reserve should have fewer capacity changes
    assert(capacity_changes_with <= capacity_changes_without);
    
    std::cout << "Vector reserve efficiency test passed!" << std::endl;
}

int main() {
    std::cout << "Running vector_capacity_demo tests..." << std::endl;
    
    test_vector_basics();
    test_vector_reserve();
    test_vector_resize();
    test_vector_operations();
    test_vector_capacity_growth();
    test_vector_reserve_efficiency();
    
    std::cout << "All vector_capacity_demo tests passed!" << std::endl;
    return 0;
}

#include <iostream>
#include <memory>

// Custom class to demonstrate resource management
class Resource {
    int* data;
public:
    Resource(int value) : data(new int(value)) {
        std::cout << "Resource constructed with value: " << *data << std::endl;
    }
    
    int getValue() const { return *data; }
    
    ~Resource() {
        std::cout << "Resource destructor called" << std::endl;
    }
};

// Custom deleter function object (functor)
class CustomDeleter {
public:
    void operator()(Resource* ptr) const {
        std::cout << "Custom deleter called for Resource with value: " << ptr->getValue() << std::endl;
        delete ptr;
    }
};

// Custom deleter as a free function
void customDeleteFunction(Resource* ptr) {
    std::cout << "Custom delete function called for Resource with value: " << ptr->getValue() << std::endl;
    delete ptr;
}

int main() {
    // Method 1: Using custom deleter functor with unique_ptr
    {
        std::cout << "\nUsing custom deleter functor:" << std::endl;
        std::unique_ptr<Resource, CustomDeleter> ptr1(new Resource(42));
    } // ptr1 automatically deleted here

    // Method 2: Using custom deleter function with unique_ptr
    {
        std::cout << "\nUsing custom deleter function:" << std::endl;
        std::unique_ptr<Resource, decltype(&customDeleteFunction)> ptr2(new Resource(100), customDeleteFunction);
    } // ptr2 automatically deleted here

    // Method 3: Using lambda as custom deleter
    {
        std::cout << "\nUsing lambda as custom deleter:" << std::endl;
        auto lambdaDeleter = [](Resource* ptr) {
            std::cout << "Lambda deleter called for Resource with value: " << ptr->getValue() << std::endl;
            delete ptr;
        };
        std::unique_ptr<Resource, decltype(lambdaDeleter)> ptr3(new Resource(200), lambdaDeleter);
    } // ptr3 automatically deleted here

    return 0;
}

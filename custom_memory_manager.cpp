#include <iostream>

// Forward declaration of our custom deleter function type
template<typename T>
using DeleterFunc = void(*)(T*);

// Custom memory management template class
template<typename T>
class AutoPtr {
private:
    T* ptr;
    DeleterFunc<T> deleter;  // Function pointer to custom deleter

public:
    // Constructor with custom deleter
    AutoPtr(T* p, DeleterFunc<T> d = [](T* ptr) { delete ptr; })
        : ptr(p), deleter(d) {
        std::cout << "AutoPtr constructed\n";
    }

    // Destructor - automatically calls the deleter
    ~AutoPtr() {
        if (ptr) {
            std::cout << "Calling deleter for AutoPtr\n";
            deleter(ptr);
            ptr = nullptr;
        }
    }

    // Prevent copying to ensure single ownership
    AutoPtr(const AutoPtr&) = delete;
    AutoPtr& operator=(const AutoPtr&) = delete;

    // Allow moving
    AutoPtr(AutoPtr&& other) noexcept
        : ptr(other.ptr), deleter(other.deleter) {
        other.ptr = nullptr;
        std::cout << "AutoPtr moved\n";
    }

    AutoPtr& operator=(AutoPtr&& other) noexcept {
        if (this != &other) {
            if (ptr) {
                deleter(ptr);
            }
            ptr = other.ptr;
            deleter = other.deleter;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Access operators
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }

    // Get raw pointer (use with caution)
    T* get() const { return ptr; }

    // Release ownership
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

// Example resource class
class Resource {
    int value;
public:
    Resource(int v) : value(v) {
        std::cout << "Resource constructed with value: " << value << "\n";
    }

    ~Resource() {
        std::cout << "Resource destroyed with value: " << value << "\n";
    }

    void doSomething() {
        std::cout << "Resource value: " << value << "\n";
    }
};

// Custom deleter function
void customResourceDeleter(Resource* ptr) {
    std::cout << "Custom deleter called\n";
    delete ptr;
}

// Example of array deleter
template<typename T>
void arrayDeleter(T* ptr) {
    std::cout << "Array deleter called\n";
    delete[] ptr;
}

int main() {
    // Example 1: Basic usage with default deleter
    {
        std::cout << "\nExample 1: Basic usage\n";
        AutoPtr<Resource> res1(new Resource(42));
        res1->doSomething();
    } // Automatically deleted here

    // Example 2: Custom deleter
    {
        std::cout << "\nExample 2: Custom deleter\n";
        AutoPtr<Resource> res2(new Resource(100), customResourceDeleter);
        res2->doSomething();
    } // Custom deleter called here

    // Example 3: Array with custom deleter
    {
        std::cout << "\nExample 3: Array handling\n";
        AutoPtr<int> arr(new int[5], arrayDeleter<int>);
    } // Array deleter called here

    // Example 4: Move semantics
    {
        std::cout << "\nExample 4: Move semantics\n";
        AutoPtr<Resource> res1(new Resource(200));
        AutoPtr<Resource> res2 = std::move(res1); // Ownership transferred
        // res1 is now nullptr
        if (res2.get()) {
            res2->doSomething();
        }
    }

    return 0;
}

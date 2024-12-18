#include <iostream>

// Resource class to demonstrate memory management
class Resource {
private:
    int* data;
    const char* name;

public:
    Resource(int value, const char* n) : name(n) {
        data = new int(value);
        std::cout << "Resource '" << name << "' constructed with value: " << *data << "\n";
    }

    ~Resource() {
        std::cout << "Resource '" << name << "' destroyed with value: " << *data << "\n";
        delete data;
    }

    void setValue(int value) {
        *data = value;
    }

    int getValue() const {
        return *data;
    }

    void print() const {
        std::cout << "Resource '" << name << "' value: " << *data << "\n";
    }
};

// Custom memory manager class
class AutoMemory {
private:
    Resource* ptr;
    void (*customDeleter)(Resource*);  // Function pointer for custom deletion

public:
    // Constructor with optional custom deleter
    AutoMemory(Resource* p, void (*deleter)(Resource*) = nullptr) 
        : ptr(p), customDeleter(deleter) {
        std::cout << "AutoMemory constructed\n";
    }

    // Destructor - automatically handles cleanup
    ~AutoMemory() {
        if (ptr) {
            std::cout << "AutoMemory destroying resource\n";
            if (customDeleter) {
                customDeleter(ptr);  // Use custom deleter if provided
            } else {
                delete ptr;  // Default deletion
            }
            ptr = nullptr;
        }
    }

    // Prevent copying
    AutoMemory(const AutoMemory&) = delete;
    AutoMemory& operator=(const AutoMemory&) = delete;

    // Allow moving
    AutoMemory(AutoMemory&& other) noexcept : ptr(other.ptr), customDeleter(other.customDeleter) {
        other.ptr = nullptr;
        other.customDeleter = nullptr;
        std::cout << "AutoMemory moved\n";
    }

    AutoMemory& operator=(AutoMemory&& other) noexcept {
        if (this != &other) {
            if (ptr) {
                if (customDeleter) {
                    customDeleter(ptr);
                } else {
                    delete ptr;
                }
            }
            ptr = other.ptr;
            customDeleter = other.customDeleter;
            other.ptr = nullptr;
            other.customDeleter = nullptr;
        }
        return *this;
    }

    // Access operators
    Resource* operator->() { return ptr; }
    Resource& operator*() { return *ptr; }

    // Get raw pointer
    Resource* get() { return ptr; }

    // Release ownership
    Resource* release() {
        Resource* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

// Example custom deleter function
void customResourceDeleter(Resource* ptr) {
    std::cout << "Custom deleter called\n";
    if (ptr) {
        ptr->print();  // Print final state before deletion
        delete ptr;
    }
}

// Example logging deleter
void loggingDeleter(Resource* ptr) {
    std::cout << "Logging deleter: Deleting resource with value: " << ptr->getValue() << "\n";
    delete ptr;
}

int main() {
    // Example 1: Basic usage with default deletion
    {
        std::cout << "\nExample 1: Basic usage\n";
        AutoMemory res1(new Resource(42, "Default"));
        res1->print();
    } // Automatically deleted here

    // Example 2: Using custom deleter
    {
        std::cout << "\nExample 2: Custom deleter\n";
        AutoMemory res2(new Resource(100, "Custom"), customResourceDeleter);
        res2->setValue(150);  // Modify value
        res2->print();
    } // Custom deleter called here

    // Example 3: Using logging deleter
    {
        std::cout << "\nExample 3: Logging deleter\n";
        AutoMemory res3(new Resource(200, "Logging"), loggingDeleter);
        res3->print();
    } // Logging deleter called here

    // Example 4: Move semantics
    {
        std::cout << "\nExample 4: Move semantics\n";
        AutoMemory res1(new Resource(300, "Original"));
        AutoMemory res2 = std::move(res1); // Ownership transferred
        res2->print();
        // res1 is now nullptr
    }

    return 0;
}

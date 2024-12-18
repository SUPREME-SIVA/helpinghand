#include <iostream>
#include <memory>
using namespace std;

class Base {
public:
    virtual ~Base() {
        cout << "Base destructor called" << endl;
    }
};

class Derived : public Base {
public:
    ~Derived() override {
        cout << "Derived destructor called" << endl;
    }
};

void demonstrateSharedPtr() {
    cout << "\nExample 1: Correct shared_ptr usage" << endl;
    cout << "--------------------------------" << endl;
    
    // Create shared_ptr
    cout << "Creating shared_ptr..." << endl;
    shared_ptr<Base> smartPtr = make_shared<Derived>();
    cout << "Reference count: " << smartPtr.use_count() << endl;
    
    // Create another reference
    cout << "\nCreating second reference..." << endl;
    shared_ptr<Base> smartPtr2 = smartPtr;
    cout << "Reference count: " << smartPtr.use_count() << endl;
    
    cout << "\nLetting smart pointers go out of scope..." << endl;
} // Automatic cleanup happens here

void demonstrateIncorrectUsage() {
    cout << "\nExample 2: Incorrect usage (DO NOT DO THIS)" << endl;
    cout << "----------------------------------------" << endl;
    
    // Create shared_ptr
    shared_ptr<Base> smartPtr = make_shared<Derived>();
    
    // WRONG: Never manually delete a shared_ptr's managed object
    // Uncommenting this would cause problems:
    // delete smartPtr.get();  // DON'T DO THIS!
    
    cout << "Smart pointer will clean up automatically" << endl;
}

void demonstrateRawVsSmartPtr() {
    cout << "\nExample 3: Raw pointer vs shared_ptr" << endl;
    cout << "---------------------------------" << endl;
    
    // Raw pointer example
    cout << "Using raw pointer:" << endl;
    Base* rawPtr = new Derived();
    delete rawPtr;  // Manual cleanup needed
    
    cout << "\nUsing shared_ptr:" << endl;
    shared_ptr<Base> smartPtr = make_shared<Derived>();
    // No delete needed - cleanup is automatic
}

int main() {
    cout << "Shared Pointer Memory Management Demo" << endl;
    cout << "===================================" << endl;

    demonstrateSharedPtr();
    demonstrateIncorrectUsage();
    demonstrateRawVsSmartPtr();

    cout << "\nProgram ending..." << endl;
    return 0;
}

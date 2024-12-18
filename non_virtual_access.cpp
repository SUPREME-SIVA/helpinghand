#include <iostream>
#include <memory>  // for std::static_pointer_cast
using namespace std;

class Base {
public:
    // Non-virtual function in base class
    void baseFunction() {
        cout << "Base class function" << endl;
    }
    
    // Virtual function for type identification
    virtual ~Base() {}
};

class Derived : public Base {
public:
    // Non-virtual function specific to Derived class
    void derivedOnlyFunction() {
        cout << "Derived class specific function" << endl;
    }
    
    // Function to demonstrate hiding base class function
    void baseFunction() {
        cout << "Derived class version of base function" << endl;
    }
    
    // Helper function to identify type
    void identifyType() {
        cout << "This is a Derived class object" << endl;
    }
};

// Different methods to access derived class functions
class DerivedAccessDemo {
public:
    static void method1_dynamicCast(Base* ptr) {
        cout << "Method 1 - Using dynamic_cast:" << endl;
        if (Derived* derived = dynamic_cast<Derived*>(ptr)) {
            derived->derivedOnlyFunction();
        } else {
            cout << "Cast failed - not a Derived object" << endl;
        }
    }
    
    static void method2_staticCast(Base* ptr) {
        cout << "Method 2 - Using static_cast (dangerous if ptr is not actually Derived):" << endl;
        Derived* derived = static_cast<Derived*>(ptr);
        derived->derivedOnlyFunction();
    }
    
    static void method3_typeCheck(Base* ptr) {
        cout << "Method 3 - Using typeid comparison:" << endl;
        if (typeid(*ptr) == typeid(Derived)) {
            Derived* derived = static_cast<Derived*>(ptr);
            derived->derivedOnlyFunction();
        } else {
            cout << "Object is not of Derived type" << endl;
        }
    }
    
    static void method4_smartPointer(shared_ptr<Base> ptr) {
        cout << "Method 4 - Using smart pointer cast:" << endl;
        if (auto derived = static_pointer_cast<Derived>(ptr)) {
            derived->derivedOnlyFunction();
        }
    }
};

int main() {
    cout << "Accessing Non-Virtual Functions Demo" << endl;
    cout << "=================================" << endl << endl;

    // Create objects for testing
    Base* basePtr = new Base();
    Base* derivedPtr = new Derived();
    shared_ptr<Base> smartPtr = make_shared<Derived>();

    cout << "1. Using dynamic_cast (Safe but with runtime overhead):" << endl;
    cout << "------------------------------------------------" << endl;
    DerivedAccessDemo::method1_dynamicCast(derivedPtr);
    DerivedAccessDemo::method1_dynamicCast(basePtr);
    
    cout << "\n2. Using static_cast (Fast but unsafe):" << endl;
    cout << "------------------------------------" << endl;
    DerivedAccessDemo::method2_staticCast(derivedPtr);  // Works
    // Uncommenting the following line would be dangerous:
    // DerivedAccessDemo::method2_staticCast(basePtr);  // Would crash!
    
    cout << "\n3. Using typeid (Type checking at runtime):" << endl;
    cout << "----------------------------------------" << endl;
    DerivedAccessDemo::method3_typeCheck(derivedPtr);
    DerivedAccessDemo::method3_typeCheck(basePtr);
    
    cout << "\n4. Using smart pointers (Modern C++ approach):" << endl;
    cout << "------------------------------------------" << endl;
    DerivedAccessDemo::method4_smartPointer(smartPtr);

    cout << "\nBest Practices:" << endl;
    cout << "1. Prefer virtual functions when possible" << endl;
    cout << "2. Use dynamic_cast when type safety is critical" << endl;
    cout << "3. Use static_cast only when you're absolutely sure of the type" << endl;
    cout << "4. Consider smart pointers for better memory management" << endl;
    cout << "5. Avoid accessing non-virtual functions through base pointers if possible" << endl;

    // Cleanup
    delete basePtr;
    delete derivedPtr;

    return 0;
}

#include <iostream>
using namespace std;

class Base {
public:
    // Make the function virtual in base class
    virtual void derivedFunction() {
        cout << "Base class derivedFunction()" << endl;
    }
    
    // Virtual destructor is important for proper cleanup
    virtual ~Base() {}
};

class Derived : public Base {
public:
    // Override keyword makes the intention clear and catches errors
    void derivedFunction() override {
        cout << "Derived class derivedFunction()" << endl;
    }
};

void processObject(Base* obj) {
    // No casting needed - virtual function will call the correct version
    obj->derivedFunction();
}

int main() {
    cout << "Optimized Derived Function Access Demo" << endl;
    cout << "====================================" << endl << endl;

    cout << "1. Using Virtual Functions (Recommended):" << endl;
    cout << "-------------------------------------" << endl;
    Base* ptr1 = new Derived();
    ptr1->derivedFunction();  // Automatically calls Derived version
    
    cout << "\n2. Using Polymorphic Function:" << endl;
    cout << "----------------------------" << endl;
    processObject(new Derived());
    
    cout << "\n3. Comparison with dynamic_cast (Less Optimal):" << endl;
    cout << "-------------------------------------------" << endl;
    Base* ptr2 = new Derived();
    if (Derived* derived = dynamic_cast<Derived*>(ptr2)) {
        derived->derivedFunction();
    }
    
    cout << "\nBest Practices:" << endl;
    cout << "1. Use virtual functions when you know derived classes will override the behavior" << endl;
    cout << "2. Avoid dynamic_cast when possible - it adds runtime overhead" << endl;
    cout << "3. Use override keyword to catch errors and make code more maintainable" << endl;
    cout << "4. Always declare virtual destructor in base class if using virtual functions" << endl;

    // Cleanup
    delete ptr1;
    delete ptr2;

    return 0;
}

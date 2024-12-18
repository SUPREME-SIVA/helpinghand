#include <iostream>
using namespace std;

// Base class for inheritance examples
class Base {
    virtual void dummy() {}  // Makes the class polymorphic
public:
    virtual ~Base() {}
};

// Derived class
class Derived : public Base {
public:
    void derivedFunction() {
        cout << "Function from Derived class" << endl;
    }
};

// Class for const_cast example
class ConstCastDemo {
public:
    int value;
    void setValue(int x) { value = x; }
};

int main() {
    cout << "C++ Casting Operators Demo" << endl;
    cout << "=========================" << endl << endl;

    // 1. static_cast
    cout << "1. static_cast Examples:" << endl;
    cout << "------------------------" << endl;
    
    // Example 1: Basic type conversion
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);
    cout << "static_cast<int>(3.14159) = " << intPi << endl;

    // Example 2: Pointer upcast (derived to base)
    Derived* derived = new Derived();
    Base* base = static_cast<Base*>(derived);
    cout << "Derived to Base pointer conversion successful" << endl;

    // 2. dynamic_cast
    cout << "\n2. dynamic_cast Examples:" << endl;
    cout << "-------------------------" << endl;
    
    // Safe downcasting
    Base* basePtr = new Derived();
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    
    if (derivedPtr) {
        cout << "dynamic_cast successful - object is of Derived type" << endl;
        derivedPtr->derivedFunction();
    } else {
        cout << "dynamic_cast failed - object is not of Derived type" << endl;
    }

    // Failed dynamic_cast example
    Base* pureBase = new Base();
    Derived* failedCast = dynamic_cast<Derived*>(pureBase);
    
    if (failedCast) {
        cout << "This won't print - cast failed" << endl;
    } else {
        cout << "dynamic_cast failed as expected - Base pointer doesn't point to Derived object" << endl;
    }

    // 3. const_cast
    cout << "\n3. const_cast Examples:" << endl;
    cout << "----------------------" << endl;
    
    const ConstCastDemo* constObj = new ConstCastDemo();
    // Remove const to call non-const member function
    ConstCastDemo* mutableObj = const_cast<ConstCastDemo*>(constObj);
    mutableObj->setValue(42);
    cout << "const_cast allowed modification of const object: " << mutableObj->value << endl;

    // 4. reinterpret_cast
    cout << "\n4. reinterpret_cast Examples:" << endl;
    cout << "---------------------------" << endl;
    
    // Convert pointer to integer
    int* ptr = new int(65);
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    cout << "Address of pointer using reinterpret_cast: 0x" << hex << addr << endl;

    // Convert back to pointer
    int* recoveredPtr = reinterpret_cast<int*>(addr);
    cout << "Value after recovering pointer: " << dec << *recoveredPtr << endl;

    cout << "\nWhen to Use Each Cast:" << endl;
    cout << "======================" << endl;
    cout << "1. static_cast: \n"
         << "   - For standard type conversions\n"
         << "   - Upcasting in inheritance hierarchy\n"
         << "   - When you're sure about the types\n";
    
    cout << "\n2. dynamic_cast: \n"
         << "   - For safe downcasting in inheritance hierarchy\n"
         << "   - When you need runtime type checking\n"
         << "   - Only works with polymorphic classes (with virtual functions)\n";
    
    cout << "\n3. const_cast: \n"
         << "   - To remove const/volatile qualifiers\n"
         << "   - When dealing with APIs that don't use const but don't modify data\n"
         << "   - Use sparingly as it can lead to undefined behavior\n";
    
    cout << "\n4. reinterpret_cast: \n"
         << "   - For low-level pointer conversions\n"
         << "   - Platform-specific code\n"
         << "   - Should be used very carefully as it's the most dangerous cast\n";

    // Cleanup
    delete derived;
    delete pureBase;
    delete constObj;
    delete ptr;

    return 0;
}

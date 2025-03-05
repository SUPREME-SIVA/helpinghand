#include <iostream>
#include <memory>
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

// Classes from shared_ptr_demo.cpp
class Base {
public:
    virtual ~Base() {
        std::cout << "Base destructor called" << std::endl;
    }
};

class Derived : public Base {
public:
    ~Derived() override {
        std::cout << "Derived destructor called" << std::endl;
    }
};

// Test functions
void test_shared_ptr_creation() {
    std::cout << "Testing shared_ptr creation..." << std::endl;
    
    std::shared_ptr<Base> smartPtr = std::make_shared<Derived>();
    assert(smartPtr != nullptr);
    assert(smartPtr.use_count() == 1);
    
    std::cout << "Shared pointer creation test passed!" << std::endl;
}

void test_shared_ptr_reference_counting() {
    std::cout << "Testing shared_ptr reference counting..." << std::endl;
    
    std::shared_ptr<Base> smartPtr1 = std::make_shared<Derived>();
    assert(smartPtr1.use_count() == 1);
    
    {
        std::shared_ptr<Base> smartPtr2 = smartPtr1;
        assert(smartPtr1.use_count() == 2);
        assert(smartPtr2.use_count() == 2);
        
        std::shared_ptr<Base> smartPtr3 = smartPtr1;
        assert(smartPtr1.use_count() == 3);
        assert(smartPtr2.use_count() == 3);
        assert(smartPtr3.use_count() == 3);
    }
    
    // After smartPtr2 and smartPtr3 go out of scope
    assert(smartPtr1.use_count() == 1);
    
    std::cout << "Shared pointer reference counting test passed!" << std::endl;
}

void test_shared_ptr_destruction() {
    std::cout << "Testing shared_ptr destruction..." << std::endl;
    
    CoutRedirect redirect;
    
    {
        std::shared_ptr<Base> smartPtr = std::make_shared<Derived>();
    } // smartPtr goes out of scope here
    
    std::string output = redirect.get_output();
    
    // Check that both destructors were called in the correct order
    assert(output.find("Derived destructor called") != std::string::npos);
    assert(output.find("Base destructor called") != std::string::npos);
    
    // Check that Derived destructor was called before Base destructor
    assert(output.find("Derived destructor called") < output.find("Base destructor called"));
    
    std::cout << "Shared pointer destruction test passed!" << std::endl;
}

void test_shared_ptr_reset() {
    std::cout << "Testing shared_ptr reset..." << std::endl;
    
    std::shared_ptr<Base> smartPtr = std::make_shared<Derived>();
    assert(smartPtr != nullptr);
    
    smartPtr.reset();
    assert(smartPtr == nullptr);
    assert(smartPtr.use_count() == 0);
    
    std::cout << "Shared pointer reset test passed!" << std::endl;
}

int main() {
    std::cout << "Running shared_ptr_demo tests..." << std::endl;
    
    test_shared_ptr_creation();
    test_shared_ptr_reference_counting();
    test_shared_ptr_destruction();
    test_shared_ptr_reset();
    
    std::cout << "All shared_ptr_demo tests passed!" << std::endl;
    return 0;
}

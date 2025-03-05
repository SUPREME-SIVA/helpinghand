#include <iostream>
#include <memory>
#include <vector>
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

// Resource class from unique_ptr_demo.cpp
class Resource {
private:
    std::string name;
    int data;

public:
    // Constructor
    Resource(const std::string& n, int d) : name(n), data(d) {
        std::cout << "Resource constructed: " << name << std::endl;
    }

    // Destructor
    ~Resource() {
        std::cout << "Resource destroyed: " << name << std::endl;
    }

    void use() {
        std::cout << "Using resource " << name << " with data: " << data << std::endl;
    }

    void updateData(int newData) {
        data = newData;
        std::cout << "Updated " << name << " with new data: " << data << std::endl;
    }
    
    int getData() const {
        return data;
    }
    
    std::string getName() const {
        return name;
    }
};

// Function that returns a unique_ptr
std::unique_ptr<Resource> createResource(const std::string& name, int data) {
    return std::make_unique<Resource>(name, data);
}

// ResourceManager class from unique_ptr_demo.cpp
class ResourceManager {
private:
    std::unique_ptr<Resource> managedResource;

public:
    // Constructor taking ownership of a resource
    ResourceManager(std::unique_ptr<Resource> resource) 
        : managedResource(std::move(resource)) {}

    // Move constructor
    ResourceManager(ResourceManager&& other) noexcept 
        : managedResource(std::move(other.managedResource)) {}

    void useResource() {
        if (managedResource) {
            managedResource->use();
        }
    }
    
    bool hasResource() const {
        return managedResource != nullptr;
    }
};

// Test functions
void test_unique_ptr_creation() {
    std::cout << "Testing unique_ptr creation..." << std::endl;
    
    CoutRedirect redirect;
    
    {
        std::unique_ptr<Resource> resource = std::make_unique<Resource>("TestResource", 100);
        assert(resource != nullptr);
        assert(resource->getData() == 100);
        assert(resource->getName() == "TestResource");
    }
    
    std::string output = redirect.get_output();
    assert(output.find("Resource constructed: TestResource") != std::string::npos);
    assert(output.find("Resource destroyed: TestResource") != std::string::npos);
    
    std::cout << "Unique pointer creation test passed!" << std::endl;
}

void test_unique_ptr_transfer() {
    std::cout << "Testing unique_ptr ownership transfer..." << std::endl;
    
    std::unique_ptr<Resource> resource1 = std::make_unique<Resource>("TransferResource", 200);
    assert(resource1 != nullptr);
    
    std::unique_ptr<Resource> resource2 = std::move(resource1);
    assert(resource1 == nullptr);
    assert(resource2 != nullptr);
    assert(resource2->getData() == 200);
    
    std::cout << "Unique pointer ownership transfer test passed!" << std::endl;
}

void test_unique_ptr_in_container() {
    std::cout << "Testing unique_ptr in container..." << std::endl;
    
    std::vector<std::unique_ptr<Resource>> resources;
    
    resources.push_back(std::make_unique<Resource>("VectorResource1", 300));
    resources.push_back(std::make_unique<Resource>("VectorResource2", 400));
    
    assert(resources.size() == 2);
    assert(resources[0]->getData() == 300);
    assert(resources[1]->getData() == 400);
    
    std::cout << "Unique pointer in container test passed!" << std::endl;
}

void test_resource_manager() {
    std::cout << "Testing ResourceManager with unique_ptr..." << std::endl;
    
    auto resource = std::make_unique<Resource>("ManagedResource", 500);
    ResourceManager manager(std::move(resource));
    
    assert(resource == nullptr);
    assert(manager.hasResource());
    
    // Test move constructor
    ResourceManager manager2(std::move(manager));
    assert(!manager.hasResource());
    assert(manager2.hasResource());
    
    std::cout << "ResourceManager test passed!" << std::endl;
}

void test_create_resource_function() {
    std::cout << "Testing createResource function..." << std::endl;
    
    auto resource = createResource("FunctionResource", 600);
    assert(resource != nullptr);
    assert(resource->getData() == 600);
    assert(resource->getName() == "FunctionResource");
    
    std::cout << "createResource function test passed!" << std::endl;
}

int main() {
    std::cout << "Running unique_ptr_demo tests..." << std::endl;
    
    test_unique_ptr_creation();
    test_unique_ptr_transfer();
    test_unique_ptr_in_container();
    test_resource_manager();
    test_create_resource_function();
    
    std::cout << "All unique_ptr_demo tests passed!" << std::endl;
    return 0;
}

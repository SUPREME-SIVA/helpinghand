#include <iostream>
#include <memory>
#include <vector>

// A simple Resource class to demonstrate resource management
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
};

// Function that returns a unique_ptr
std::unique_ptr<Resource> createResource(const std::string& name, int data) {
    return std::make_unique<Resource>(name, data);
}

// Function that takes a unique_ptr by reference
void processResource(const std::unique_ptr<Resource>& resource) {
    if (resource) {
        resource->use();
    }
}

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
};

int main() {
    // Example 1: Basic unique_ptr usage
    std::cout << "\n=== Example 1: Basic Usage ===\n";
    {
        std::unique_ptr<Resource> resource1 = std::make_unique<Resource>("Resource1", 100);
        resource1->use();
        resource1->updateData(200);
    } // resource1 automatically deleted here

    // Example 2: Transfer ownership
    std::cout << "\n=== Example 2: Transfer Ownership ===\n";
    {
        auto resource2 = createResource("Resource2", 300);
        processResource(resource2);
        
        // Transfer ownership to another unique_ptr
        std::unique_ptr<Resource> resource2_new = std::move(resource2);
        // resource2 is now nullptr
        std::cout << "Is original pointer empty? " << (resource2 == nullptr ? "Yes" : "No") << std::endl;
        
        processResource(resource2_new);
    }

    // Example 3: Using unique_ptr in a container
    std::cout << "\n=== Example 3: Container Usage ===\n";
    {
        std::vector<std::unique_ptr<Resource>> resources;
        
        resources.push_back(std::make_unique<Resource>("VectorResource1", 400));
        resources.push_back(std::make_unique<Resource>("VectorResource2", 500));
        
        for (const auto& res : resources) {
            res->use();
        }
    } // all resources in vector automatically cleaned up

    // Example 4: Using unique_ptr in a class
    std::cout << "\n=== Example 4: Class Usage ===\n";
    {
        auto resource4 = std::make_unique<Resource>("ManagedResource", 600);
        ResourceManager manager(std::move(resource4));
        manager.useResource();
    }

    std::cout << "\nProgram ending...\n";
    return 0;
}

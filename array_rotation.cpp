#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Interface segregation principle: Separate interface for array operations
class IArrayRotator {
public:
    virtual void rotate(std::vector<int>& arr, int positions) = 0;
    virtual std::string getRotationType() const = 0;
    virtual ~IArrayRotator() = default;
};

// Single responsibility principle: Base class for array operations
class ArrayHandler {
protected:
    void rotateOnePosition(std::vector<int>& arr, bool isLeft) {
        if (arr.empty()) return;
        
        if (isLeft) {
            int temp = arr[0];
            arr.erase(arr.begin());
            arr.push_back(temp);
        } else {
            int temp = arr.back();
            arr.pop_back();
            arr.insert(arr.begin(), temp);
        }
    }

public:
    virtual ~ArrayHandler() = default;
    
    void printArray(const std::vector<int>& arr, const std::string& message) const {
        std::cout << message << ": ";
        for (const int& num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
};

// Open/closed principle: Concrete implementations for different rotation types
class LeftRotator : public ArrayHandler, public IArrayRotator {
public:
    void rotate(std::vector<int>& arr, int positions) override {
        positions %= arr.size();
        for (int i = 0; i < positions; ++i) {
            rotateOnePosition(arr, true);
        }
    }
    
    std::string getRotationType() const override {
        return "left";
    }
};

class RightRotator : public ArrayHandler, public IArrayRotator {
public:
    void rotate(std::vector<int>& arr, int positions) override {
        positions %= arr.size();
        for (int i = 0; i < positions; ++i) {
            rotateOnePosition(arr, false);
        }
    }
    
    std::string getRotationType() const override {
        return "right";
    }
};

// Dependency inversion principle: High-level module depending on abstraction
class RotationDemonstrator {
private:
    std::shared_ptr<IArrayRotator> rotator;
    ArrayHandler arrayHandler;

public:
    explicit RotationDemonstrator(std::shared_ptr<IArrayRotator> r) : rotator(r) {}

    void demonstrate(std::vector<int> arr, int positions) {
        std::cout << "\nDemonstrating " << positions << " rotations " 
                 << rotator->getRotationType() << ":\n";
        
        arrayHandler.printArray(arr, "Before rotation");
        rotator->rotate(arr, positions);
        arrayHandler.printArray(arr, "After rotation");
    }
};

// Liskov substitution principle: Both rotation types can be used interchangeably
void runRotationExample(const std::string& title, 
                       std::shared_ptr<IArrayRotator> rotator,
                       const std::vector<int>& arr, 
                       int positions) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.length(), '-') << "\n";
    
    RotationDemonstrator demo(rotator);
    demo.demonstrate(arr, positions);
}

int main() {
    // Example 1: Left rotation
    std::vector<int> arr1 = {1, 2, 3, 4, 5, 6, 7, 8};
    auto leftRotator = std::make_shared<LeftRotator>();
    runRotationExample("Example 1: Left Rotation", leftRotator, arr1, 4);

    // Example 2: Right rotation
    std::vector<int> arr2 = {10, 20, 30, 40, 50, 60, 70, 80};
    auto rightRotator = std::make_shared<RightRotator>();
    runRotationExample("Example 2: Right Rotation", rightRotator, arr2, 4);

    return 0;
}

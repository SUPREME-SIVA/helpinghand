/**
 * @file singly_linked_list.cpp
 * @brief Implementation of a singly linked list in C++
 *
 * This file demonstrates a modern C++ implementation of a singly linked list
 * with operations like creation, insertion, deletion, and traversal.
 * It uses smart pointers for memory management and follows modern C++ practices.
 */

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * @brief Template class for a singly linked list
 * 
 * @tparam T Type of data stored in the list
 */
template <typename T>
class SinglyLinkedList {
private:
    /**
     * @brief Structure for a node in the singly linked list
     */
    struct Node {
        T data;                      // Data stored in the node
        std::unique_ptr<Node> next;  // Smart pointer to the next node
        
        /**
         * @brief Constructor for Node
         * 
         * @param value Data to be stored in the node
         */
        explicit Node(T value) : data(std::move(value)), next(nullptr) {}
    };
    
    std::unique_ptr<Node> head_;     // Smart pointer to the first node
    size_t size_;                    // Number of nodes in the list
    
public:
    /**
     * @brief Default constructor
     */
    SinglyLinkedList() : head_(nullptr), size_(0) {}
    
    /**
     * @brief Destructor
     * 
     * The destructor is automatically handled by the unique_ptr
     */
    ~SinglyLinkedList() = default;
    
    /**
     * @brief Copy constructor (deleted)
     * 
     * Copying is disabled because std::unique_ptr cannot be copied
     */
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    
    /**
     * @brief Copy assignment operator (deleted)
     * 
     * Copying is disabled because std::unique_ptr cannot be copied
     */
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;
    
    /**
     * @brief Move constructor
     * 
     * @param other SinglyLinkedList to move from
     */
    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head_(std::move(other.head_)), size_(other.size_) {
        other.size_ = 0;
    }
    
    /**
     * @brief Move assignment operator
     * 
     * @param other SinglyLinkedList to move from
     * @return SinglyLinkedList& Reference to this list
     */
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            head_ = std::move(other.head_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    /**
     * @brief Check if the linked list is empty
     * 
     * @return true if the list is empty, false otherwise
     */
    [[nodiscard]] bool isEmpty() const {
        return head_ == nullptr;
    }
    
    /**
     * @brief Get the size of the linked list
     * 
     * @return size_t Number of nodes in the list
     */
    [[nodiscard]] size_t size() const {
        return size_;
    }
    
    /**
     * @brief Insert a node at the beginning of the linked list
     * 
     * @param value Data to be inserted
     */
    void insertAtBeginning(T value) {
        auto new_node = std::make_unique<Node>(std::move(value));
        
        if (isEmpty()) {
            head_ = std::move(new_node);
        } else {
            new_node->next = std::move(head_);
            head_ = std::move(new_node);
        }
        
        ++size_;
    }
    
    /**
     * @brief Insert a node at the end of the linked list
     * 
     * @param value Data to be inserted
     */
    void insertAtEnd(T value) {
        auto new_node = std::make_unique<Node>(std::move(value));
        
        if (isEmpty()) {
            head_ = std::move(new_node);
        } else {
            Node* current = head_.get();
            while (current->next) {
                current = current->next.get();
            }
            current->next = std::move(new_node);
        }
        
        ++size_;
    }
    
    /**
     * @brief Insert a node at a specific position in the linked list
     * 
     * @param value Data to be inserted
     * @param position Position at which to insert (0-based index)
     * @return true if insertion was successful, false otherwise
     */
    bool insertAtPosition(T value, size_t position) {
        // Check if position is valid
        if (position > size_) {
            return false;
        }
        
        // If position is 0, insert at the beginning
        if (position == 0) {
            insertAtBeginning(std::move(value));
            return true;
        }
        
        // If position is at the end, insert at the end
        if (position == size_) {
            insertAtEnd(std::move(value));
            return true;
        }
        
        // Otherwise, insert at the specified position
        auto new_node = std::make_unique<Node>(std::move(value));
        Node* current = head_.get();
        
        // Traverse to the node just before the desired position
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next.get();
        }
        
        // Insert the new node
        new_node->next = std::move(current->next);
        current->next = std::move(new_node);
        
        ++size_;
        return true;
    }
    
    /**
     * @brief Delete a node from the beginning of the linked list
     * 
     * @return true if deletion was successful, false if the list was empty
     */
    bool deleteFromBeginning() {
        if (isEmpty()) {
            return false;
        }
        
        head_ = std::move(head_->next);
        --size_;
        
        return true;
    }
    
    /**
     * @brief Delete a node from the end of the linked list
     * 
     * @return true if deletion was successful, false if the list was empty
     */
    bool deleteFromEnd() {
        if (isEmpty()) {
            return false;
        }
        
        // If there's only one node
        if (!head_->next) {
            head_.reset();
            --size_;
            return true;
        }
        
        // Find the second-to-last node
        Node* current = head_.get();
        while (current->next->next) {
            current = current->next.get();
        }
        
        // Delete the last node
        current->next.reset();
        
        --size_;
        return true;
    }
    
    /**
     * @brief Delete a node at a specific position in the linked list
     * 
     * @param position Position from which to delete (0-based index)
     * @return true if deletion was successful, false otherwise
     */
    bool deleteFromPosition(size_t position) {
        // Check if position is valid
        if (position >= size_ || isEmpty()) {
            return false;
        }
        
        // If position is 0, delete from the beginning
        if (position == 0) {
            return deleteFromBeginning();
        }
        
        // If position is at the end, delete from the end
        if (position == size_ - 1) {
            return deleteFromEnd();
        }
        
        // Otherwise, delete from the specified position
        Node* current = head_.get();
        
        // Traverse to the node just before the one to be deleted
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next.get();
        }
        
        // Delete the node
        current->next = std::move(current->next->next);
        
        --size_;
        return true;
    }
    
    /**
     * @brief Search for a value in the linked list
     * 
     * @param value Data to search for
     * @return std::optional<size_t> Position of the first occurrence (0-based),
     *         std::nullopt if not found
     */
    [[nodiscard]] std::optional<size_t> search(const T& value) const {
        if (isEmpty()) {
            return std::nullopt;
        }
        
        const Node* current = head_.get();
        size_t position = 0;
        
        while (current) {
            if (current->data == value) {
                return position;
            }
            current = current->next.get();
            ++position;
        }
        
        return std::nullopt;  // Not found
    }
    
    /**
     * @brief Get the value at a specific position in the linked list
     * 
     * @param position Position to get the value from (0-based index)
     * @return std::optional<T> Value at the position, std::nullopt if position is invalid
     */
    [[nodiscard]] std::optional<T> at(size_t position) const {
        if (position >= size_) {
            return std::nullopt;
        }
        
        const Node* current = head_.get();
        
        for (size_t i = 0; i < position; ++i) {
            current = current->next.get();
        }
        
        return current->data;
    }
    
    /**
     * @brief Display the contents of the linked list
     */
    void display() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }
        
        std::cout << "List (size " << size_ << "): ";
        
        const Node* current = head_.get();
        while (current) {
            std::cout << current->data;
            current = current->next.get();
            if (current) {
                std::cout << " -> ";
            }
        }
        
        std::cout << std::endl;
    }
    
    /**
     * @brief Reverse the linked list
     */
    void reverse() {
        if (isEmpty() || size_ == 1) {
            return;  // Nothing to reverse
        }
        
        std::unique_ptr<Node> prev = nullptr;
        std::unique_ptr<Node> current = std::move(head_);
        
        while (current) {
            std::unique_ptr<Node> next = std::move(current->next);
            current->next = std::move(prev);
            prev = std::move(current);
            current = std::move(next);
        }
        
        head_ = std::move(prev);
    }
    
    /**
     * @brief Get the middle node of the linked list
     * 
     * Uses the slow and fast pointer technique (Floyd's cycle-finding algorithm)
     * 
     * @return std::optional<T> Value of the middle node, std::nullopt if the list is empty
     */
    [[nodiscard]] std::optional<T> getMiddleValue() const {
        if (isEmpty()) {
            return std::nullopt;
        }
        
        const Node* slow = head_.get();
        const Node* fast = head_.get();
        
        // Fast pointer moves twice as fast as the slow pointer
        // When fast reaches the end, slow will be at the middle
        while (fast && fast->next) {
            slow = slow->next.get();
            fast = fast->next->next.get();
        }
        
        return slow->data;
    }
    
    /**
     * @brief Check if the linked list has a cycle
     * 
     * Note: This is for demonstration purposes only. In a real implementation with
     * unique_ptr, cycles should not be possible as they would create ownership issues.
     * 
     * @return bool Always returns false with unique_ptr implementation
     */
    [[nodiscard]] bool hasCycle() const {
        // With unique_ptr, cycles are not possible as each node can only have one owner
        return false;
    }
    
    /**
     * @brief Convert the linked list to a vector
     * 
     * @return std::vector<T> Vector containing all elements in the list
     */
    [[nodiscard]] std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(size_);
        
        const Node* current = head_.get();
        while (current) {
            result.push_back(current->data);
            current = current->next.get();
        }
        
        return result;
    }
};

/**
 * @brief Main function to demonstrate the linked list operations
 */
int main() {
    // Create a new linked list
    SinglyLinkedList<int> list;
    
    // Check if the list is empty
    std::cout << "Is the list empty? " << (list.isEmpty() ? "Yes" : "No") << std::endl;
    
    // Insert elements at the beginning
    std::cout << "\nInserting elements at the beginning..." << std::endl;
    list.insertAtBeginning(10);
    list.insertAtBeginning(20);
    list.insertAtBeginning(30);
    list.display();
    
    // Insert elements at the end
    std::cout << "\nInserting elements at the end..." << std::endl;
    list.insertAtEnd(40);
    list.insertAtEnd(50);
    list.display();
    
    // Insert at a specific position
    std::cout << "\nInserting 35 at position 2..." << std::endl;
    list.insertAtPosition(35, 2);
    list.display();
    
    // Search for elements
    std::cout << "\nSearching for elements..." << std::endl;
    auto pos30 = list.search(30);
    std::cout << "Position of 30: " << (pos30.has_value() ? std::to_string(pos30.value()) : "Not found") << std::endl;
    
    auto pos35 = list.search(35);
    std::cout << "Position of 35: " << (pos35.has_value() ? std::to_string(pos35.value()) : "Not found") << std::endl;
    
    auto pos100 = list.search(100);
    std::cout << "Position of 100: " << (pos100.has_value() ? std::to_string(pos100.value()) : "Not found") << std::endl;
    
    // Delete from beginning
    std::cout << "\nDeleting from the beginning..." << std::endl;
    list.deleteFromBeginning();
    list.display();
    
    // Delete from end
    std::cout << "\nDeleting from the end..." << std::endl;
    list.deleteFromEnd();
    list.display();
    
    // Delete from position
    std::cout << "\nDeleting from position 1..." << std::endl;
    list.deleteFromPosition(1);
    list.display();
    
    // Get middle value
    std::cout << "\nMiddle value: ";
    auto middle = list.getMiddleValue();
    if (middle.has_value()) {
        std::cout << middle.value() << std::endl;
    } else {
        std::cout << "None (list is empty)" << std::endl;
    }
    
    // Reverse the list
    std::cout << "\nReversing the list..." << std::endl;
    list.reverse();
    list.display();
    
    // Convert to vector
    std::cout << "\nConverting to vector..." << std::endl;
    auto vec = list.toVector();
    std::cout << "Vector: [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
    
    // Check for cycle
    std::cout << "\nDoes the list have a cycle? " << (list.hasCycle() ? "Yes" : "No") << std::endl;
    
    std::cout << "\nLinked list will be automatically destroyed when it goes out of scope" << std::endl;
    
    return 0;
}

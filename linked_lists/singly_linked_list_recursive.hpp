#ifndef SINGLY_LINKED_LIST_RECURSIVE_HPP
#define SINGLY_LINKED_LIST_RECURSIVE_HPP

#include <memory>
#include <stdexcept>

// Node structure for the linked list
class Node {
public:
    int data;
    std::shared_ptr<Node> next;

    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    std::shared_ptr<Node> head;

    // Helper functions declarations
    void insertAtStartHelper(std::shared_ptr<Node>& head, int value);
    void insertAtMiddleHelper(std::shared_ptr<Node> current, int value, int position, int count = 1);
    std::shared_ptr<Node> findMiddleHelper(std::shared_ptr<Node> slow, std::shared_ptr<Node> fast);
    bool detectCycleHelper(std::shared_ptr<Node> slow, std::shared_ptr<Node> fast);
    void insertAtEndHelper(std::shared_ptr<Node> current, int value);
    bool searchHelper(std::shared_ptr<Node> current, int value);
    int lengthHelper(std::shared_ptr<Node> current, int count = 0);
    std::shared_ptr<Node> reverseHelper(std::shared_ptr<Node> current, std::shared_ptr<Node> prev = nullptr);
    std::shared_ptr<Node> getLastNodeHelper(std::shared_ptr<Node> current) const;

public:
    LinkedList() : head(nullptr) {}

    // Public methods
    void insertAtStart(int value);
    void insertAtMiddle(int value, int position);
    std::shared_ptr<Node> findMiddle();
    bool detectCycle();
    void insertAtEnd(int value);
    bool deleteByValue(int value);
    bool search(int value);
    int length();
    void reverse();
    void print() const;
    std::shared_ptr<Node> getLastNode() const;
    
    // Public method for testing
    std::shared_ptr<Node> getHead() const { return head; }
};

#endif // SINGLY_LINKED_LIST_RECURSIVE_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include "singly_linked_list_recursive.hpp"

// Implementation of helper functions
void LinkedList::insertAtStartHelper(std::shared_ptr<Node>& head, int value) {
    auto newNode = std::make_shared<Node>(value);
    newNode->next = head;
    head = newNode;
}

void LinkedList::insertAtMiddleHelper(std::shared_ptr<Node> current, int value, int position, int count) {
    if (current == nullptr) {
        throw std::out_of_range("Position out of range");
    }
    if (count == position) {
        auto newNode = std::make_shared<Node>(value);
        newNode->next = current->next;
        current->next = newNode;
    } else {
        insertAtMiddleHelper(current->next, value, position, count + 1);
    }
}

std::shared_ptr<Node> LinkedList::findMiddleHelper(std::shared_ptr<Node> slow, std::shared_ptr<Node> fast) {
    if (fast == nullptr || fast->next == nullptr) {
        return slow;
    }
    return findMiddleHelper(slow->next, fast->next->next);
}

bool LinkedList::detectCycleHelper(std::shared_ptr<Node> slow, std::shared_ptr<Node> fast) {
    if (fast == nullptr || fast->next == nullptr) {
        return false;
    }
    if (slow == fast) {
        return true;
    }
    return detectCycleHelper(slow->next, fast->next->next);
}

void LinkedList::insertAtEndHelper(std::shared_ptr<Node> current, int value) {
    if (current->next == nullptr) {
        current->next = std::make_shared<Node>(value);
    } else {
        insertAtEndHelper(current->next, value);
    }
}

bool LinkedList::deleteByValue(int value) {
    if (this->head == nullptr) {
        return false;
    }
    
    // Check if value is at head
    if (this->head->data == value) {
        this->head = this->head->next;
        return true;
    }
    
    // Check rest of the list
    auto prev = this->head;
    auto current = this->head->next;
    while (current != nullptr) {
        if (current->data == value) {
            prev->next = current->next;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

bool LinkedList::searchHelper(std::shared_ptr<Node> current, int value) {
    if (current == nullptr) {
        return false;
    }
    if (current->data == value) {
        return true;
    }
    return searchHelper(current->next, value);
}

int LinkedList::lengthHelper(std::shared_ptr<Node> current, int count) {
    if (current == nullptr) {
        return count;
    }
    return lengthHelper(current->next, count + 1);
}

std::shared_ptr<Node> LinkedList::reverseHelper(std::shared_ptr<Node> current, std::shared_ptr<Node> prev) {
    if (current == nullptr) {
        return prev;
    }
    std::shared_ptr<Node> next = current->next;
    current->next = prev;
    return reverseHelper(next, current);
}

std::shared_ptr<Node> LinkedList::getLastNodeHelper(std::shared_ptr<Node> current) const {
    if (current->next == nullptr) {
        return current;
    }
    return getLastNodeHelper(current->next);
}

// Implementation of public methods
void LinkedList::insertAtStart(int value) {
    insertAtStartHelper(head, value);
}

void LinkedList::insertAtMiddle(int value, int position) {
    if (position < 1 || position > length() + 1) {
        throw std::out_of_range("Position out of range");
    }
    if (position == 1) {
        insertAtStart(value);
        return;
    }
    insertAtMiddleHelper(head, value, position, 1);
}

std::shared_ptr<Node> LinkedList::findMiddle() {
    if (this->head == nullptr) {
        return nullptr;
    }
    return findMiddleHelper(this->head, this->head->next);
}

bool LinkedList::detectCycle() {
    if (this->head == nullptr || this->head->next == nullptr) {
        return false;
    }
    return detectCycleHelper(this->head, this->head->next);
}

void LinkedList::insertAtEnd(int value) {
    if (this->head == nullptr) {
        this->head = std::make_shared<Node>(value);
    } else {
        insertAtEndHelper(this->head, value);
    }
}

bool LinkedList::search(int value) {
    return searchHelper(this->head, value);
}

int LinkedList::length() {
    return lengthHelper(this->head);
}

void LinkedList::reverse() {
    this->head = reverseHelper(this->head, nullptr);
}

void LinkedList::print() const {
    auto current = this->head;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "NULL" << std::endl;
}

std::shared_ptr<Node> LinkedList::getLastNode() const {
    if (this->head == nullptr) {
        return nullptr;
    }
    return getLastNodeHelper(this->head);
}

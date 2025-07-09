#include "gtest/gtest.h"
#include "singly_linked_list_recursive.hpp"

// Helper function to create a list with specific values
std::shared_ptr<Node> createList(const std::vector<int>& values) {
    LinkedList list;
    for (int val : values) {
        list.insertAtEnd(val);
    }
    return list.getHead();
}

class LinkedListTest : public ::testing::Test {
protected:
    LinkedList list;

    void SetUp() override {
        list = LinkedList();
    }

    void TearDown() override {
        // No cleanup needed as smart pointers handle memory
    }

    // Helper method to get node data at position
    int getNodeDataAtPosition(int position) {
        std::shared_ptr<Node> current = list.getHead();
        int count = 0;
        while (current && count < position) {
            current = current->next;
            count++;
        }
        return current ? current->data : -1;
    }
};

// Test insertion operations
TEST_F(LinkedListTest, InsertionOperations) {
    // Test insert at start
    list.insertAtStart(1);
    EXPECT_EQ(getNodeDataAtPosition(0), 1);
    
    list.insertAtStart(2);
    EXPECT_EQ(getNodeDataAtPosition(0), 2);
    EXPECT_EQ(getNodeDataAtPosition(1), 1);

    // Test insert at end
    list.insertAtEnd(3);
    EXPECT_EQ(getNodeDataAtPosition(2), 3);
    
    // Test insert at middle
    list.insertAtMiddle(4, 2);
    EXPECT_EQ(list.length(), 4);
    EXPECT_EQ(getNodeDataAtPosition(2), 4);
}

// Test deletion
TEST_F(LinkedListTest, Deletion) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);

    // Delete existing value
    EXPECT_TRUE(list.deleteByValue(2));
    EXPECT_EQ(list.length(), 2);
    
    // Delete non-existing value
    EXPECT_FALSE(list.deleteByValue(100));
}

// Test search functionality
TEST_F(LinkedListTest, Search) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);

    EXPECT_TRUE(list.search(2));
    EXPECT_FALSE(list.search(100));
}

// Test length calculation
TEST_F(LinkedListTest, Length) {
    EXPECT_EQ(list.length(), 0);
    
    list.insertAtEnd(1);
    EXPECT_EQ(list.length(), 1);
    
    list.insertAtEnd(2);
    EXPECT_EQ(list.length(), 2);
}

// Test reverse functionality
TEST_F(LinkedListTest, Reverse) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);

    list.reverse();
    EXPECT_EQ(getNodeDataAtPosition(0), 3);
    EXPECT_EQ(getNodeDataAtPosition(1), 2);
    EXPECT_EQ(getNodeDataAtPosition(2), 1);
}

// Test find middle
TEST_F(LinkedListTest, FindMiddle) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.insertAtEnd(5);

    auto middle = list.findMiddle();
    EXPECT_EQ(middle->data, 3);
}

// Test cycle detection
TEST_F(LinkedListTest, DetectCycle) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);

    // No cycle initially
    EXPECT_FALSE(list.detectCycle());

    // Create cycle
    auto last = list.getLastNode();
    last->next = list.getHead();
    
    // Cycle should be detected
    EXPECT_TRUE(list.detectCycle());
}

// Test edge cases
TEST_F(LinkedListTest, EdgeCases) {
    // Test empty list operations
    EXPECT_FALSE(list.deleteByValue(1));
    EXPECT_FALSE(list.search(1));
    EXPECT_EQ(list.length(), 0);
    
    // Test invalid position in insertAtMiddle
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    EXPECT_THROW(list.insertAtMiddle(3, 10), std::out_of_range);  // Invalid position
    EXPECT_EQ(list.length(), 2);
}

// Test print functionality
TEST_F(LinkedListTest, Print) {
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    
    // Redirect cout to string stream
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    list.print();
    
    std::cout.rdbuf(old);
    EXPECT_EQ(buffer.str(), "1 -> 2 -> 3 -> NULL\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

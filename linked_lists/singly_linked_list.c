/**
 * @file singly_linked_list.c
 * @brief Implementation of a singly linked list in C
 *
 * This file demonstrates a basic singly linked list implementation with
 * operations like creation, insertion, deletion, and traversal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Structure for a node in the singly linked list
 */
typedef struct Node {
    int data;               // Data stored in the node
    struct Node* next;      // Pointer to the next node
} Node;

/**
 * @brief Structure for the linked list
 */
typedef struct {
    Node* head;             // Pointer to the first node
    size_t size;            // Number of nodes in the list
} LinkedList;

/**
 * @brief Create a new empty linked list
 * 
 * @return LinkedList* Pointer to the newly created linked list
 */
LinkedList* create_list() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    list->head = NULL;
    list->size = 0;
    
    return list;
}

/**
 * @brief Check if the linked list is empty
 * 
 * @param list Pointer to the linked list
 * @return true if the list is empty, false otherwise
 */
bool is_empty(const LinkedList* list) {
    return (list->head == NULL);
}

/**
 * @brief Create a new node with the given data
 * 
 * @param data Data to be stored in the node
 * @return Node* Pointer to the newly created node
 */
Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    new_node->data = data;
    new_node->next = NULL;
    
    return new_node;
}

/**
 * @brief Insert a node at the beginning of the linked list
 * 
 * @param list Pointer to the linked list
 * @param data Data to be inserted
 */
void insert_at_beginning(LinkedList* list, int data) {
    Node* new_node = create_node(data);
    
    // If the list is empty, the new node becomes the head
    if (is_empty(list)) {
        list->head = new_node;
    } else {
        // Otherwise, insert at the beginning
        new_node->next = list->head;
        list->head = new_node;
    }
    
    list->size++;
}

/**
 * @brief Insert a node at the end of the linked list
 * 
 * @param list Pointer to the linked list
 * @param data Data to be inserted
 */
void insert_at_end(LinkedList* list, int data) {
    Node* new_node = create_node(data);
    
    // If the list is empty, the new node becomes the head
    if (is_empty(list)) {
        list->head = new_node;
    } else {
        // Otherwise, find the last node and append
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    list->size++;
}

/**
 * @brief Insert a node at a specific position in the linked list
 * 
 * @param list Pointer to the linked list
 * @param data Data to be inserted
 * @param position Position at which to insert (0-based index)
 * @return true if insertion was successful, false otherwise
 */
bool insert_at_position(LinkedList* list, int data, size_t position) {
    // Check if position is valid
    if (position > list->size) {
        return false;
    }
    
    // If position is 0, insert at the beginning
    if (position == 0) {
        insert_at_beginning(list, data);
        return true;
    }
    
    // If position is at the end, insert at the end
    if (position == list->size) {
        insert_at_end(list, data);
        return true;
    }
    
    // Otherwise, insert at the specified position
    Node* new_node = create_node(data);
    Node* current = list->head;
    
    // Traverse to the node just before the desired position
    for (size_t i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    // Insert the new node
    new_node->next = current->next;
    current->next = new_node;
    
    list->size++;
    return true;
}

/**
 * @brief Delete a node from the beginning of the linked list
 * 
 * @param list Pointer to the linked list
 * @return true if deletion was successful, false if the list was empty
 */
bool delete_from_beginning(LinkedList* list) {
    if (is_empty(list)) {
        return false;
    }
    
    Node* temp = list->head;
    list->head = list->head->next;
    free(temp);
    
    list->size--;
    return true;
}

/**
 * @brief Delete a node from the end of the linked list
 * 
 * @param list Pointer to the linked list
 * @return true if deletion was successful, false if the list was empty
 */
bool delete_from_end(LinkedList* list) {
    if (is_empty(list)) {
        return false;
    }
    
    // If there's only one node
    if (list->head->next == NULL) {
        free(list->head);
        list->head = NULL;
        list->size--;
        return true;
    }
    
    // Find the second-to-last node
    Node* current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    
    // Delete the last node
    free(current->next);
    current->next = NULL;
    
    list->size--;
    return true;
}

/**
 * @brief Delete a node at a specific position in the linked list
 * 
 * @param list Pointer to the linked list
 * @param position Position from which to delete (0-based index)
 * @return true if deletion was successful, false otherwise
 */
bool delete_from_position(LinkedList* list, size_t position) {
    // Check if position is valid
    if (position >= list->size || is_empty(list)) {
        return false;
    }
    
    // If position is 0, delete from the beginning
    if (position == 0) {
        return delete_from_beginning(list);
    }
    
    // If position is at the end, delete from the end
    if (position == list->size - 1) {
        return delete_from_end(list);
    }
    
    // Otherwise, delete from the specified position
    Node* current = list->head;
    
    // Traverse to the node just before the one to be deleted
    for (size_t i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    // Delete the node
    Node* temp = current->next;
    current->next = temp->next;
    free(temp);
    
    list->size--;
    return true;
}

/**
 * @brief Search for a value in the linked list
 * 
 * @param list Pointer to the linked list
 * @param data Data to search for
 * @return int Position of the first occurrence (0-based), -1 if not found
 */
int search(const LinkedList* list, int data) {
    if (is_empty(list)) {
        return -1;
    }
    
    Node* current = list->head;
    int position = 0;
    
    while (current != NULL) {
        if (current->data == data) {
            return position;
        }
        current = current->next;
        position++;
    }
    
    return -1;  // Not found
}

/**
 * @brief Display the contents of the linked list
 * 
 * @param list Pointer to the linked list
 */
void display(const LinkedList* list) {
    if (is_empty(list)) {
        printf("List is empty\n");
        return;
    }
    
    printf("List (size %zu): ", list->size);
    
    Node* current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL) {
            printf(" -> ");
        }
    }
    
    printf("\n");
}

/**
 * @brief Free all memory used by the linked list
 * 
 * @param list Pointer to the linked list
 */
void destroy_list(LinkedList* list) {
    Node* current = list->head;
    Node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

/**
 * @brief Reverse the linked list
 * 
 * @param list Pointer to the linked list
 */
void reverse(LinkedList* list) {
    if (is_empty(list) || list->size == 1) {
        return;  // Nothing to reverse
    }
    
    Node* prev = NULL;
    Node* current = list->head;
    Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;  // Store the next node
        current->next = prev;  // Reverse the current node's pointer
        
        // Move pointers one position ahead
        prev = current;
        current = next;
    }
    
    list->head = prev;  // Update the head to the new first node
}

/**
 * @brief Get the middle node of the linked list
 * 
 * Uses the slow and fast pointer technique (Floyd's cycle-finding algorithm)
 * 
 * @param list Pointer to the linked list
 * @return Node* Pointer to the middle node, NULL if the list is empty
 */
Node* get_middle_node(const LinkedList* list) {
    if (is_empty(list)) {
        return NULL;
    }
    
    Node* slow = list->head;
    Node* fast = list->head;
    
    // Fast pointer moves twice as fast as the slow pointer
    // When fast reaches the end, slow will be at the middle
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

/**
 * @brief Check if the linked list has a cycle
 * 
 * Uses Floyd's cycle-finding algorithm (tortoise and hare)
 * 
 * @param list Pointer to the linked list
 * @return true if a cycle is detected, false otherwise
 */
bool has_cycle(const LinkedList* list) {
    if (is_empty(list)) {
        return false;
    }
    
    Node* slow = list->head;
    Node* fast = list->head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Move one step
        fast = fast->next->next;    // Move two steps
        
        // If slow and fast pointers meet, there's a cycle
        if (slow == fast) {
            return true;
        }
    }
    
    return false;  // No cycle detected
}

/**
 * @brief Main function to demonstrate the linked list operations
 */
int main() {
    // Create a new linked list
    LinkedList* list = create_list();
    
    // Check if the list is empty
    printf("Is the list empty? %s\n", is_empty(list) ? "Yes" : "No");
    
    // Insert elements at the beginning
    printf("\nInserting elements at the beginning...\n");
    insert_at_beginning(list, 10);
    insert_at_beginning(list, 20);
    insert_at_beginning(list, 30);
    display(list);
    
    // Insert elements at the end
    printf("\nInserting elements at the end...\n");
    insert_at_end(list, 40);
    insert_at_end(list, 50);
    display(list);
    
    // Insert at a specific position
    printf("\nInserting 35 at position 2...\n");
    insert_at_position(list, 35, 2);
    display(list);
    
    // Search for elements
    printf("\nSearching for elements...\n");
    printf("Position of 30: %d\n", search(list, 30));
    printf("Position of 35: %d\n", search(list, 35));
    printf("Position of 100: %d\n", search(list, 100));
    
    // Delete from beginning
    printf("\nDeleting from the beginning...\n");
    delete_from_beginning(list);
    display(list);
    
    // Delete from end
    printf("\nDeleting from the end...\n");
    delete_from_end(list);
    display(list);
    
    // Delete from position
    printf("\nDeleting from position 1...\n");
    delete_from_position(list, 1);
    display(list);
    
    // Get middle node
    printf("\nMiddle node: %d\n", get_middle_node(list)->data);
    
    // Reverse the list
    printf("\nReversing the list...\n");
    reverse(list);
    display(list);
    
    // Check for cycle
    printf("\nDoes the list have a cycle? %s\n", has_cycle(list) ? "Yes" : "No");
    
    // Create a cycle for demonstration (uncomment to test)
    /*
    if (!is_empty(list) && list->size >= 3) {
        Node* last = list->head;
        while (last->next != NULL) {
            last = last->next;
        }
        
        // Point the last node to the second node to create a cycle
        Node* second = list->head->next;
        last->next = second;
        
        printf("\nCreated a cycle for demonstration\n");
        printf("Does the list have a cycle now? %s\n", has_cycle(list) ? "Yes" : "No");
        
        // Break the cycle before freeing memory
        last->next = NULL;
    }
    */
    
    // Clean up
    destroy_list(list);
    printf("\nLinked list destroyed\n");
    
    return 0;
}

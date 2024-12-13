#include <stdio.h>
#include <stdlib.h>

// Structure for a node in linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert at end of list
void insertEnd(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Function to display the linked list
void displayList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

// Function to swap even numbers in the linked list
void swapEvenNumbers(struct Node* head) {
    struct Node *current = head;
    
    // Store even numbers in array
    int evenArr[100], evenCount = 0;
    while (current != NULL) {
        if (current->data % 2 == 0) {
            evenArr[evenCount++] = current->data;
        }
        current = current->next;
    }
    
    // Sort even numbers
    for (int i = 0; i < evenCount - 1; i++) {
        for (int j = 0; j < evenCount - i - 1; j++) {
            if (evenArr[j] > evenArr[j + 1]) {
                int temp = evenArr[j];
                evenArr[j] = evenArr[j + 1];
                evenArr[j + 1] = temp;
            }
        }
    }
    
    // Replace even numbers in linked list
    current = head;
    int evenIndex = 0;
    while (current != NULL) {
        if (current->data % 2 == 0) {
            current->data = evenArr[evenIndex++];
        }
        current = current->next;
    }
}

int main() {
    struct Node* head = NULL;
    
    // Insert the input numbers
    insertEnd(&head, 1);
    insertEnd(&head, 8);
    insertEnd(&head, 7);
    insertEnd(&head, 4);
    insertEnd(&head, 3);
    
    printf("Original list: ");
    displayList(head);
    
    swapEvenNumbers(head);
    
    printf("After swapping even numbers: ");
    displayList(head);
    
    return 0;
}

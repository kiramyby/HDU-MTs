#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createList() {
    Node* head = (Node*)malloc(sizeof(Node));
    head->next = NULL;
    return head;
}

void insertSorted(Node* head, int value) {
    Node* current = head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = current->next;
    current->next = newNode;
}

void displayList(Node* head) {
    Node* current = head->next;
    if (current != NULL) {
        printf("%d", current->data);
        current = current->next;
    }
    while (current != NULL) {
        printf("->%d", current->data);
        current = current->next;
    }
    printf("\n");
}

Node* splitList(Node* original) {
    Node* evenList = createList();
    Node* evenCurrent = evenList;
    Node* current = original;
    Node* next;

    while (current->next != NULL) {
        if (current->next->data % 2 == 0) {
            // Move even node to evenList
            next = current->next;
            current->next = next->next;
            next->next = evenCurrent->next;
            evenCurrent->next = next;
            evenCurrent = next;
        } else {
            current = current->next;
        }
    }

    return evenList;
}

void destroyList(Node* head) {
    Node* current = head->next;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
}

int main() {
    Node* originalList = createList();
    int num;

    while (scanf("%d", &num) != EOF) {
        if (num > 0) {
            insertSorted(originalList, num);
        }
    }

    // Display original list
    displayList(originalList);

    // Split list and display both parts
    Node* evenList = splitList(originalList);
    displayList(originalList);  // Now contains only odd numbers
    displayList(evenList);      // Contains even numbers

    // Cleanup
    destroyList(originalList);
    destroyList(evenList);

    return 0;
}

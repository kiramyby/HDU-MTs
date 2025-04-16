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

Node* mergeLists(Node* list1, Node* list2) {
    Node* mergedHead = createList();
    Node* current1 = list1->next;
    Node* current2 = list2->next;
    Node* currentMerged = mergedHead;

    while (current1 != NULL && current2 != NULL) {
        if (current1->data <= current2->data) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->data = current1->data;
            currentMerged->next = newNode;
            current1 = current1->next;
        } else {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->data = current2->data;
            currentMerged->next = newNode;
            current2 = current2->next;
        }
        currentMerged = currentMerged->next;
        currentMerged->next = NULL;
    }

    while (current1 != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = current1->data;
        currentMerged->next = newNode;
        currentMerged = currentMerged->next;
        currentMerged->next = NULL;
        current1 = current1->next;
    }

    while (current2 != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = current2->data;
        currentMerged->next = newNode;
        currentMerged = currentMerged->next;
        currentMerged->next = NULL;
        current2 = current2->next;
    }

    return mergedHead;
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
    Node* positiveList = createList();
    Node* negativeList = createList();
    int num;

    while (scanf("%d", &num) != EOF) {
        if (num >= 0) {
            insertSorted(positiveList, num);
        } else {
            insertSorted(negativeList, num);
        }
    }

    displayList(positiveList);
    displayList(negativeList);

    Node* mergedList = mergeLists(negativeList, positiveList);
    displayList(mergedList);

    destroyList(positiveList);
    destroyList(negativeList);
    destroyList(mergedList);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

void insert(Node *head, int val) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = val;
    newNode->next = NULL;

    Node *current = head;
    while (current->next != NULL && current->next->data <= val) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void printList(Node *head) {
    Node *current = head->next;
    if (current == NULL) {
        printf("\n");
        return;
    }
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf("->");
        }
        current = current->next;
    }
    printf("\n");
}

void destroyList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    Node *posHead = (Node *)malloc(sizeof(Node));
    posHead->next = NULL;
    Node *negHead = (Node *)malloc(sizeof(Node));
    negHead->next = NULL;

    int num;
    while (scanf("%d", &num) != EOF) {
        if (num > 0) {
            insert(posHead, num);
        } else if (num < 0) {
            insert(negHead, num);
        }
    }

    printList(posHead);
    printList(negHead);

    destroyList(posHead);
    destroyList(negHead);

    return 0;
}
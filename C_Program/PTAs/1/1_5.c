#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int number;
    struct Node* next;
} Node;

void Append(Node** tail, int number) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->number = number;
    
    if (*tail == NULL) {
        // First node
        newNode->next = newNode;
        *tail = newNode;
    } else {
        // Add to end and maintain circular nature
        newNode->next = (*tail)->next;
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

void josephus(int M, int N) {
    if (M <= 0 || N <= 0) return;

    // Create circular list with M people
    Node* tail = NULL;
    for (int i = 1; i <= M; i++) {
        Append(&tail, i);
    }

    Node* current = tail;
    
    // Process until all nodes are removed
    while (M > 0) {
        // Move N-1 steps (Nth person will be next)
        for (int i = 1; i < N; i++) {
            current = current->next;
        }
        
        // Remove the Nth person
        Node* toDelete = current->next;
        printf("%4d", toDelete->number);
        
        if (toDelete == toDelete->next) {
            // Last node
            free(toDelete);
            break;
        }
        
        current->next = toDelete->next;
        free(toDelete);
        M--;
    }
    printf("\n");
}

int main() {
    int M, N;
    scanf("%d %d", &M, &N);
    josephus(M, N);
    return 0;
}

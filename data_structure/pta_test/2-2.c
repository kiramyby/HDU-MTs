#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100

typedef char ElemType;
typedef struct node {
    ElemType data;                // 数据元素
    struct node *lchild;          // 指向左孩子
    struct node *rchild;          // 指向右孩子
} BTNode;

void TravLevelLeaf(BTNode *b)
{
    if (b == NULL)
        return;

    // Create a queue for level order traversal
    BTNode *queue[MaxSize];
    int front = 0, rear = 0;

    // Enqueue root
    queue[rear++] = b;

    while (front < rear) {
        // Dequeue a node
        BTNode *current = queue[front++];

        // Check if it's a leaf node
        if (current->lchild == NULL && current->rchild == NULL) {
            printf("%c ", current->data);
        }

        // Enqueue left child if it exists
        if (current->lchild != NULL) {
            queue[rear++] = current->lchild;
        }

        // Enqueue right child if it exists
        if (current->rchild != NULL) {
            queue[rear++] = current->rchild;
        }
    }
}
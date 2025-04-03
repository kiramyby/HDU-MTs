/*1.10 编写程序选用顺序存储结构和链式存储结构实现抽象数据类型栈和队列，再利用栈和队列，输入若干个整数，将输入后的正整数和负整数分别保存起来，输入完成后，首先将以输入相反的次序输出所有保存的正整数，再以输入相同次序输出所有保存的负整数，。

输入格式:
若干非0整数。

输出格式:
正整数和负整数输出各占一行​，每个数占5位。

输入样例:
100 2 3 -2 -8 -6 -9 -10 50 2 -1
输出样例:
    2   50    3    2  100
   -2   -8   -6   -9  -10   -1*/

#include <stdio.h>
#include <stdlib.h>

// Stack structure for positive integers
typedef struct Stack {
    int data[100];
    int top;
} Stack;

// Queue structure for negative integers
typedef struct Queue {
    int data[100];
    int front, rear;
} Queue;

// Stack operations
void initStack(Stack *s) {
    s->top = -1;
}

int isStackEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, int value) {
    s->data[++(s->top)] = value;
}

int pop(Stack *s) {
    return s->data[(s->top)--];
}

// Queue operations
void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

int isQueueEmpty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, int value) {
    q->data[(q->rear)++] = value;
}

int dequeue(Queue *q) {
    return q->data[(q->front)++];
}

int main() {
    Stack positiveStack;
    Queue negativeQueue;
    int num;

    initStack(&positiveStack);
    initQueue(&negativeQueue);

    // Modified input handling: read until end of input
    while (scanf("%d", &num) == 1) {
        if (num > 0) {
            push(&positiveStack, num);
        } else if (num < 0) {
            enqueue(&negativeQueue, num);
        }
        // Ignore 0 if it appears in input
    }

    // Output positive integers in reverse order
    while (!isStackEmpty(&positiveStack)) {
        printf("%5d", pop(&positiveStack));
    }
    printf("\n");

    // Output negative integers in the same order
    while (!isQueueEmpty(&negativeQueue)) {
        printf("%5d", dequeue(&negativeQueue));
    }
    printf("\n");

    return 0;
}
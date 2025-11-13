#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

// 邻接表节点
struct Node {
    int vertex;
    struct Node* next;
};

// 图结构
struct Graph {
    int numVertices;
    int visited[MAX_VERTICES];
    struct Node** adjLists;
};

// 栈结构体定义
struct Stack {
    int items[MAX_VERTICES];
    int top;
};

// 初始化栈
void initStack(struct Stack* stack) {
    stack->top = -1;
}

// 压入元素到栈
void push(struct Stack* stack, int vertex) {
    stack->items[++(stack->top)] = vertex;
}

// 从栈弹出元素
int pop(struct Stack* stack) {
    return stack->items[(stack->top)--];
}

// 检查栈是否为空
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

// 深度优先搜索
void DFS(struct Graph* graph, int startVertex) {
    struct Stack stack;
    initStack(&stack);
    
    push(&stack, startVertex);
    graph->visited[startVertex] = 1;
    
    while (!isEmpty(&stack)) {
        int currentVertex = pop(&stack);
        printf("已访问 %d\n", currentVertex);
        
        struct Node* temp = graph->adjLists[currentVertex];
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            
            if (graph->visited[adjVertex] == 0) {
                graph->visited[adjVertex] = 1;
                push(&stack, adjVertex);
            }
            temp = temp->next;
        }
    }
}
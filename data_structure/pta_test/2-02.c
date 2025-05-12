// 本题要求实现对图的广度优先遍历，并输出结点信息。 本题中图的表示采用邻接矩阵表示方法。
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>//使用INT_MAX宏，指定整数变量不能存储超出此限制的任何值

typedef int DataType;
struct Node
{
    DataType data;
    struct Node    *link;
};
typedef struct Node *PNode;
struct Queue
{
    PNode        f;
    PNode        r;
};
typedef struct Queue *LinkQueue;
LinkQueue  SetNullQueue_Link()
{
    LinkQueue lqueue;
    lqueue = (LinkQueue)malloc(sizeof(struct Queue));
    if (lqueue != NULL)
    {
        lqueue->f = NULL;
        lqueue->r = NULL;
    }
    else
        printf("Alloc failure! \n");
    return  lqueue;
}

int IsNullQueue_Link(LinkQueue lqueue)
{
    return (lqueue->f == NULL);
}

void EnQueue_link(LinkQueue lqueue, DataType x)
{
    PNode  p;
    p = (PNode)malloc(sizeof(struct Node));
    if (p == NULL)
        printf("Alloc failure!");
    else{
        p->data = x;
        p->link = NULL;
        if (lqueue->f == NULL) 
        {
            lqueue->f = p;
            lqueue->r = p;
        }
        else
        {
            lqueue->r->link = p;
            lqueue->r = p;
        }
    }
}
void DeQueue_link(LinkQueue lqueue)
{
    struct Node  * p;
    if (lqueue->f == NULL)
        printf("It is empty queue!\n ");
    else
    {
        p = lqueue->f;
        lqueue->f = lqueue->f->link;
        free(p);
    }
}
DataType  FrontQueue_link(LinkQueue lqueue)
{
    if (lqueue->f == NULL)
    {
        printf("It is empty queue!\n");
        return 0;
    }
    else
        return (lqueue->f->data);
}
typedef struct    GRAPHMATRIX_STRU
{
    int size; // 图中结点的个数 
    int **graph; //二维数组保存图 
}GraphMatrix;

GraphMatrix* InitGraph(int num)
{
    int i;
    int j;
    GraphMatrix* graphMatrix = (GraphMatrix*)malloc(sizeof(GraphMatrix));
    graphMatrix->size = num;
    graphMatrix->graph = (int**)malloc(sizeof(int*)* graphMatrix->size);
    for (i = 0; i<graphMatrix->size; i++)
    {
        graphMatrix->graph[i] = (int*)malloc(sizeof(int)* graphMatrix->size);
    }

    for (i = 0; i<graphMatrix->size; i++)
    {
        for (j = 0; j<graphMatrix->size; j++)
        {
            graphMatrix->graph[i][j] = INT_MAX;
        }
    }
    return graphMatrix;
}


void ReadGraph(GraphMatrix* graphMatrix)
{
    int vex1, vex2, weight;
    /** 输入方式为点 点 权值，权值为0，则输入结束 **/
    scanf("%d %d %d", &vex1, &vex2, &weight);
    while (weight != 0)
    {
        graphMatrix->graph[vex1][vex2] = weight;
        scanf("%d %d %d", &vex1, &vex2, &weight);
    }
}

/*从结点i开始广度优先遍历 */
void BFS(GraphMatrix* graphMatrix, int *visited, int i)
{
    int j;
    int tempVex;
    LinkQueue waitingQueue = NULL;
    waitingQueue = SetNullQueue_Link();
    if (!visited[i])
    {
        visited[i] = 1;
        printf("%d ", i);
        EnQueue_link(waitingQueue, i);
        while (!IsNullQueue_Link(waitingQueue))
        {
            tempVex = FrontQueue_link(waitingQueue);
            DeQueue_link(waitingQueue);

            for (j = 0; j < graphMatrix->size; j++)
            {
                if (graphMatrix->graph[tempVex][j] != INT_MAX && !visited[j])
                {
                    visited[j] = 1; // 标记为已访问
                    EnQueue_link(waitingQueue, j); // 加入队列
                    printf("%d ", j); // 输出节点
                }
            }
        }
    }
}

void BFSGraphMatrix(GraphMatrix* graphMatrix)
{
    int i;
    int *visited = (int*)malloc(sizeof(int) * graphMatrix->size);
    
    /** 设置所有结点都没有被访问，其中1为访问过，0为没有被访问 **/
    for (i = 0; i < graphMatrix->size; i++)
        visited[i] = 0;

    /** 从0号结点开始进行广度优先遍历 **/
    for (i = 0; i < graphMatrix->size; i++)
    {
        BFS(graphMatrix, visited, i);
    }

    free(visited); // 释放内存
}
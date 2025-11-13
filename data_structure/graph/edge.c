/*
 * 删除无向图中的边(i,j)
 * G为邻接表表示的图
 * i, j为要删除边的两个顶点编号
 */

#include <stdio.h>
#include <stdlib.h>

// 图的邻接表表示
typedef struct ArcNode {  // 边表结点
    int adjvex;           // 邻接点域
    struct ArcNode *next; // 指向下一个邻接点的指针
} ArcNode;

typedef struct VNode {    // 顶点表结点
    int data;             // 顶点信息
    ArcNode *firstarc;    // 指向第一个邻接点的指针
} VNode, AdjList[100];    // 假设最多100个顶点

typedef struct {
    AdjList vertices;     // 邻接表
    int vexnum, arcnum;   // 图的顶点数和边数
} ALGraph;

// 删除边(i, j)的算法
int DeleteArc(ALGraph *G, int i, int j) {
    if (i < 0 || i >= G->vexnum || j < 0 || j >= G->vexnum) {
        return 0;  // 顶点编号错误
    }
    
    // 删除顶点i的邻接表中的j
    ArcNode *p = G->vertices[i].firstarc;
    ArcNode *pre = NULL;
    
    // 找到j结点在i的邻接表中的位置
    while (p != NULL && p->adjvex != j) {
        pre = p;
        p = p->next;
    }
    
    // 如果找到了j结点
    if (p != NULL) {
        // 如果j是i的第一个邻接点
        if (pre == NULL) {
            G->vertices[i].firstarc = p->next;
        } else {
            pre->next = p->next;
        }
        free(p);
    }
    
    // 删除顶点j的邻接表中的i（无向图需要删除两次）
    p = G->vertices[j].firstarc;
    pre = NULL;
    
    // 找到i结点在j的邻接表中的位置
    while (p != NULL && p->adjvex != i) {
        pre = p;
        p = p->next;
    }
    
    // 如果找到了i结点
    if (p != NULL) {
        // 如果i是j的第一个邻接点
        if (pre == NULL) {
            G->vertices[j].firstarc = p->next;
        } else {
            pre->next = p->next;
        }
        free(p);
        
        // 边数减1
        G->arcnum--;
        return 1;  // 删除成功
    }
    
    return 0;  // 边不存在
}

// 测试函数
void testDeleteArc() {
    // 这里可以添加测试代码
    printf("Delete edge algorithm for undirected graph using adjacency list.\n");
}

int main() {
    testDeleteArc();
    return 0;
}
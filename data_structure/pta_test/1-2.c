#include <stdio.h>
#include <stdlib.h>

typedef int DataType;

typedef struct Node {
    DataType data;
    struct Node* next;
} LNode, *PNode, *LinkList;

// 初始化单链表，开辟头结点
int InitLinkList(LinkList* head) {
    *head = (LinkList)malloc(sizeof(LNode));
    if (!*head) {
        return 0;
    }
    (*head)->next = NULL;
    return 1;
}

// 销毁单链表
void DestroyLinkList(LinkList h) {
    PNode p = h->next;
    while (h) {
        p = h;
        h = h->next;
        free(p);
    }
}

// 遍历单链表
void TraverseLinkList(LinkList h, int t) {
    PNode p = h->next;
    if(p){ printf("%d", p->data); p=p->next;}
    while (p) {
        printf(" %d", p->data);
        p = p->next;
    }
    if(t>0) printf("\n");
}

// 创建单链表
void CreateLinkList(LinkList h, int n, int arr[]) {
    PNode tail = h;
    for (int i = 0; i < n; i++) {
        PNode newNode = (PNode)malloc(sizeof(LNode));
        newNode->data = arr[i];
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
}

// 求差集
void Difference(LinkList A, LinkList B) {
    PNode prev = A, curr = A->next, b = B->next;
    while (curr && b) {
        if (curr->data < b->data) {
            prev = curr;
            curr = curr->next;
        } else if (curr->data > b->data) {
            b = b->next;
        } else {
            // 删除A中的当前节点
            prev->next = curr->next;
            free(curr);
            curr = prev->next;
            b = b->next;
        }
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        LinkList A, B;
        InitLinkList(&A);
        InitLinkList(&B);

        int n, m;
        scanf("%d", &n);
        int A_arr[n];
        for (int i = 0; i < n; i++) {
            scanf("%d", &A_arr[i]);
        }
        CreateLinkList(A, n, A_arr);

        scanf("%d", &m);
        int B_arr[m];
        for (int i = 0; i < m; i++) {
            scanf("%d", &B_arr[i]);
        }
        CreateLinkList(B, m, B_arr);

        Difference(A, B);
        TraverseLinkList(A,T);

        DestroyLinkList(A);
        DestroyLinkList(B);
    }
    return 0;
}
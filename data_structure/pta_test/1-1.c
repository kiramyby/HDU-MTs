#include<stdio.h>
#include<stdlib.h>
typedef int DataType;
typedef struct Node
{
    DataType data;
    struct Node* next;
}LNode, * PNode, * LinkList;
int InitLinkList(LinkList* head)
{  // 初始化单链表，开辟头结点
    *head = (LinkList)malloc(sizeof(LNode));
    if (!head)
    {
        printf("初始化链表错误!\n");
        return 0;
    }
    (*head)->next = NULL;
    return 1;
}
int LinkListInsert(LinkList h, int pos, DataType x)
{  // 在单链表的第pos个位置插入新开辟的x值链点
    PNode p = h, q;
    int i = 0;
    while (p && i < pos - 1)
    {
        p = p->next;
        i++;
    }
    if (!p || i > pos - 1)
    {
        printf("插入位置不合法!\n");
        return 0;
    }
    q = (PNode)malloc(sizeof(LNode));
    if (!q)
    {
        printf("不能生成新结点\n");
        return 0;
    }
    q->data = x;
    q->next = p->next;
    p->next = q;
    return 1;
}
void DestroyLinkList(LinkList h)
{ // 销毁单链表
    PNode p = h->next;
    while (h)
    {
        p = h;
        h = h->next;
        free(p);
    }
}
void TraverseLinkList(LinkList h)
{  // 遍历单链表
    PNode p = h->next;
    while (p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}
PNode findN(LinkList a, int n)
{ // 找到单链表的第n个链点，返回其指针
    int cnt = 1;
    PNode p = a;
    while (p && cnt++ <= n)
        p = p->next;
    return p;
}

int SwapN_inList( LinkList a , int posN , int *len);

int main()
{
    LinkList h;
    char ch;
    PNode s1 = NULL, s2 = NULL, s3 = NULL, s4 = NULL;
    DataType x, N;
    int length = 0, pos = 1;
    InitLinkList(&h);
    do
    {
        scanf("%d", &x);  // 某些编译器要求此处改为scanf_s
        LinkListInsert(h, pos++, x);
    } while ((ch = getchar()) != '\n');
    scanf("%d", &N);  // 某些编译器要求此处改为scanf_s
    s1 = findN(h, N);
    if (s1)
    {
        s2 = s1->next;
    }
    if (SwapN_inList(h, N, &length) == 1)
    {
        s3 = findN(h, N);
        if (s3)
        {
            s4 = findN(h, N + 1);
        }
        if (s1 == s4 && s2 == s3)
        {
            printf("交换第%d个和第%d个链点位置后的单链表A是\n", N, N + 1);
            TraverseLinkList(h);
        }
    }
    else
    {
        printf("单链表A的第%d个结点为尾结点，没有下一个结点与之交换\n", length);
    }
    DestroyLinkList(h);
    return 0;
}
/* 请在这里填写答案 */

int SwapN_inList(LinkList a, int posN, int *len) {
    PNode prev = a;
    int count = 0;

    while (prev->next && count < posN - 1) {
        prev = prev->next;
        count++;
    }

    *len = count + 1;
    PNode nNode = prev->next;  
    if (!nNode || !nNode->next) {
        return 0;
    }

    PNode nPlusOneNode = nNode->next;
    nNode->next = nPlusOneNode->next;
    nPlusOneNode->next = nNode;
    prev->next = nPlusOneNode;

    return 1;
}

int SwapN_inList (LinkList a, int posN, int *len) {
    PNode  prev = a;
    int count = 0;

    while  (prev->next && count < posN - 1) {
        prev = prev->next;
        count++;


        }
}
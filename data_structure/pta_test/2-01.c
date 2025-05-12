// 假设二叉树采用二叉链存储，完成以下二叉树先序遍历的非递归算法。
// 提示：以下代码采用栈实现先序遍历的非递归算法。
#include <stdio.h>
#define MaxSize 100
typedef char ElemType;
typedef struct node 
{    
  ElemType data;    //数据元素
  struct node *lchild;    //指向左孩子节点
  struct node *rchild;    //指向右孩子节点
} BTNode;
typedef struct 
{
  BTNode *data[MaxSize];    //存放栈中的数据元素
  int top;                //存放栈顶指针，即栈顶元素在data数组中的下标
} SqStack;            //顺序栈类型
void InitStack(SqStack *&s);    //初始化栈;
void DestroyStack(SqStack *&s);    //销毁栈;
bool Push(SqStack *&s,BTNode *e);//进栈
bool Pop(SqStack *&s,BTNode *&e);//出栈
bool GetTop(SqStack *s,BTNode *&e);//取栈顶元素
void DestroyBTree(BTNode *&b);     //销毁栈

void PreOrder1(BTNode *b)    //先序非递归遍历算法
{
    BTNode *p;
    SqStack *st;                    
    InitStack(st);                    
    Push(st, b);                
    while (st->top != -1)        // 栈不为空
    {
        Pop(st, p);                // 出栈
        printf("%c ", p->data);    // 访问节点p
        if (p->rchild != NULL)    // 如果右孩子存在
            Push(st, p->rchild);  // 右孩子进栈
        if (p->lchild != NULL)    // 如果左孩子存在
            Push(st, p->lchild);  // 左孩子进栈
    }
    printf("\n");
    DestroyStack(st);                
}
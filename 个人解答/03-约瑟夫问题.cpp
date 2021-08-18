#include<stdio.h>  
#include<stdlib.h>  
  
  
typedef int Status;  
#define OK 1  
#define ERROR 0  
#define OVERFLOW -2  
  
typedef int ElemType;  
  
#define HEAD_INSERTION 1  
#define TAIL_INSERTION 0  
  
#define DELETED 1  
#define UNDELETED 0  
  
typedef struct Lnode {  
    ElemType data;  
    int flag;   //flag为DELETED表示已删除，UNDELETED表示未删除  
    struct Lnode *next;  
}LNode, *LinkList;  
  
Status InitList_CL(LinkList &L) {  
    //初始化操作：构造一个空的（带头结点的）循环链表  
    L = (LinkList)malloc(sizeof(LNode));  
    if (!L) exit(OVERFLOW);  
    L->next = L; //循环  
    L->flag = DELETED;   //技巧：把头结点标记为已删除，在PaceForward的时候会自动跳过头结点  
    return OK;  
}  
  
Status ListInsert_CL(LinkList &L, int i, ElemType e) {  
    //插入操作：在（带头结点的）循环链表的某个位置插入一个新的元素  
    //i表示插入元素的位序，e表示插入的元素  
    LinkList s; int k; LinkList cursor = L;  
    if (!L || i < 1) exit(ERROR);    //i不合法，L是空指针  
    s = (LinkList)malloc(sizeof(LNode));    //新结点  
    s->data = e;  
    s->flag = UNDELETED;  
    for (k = 0; k < i - 1; k++)  
    {  
        if (cursor->next == L)   exit(ERROR);    //i不合法  
        cursor = cursor->next;  
    }  
    s->next = cursor->next;  
    cursor->next = s;  
    return OK;  
}  
  
Status Create_ONE_TO_N_List_CL(LinkList &L, int InsertType, int n) {  
    //创建从数字1到n的（带头结点的）循环链表  
    //InsertType表示插入类型，HEAD_INSERTION为头插法，TAIL_INSERTION为尾插法  
    //n表示循环链表的长度  
    int i;  
    InitList_CL(L);  
    for (i = 1; i <= n; i++)  
    {  
        if (InsertType == HEAD_INSERTION)  
            ListInsert_CL(L, 1, i);  
        else if (InsertType == TAIL_INSERTION)  
            ListInsert_CL(L, i, i);  
        else  
            exit(ERROR);  
    }  
    return OK;  
}  
  
Status GetElem_L(LinkList L, int i, LinkList &pe, ElemType &e) {  
    //取元素操作  
    //i为要获取的元素位序，pe存放指向该元素的指针，e存放获取到的元素  
    int k; LinkList cursor = L;  
    if (!L || i < 0) exit(ERROR);    //i不合法，L是空指针  
    if (i == 0)  
    {  
        pe = L;  
        return OK;  
    }  
    cursor = cursor->next;  
    for (k = 0; k < i - 1; k++)  
    {  
        if (cursor->next == L)   exit(ERROR);    //i不合法  
        cursor = cursor->next;  
    }  
    pe = cursor;  
    e = cursor->data;  
    return OK;  
}  
  
Status PaceForward_L(LinkList L, LinkList start, int i, LinkList &destination, ElemType &e) {  
    //在某个位置的基础上向前进i个位序（跳过已删除的）  
    //start为指向起点结点的指针，i为要前进的步数，destination存放指向终点结点的指针，e存放终点结点的元素  
    int k; LinkList cursor = start;  
    if (!L || i < 1) exit(ERROR);    //i不合法，L是空指针  
  
    for (k = 0; k < i; k++)  
    {  
        while (cursor->next->flag == DELETED) cursor = cursor->next;  
        cursor = cursor->next;  
    }  
    destination = cursor;  
    e = cursor->data;  
    return OK;  
}  
  
int main() {  
    LinkList L, cursor; int n, k, m; ElemType e; int counter = 0;  
    scanf_s("%d,%d,%d", &n, &k, &m);  
    if (n < 1 || k < 1 || m < 1)  
    {  
        printf("n,m,k must bigger than 0.\n");  
        exit(0);  
    }  
    if (k > n)  
    {  
        printf("k should not bigger than n.\n");  
        exit(0);  
    }  
  
    Create_ONE_TO_N_List_CL(L, TAIL_INSERTION, n);  
  
    GetElem_L(L, k - 1, cursor, e); //因为第k个人报数1，相当于第k-1个人报数"0"  
    PaceForward_L(L, cursor, m, cursor, e); //通过引用参数destination更新cursor  
    printf("%d", e);  
    cursor->flag = DELETED;  
    for(counter = 1; counter < n; counter++) //如果跳过的已删除结点的个数超过n（绕了整整一圈），说明所有结点已经删除，结束  
    {  
        if (counter % 10 == 0)  printf("\n");  
        PaceForward_L(L, cursor, m, cursor, e); //通过引用参数destination更新cursor  
        if (counter % 10 != 0)  printf(" ");  
        printf("%d", e);  
        cursor->flag = DELETED;  
    }  
    printf("\n");  
      
    system("pause");  
    return 0;  
}  

#include<stdio.h>  
#include<stdlib.h>  
  
typedef unsigned char Status;  
#define FOUND 1  
#define NOT_FOUND 0  
  
//平衡二叉树定义（带双亲域的二叉链表）  
typedef struct BiTNode {  
    char data;  
    int bf; //平衡因子 = 左子树深度 - 右子树深度  
    BiTNode *lchild, *rchild, *parent;  
}BiTNode, *BiTree;  
typedef enum { N, L, R } Direction;  
  
  
void PreOrderTraverse(BiTree &T) {  
    //先序（先左后右）遍历二叉树（递归算法），打印先序遍历序列  
    if (!T) return;  
    printf("%c", T->data);  
    PreOrderTraverse(T->lchild);  
    PreOrderTraverse(T->rchild);  
}  
  
void InOrderTraverse1(BiTree &T) {  
    //中序（先左后右）遍历二叉树（递归算法），打印中序遍历序列  
    if (!T) return;  
    InOrderTraverse1(T->lchild);  
    printf("%c", T->data);  
    InOrderTraverse1(T->rchild);  
}  
  
void PostOrderTraverse(BiTree &T) {  
    //后序（先左后右）遍历二叉树（递归算法），打印后序遍历序列  
    if (!T) return;  
    PostOrderTraverse(T->lchild);  
    PostOrderTraverse(T->rchild);  
    printf("%c", T->data);  
}  
  
void InOrderTraverse2(BiTree &T, int current_level) {  
    //中序（先右后左）遍历二叉树（递归算法），打印二叉树的凹入表示  
    //current_level用于记录当前子树的层数  
    if (!T) return;  
    InOrderTraverse2(T->rchild, current_level + 1);  
    for (int i = 0; i < 4 * (current_level - 1); ++i)    printf(" ");    //根据当前层数确定打印缩进量  
    printf("%c\n", T->data);  
    InOrderTraverse2(T->lchild, current_level + 1);  
}  
  
int BiTreeDepth(BiTree T) {  
    //求二叉树的深度（递归算法）  
    if (!T) return 0;  
    int ldepth = BiTreeDepth(T->lchild), rdepth = BiTreeDepth(T->rchild);  
    return ((ldepth >= rdepth) ? ldepth : rdepth) + 1;  
}  
  
Status SearchBST(BiTree BST, char key, BiTNode *&insert_aim_node, Direction &L_or_R) {  
    //查找二叉排序树BBST中是否存在关键字为key的记录，非递归算法  
    //若存在，返回FOUND；否则，返回NOT_FOUND，待插入位置存放于insert_aim_node和L_or_R  
    if (!BST)   return NOT_FOUND;  
    BiTNode *p = BST;  
    while (1)  
    {  
        if (key == p->data) { insert_aim_node = NULL; return FOUND; }  
        if (key < p->data)  
        {  
            if (!p->lchild) { insert_aim_node = p; L_or_R = L; return NOT_FOUND; }  
            else p = p->lchild;  
        }  
        if (key > p->data)  
        {  
            if (!p->rchild) { insert_aim_node = p; L_or_R = R; return NOT_FOUND; }  
            else p = p->rchild;  
        }  
    }  
}  
  
void L_Rotate(BiTree &p, BiTree &BST) {  
    //对二叉排序树BBST的p子树作左旋处理，并链接到原来的位置。处理结束后p指向该子树新的根  
    BiTNode *Parent = p->parent, *q = p; p = p->rchild;   //Parent总是指向该子树的双亲结点，q总是指向该子树原来的根，p总是指向该子树新的根  
    BiTree hang = p->lchild; //需要摘下来挂到对面的子树  
    //重新链接相关结点  
    p->lchild = q;   //颠倒p,q的亲子关系  
    q->rchild = hang;    //挂到对面  
    //更新相关结点的parent域  
    p->parent = Parent;  
    q->parent = p;  
    if(hang)    hang->parent = q;  
    //链接到原来的位置  
    if (Parent)  
    {  
        if (Parent->lchild == q) Parent->lchild = p;  //原来是左孩子，就链接到左孩子的位置  
        if (Parent->rchild == q) Parent->rchild = p;  //原来是右孩子，就链接到右孩子的位置  
    }  
    else BST = p;   //双亲不存在（处理的子树就是BBST本身），直接将p赋值给BBST  
    //更新平衡因子发生改变的结点的bf域  
    q->bf = 0; p->bf = 0; //p结点平衡因子由-2变为0，q结点平衡因子由-1变为0  
    while (Parent)  //所有祖先结点平衡因子都可能发生变化  
    {  
        Parent->bf = BiTreeDepth(Parent->lchild) - BiTreeDepth(Parent->rchild);    //重新计算平衡因子  
        Parent = Parent->parent;  
    }  
}  
  
void R_Rotate(BiTree &p, BiTree &BST) {  
    //右旋处理，与左旋类似  
    BiTNode *Parent = p->parent, *q = p; p = p->lchild;  
    BiTree hang = p->rchild;  
    p->rchild = q;  
    q->lchild = hang;  
    p->parent = Parent;  
    q->parent = p;  
    if (hang)   hang->parent = q;  
    if (Parent)  
    {  
        if (Parent->lchild == q) Parent->lchild = p;  
        if (Parent->rchild == q) Parent->rchild = p;  
    }  
    else BST = p;  
    q->bf = 0; p->bf = 0;  
    while (Parent)  
    {  
        Parent->bf = BiTreeDepth(Parent->lchild) - BiTreeDepth(Parent->rchild);  
        Parent = Parent->parent;  
    }  
}  
  
void InsertBBST(BiTree &BBST, char &insert_elem) {  
    //在平衡二叉树BBST中插入关键字为insert_elem的记录（查找不成功时）  
    if (!BBST)  //如果树为空，直接建立根结点  
    {  
        BBST = (BiTNode *)malloc(sizeof(BiTNode));  
        BBST->data = insert_elem; BBST->lchild = BBST->rchild = NULL; BBST->bf = 0; BBST->parent = NULL;  
        return;   
    }  
    BiTNode *insert_aim_node, *new_node; Direction L_or_R;  
    if (!SearchBST(BBST, insert_elem, insert_aim_node, L_or_R)) //查找不成功时  
    {  
        //在SearchBST找到的相应位置插入记录  
        new_node = (BiTNode *)malloc(sizeof(BiTNode));  
        new_node->data = insert_elem; new_node->lchild = new_node->rchild = NULL; new_node->parent = insert_aim_node; new_node->bf = 0;  
        if (L_or_R == L)    insert_aim_node->lchild = new_node;  
        else insert_aim_node->rchild = new_node;  
        //更新平衡因子发生改变的结点（所有祖先结点）的bf域，并找到因插入结点而失去平衡的最小子树根结点unblcd_tree  
        BiTNode *p = new_node; BiTree unblcd_tree = NULL; int find_unblcd = 0; Direction dp = N, dpp;  
        while (p->parent)  
        {  
            if (p->parent->lchild == p)  
            {  
                if (!find_unblcd) { dpp = dp; dp = L; } //不断记录先后两个拐弯方向，当找到第一个不平衡结点时停止记录  
                p = p->parent;  
                if ((p->bf = BiTreeDepth(p->lchild) - BiTreeDepth(p->rchild)) >= 2 && !find_unblcd)  
                {  
                    unblcd_tree = p;  
                    find_unblcd = 1;  
                }  
            }  
            else  
            {  
                if (!find_unblcd) { dpp = dp; dp = R; }  
                p = p->parent;  
                if ((p->bf = BiTreeDepth(p->lchild) - BiTreeDepth(p->rchild)) <= -2 && !find_unblcd)  
                {  
                    unblcd_tree = p;  
                    find_unblcd = 1;  
                }  
            }  
        }  
        //如果找到了不平衡的结点，则需旋转处理  
        if (find_unblcd)  
        {  
            if (dp == L && dpp == L)    //LL型不平衡：右旋  
                R_Rotate(unblcd_tree, BBST);  
            if (dp == R && dpp == R)    //RR型不平衡：左旋  
                L_Rotate(unblcd_tree, BBST);  
            if (dp == L && dpp == R)    //LR型不平衡：先左旋再右旋  
            {  
                L_Rotate(unblcd_tree->lchild, BBST);  
                R_Rotate(unblcd_tree, BBST);  
            }  
            if (dp == R && dpp == L)    //RL型不平衡：先右旋再左旋  
            {  
                R_Rotate(unblcd_tree->rchild, BBST);  
                L_Rotate(unblcd_tree, BBST);  
            }  
        }  
    }  
}  
  
int main() {  
    BiTree BBST = NULL; char insert_elem;  
    while (1)   //建立平衡二叉树BBST  
    {  
        insert_elem = getchar();  
        if (insert_elem == '\n')    break;  
        InsertBBST(BBST, insert_elem);  
    }  
    printf("Preorder: "); PreOrderTraverse(BBST); printf("\n");  
    printf("Inorder: "); InOrderTraverse1(BBST); printf("\n");  
    printf("Postorder: "); PostOrderTraverse(BBST); printf("\n");  
    printf("Tree:\n"); InOrderTraverse2(BBST, 1);   
  
    system("pause");  
    return 0;  
}  

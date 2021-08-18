#include<stdio.h>  
#include<stdlib.h>  
  
typedef unsigned char Status;  
#define FOUND 1  
#define NOT_FOUND 0  
  
typedef struct BiTNode {  
    int data;  
    BiTNode *lchild, *rchild;  
}BiTNode, *BiTree;  
#define L 1  
#define R 2  
  
int BST_node_num, K, *InOrderTraverseSequence;  //全局变量，用于存储中序遍历序列  
  
void InOrderTraverse(BiTree &T, int current_level) {  
    //中序遍历二叉树（递归算法），打印二叉树的凹入表示，并存储中序遍历序列  
    //current_level用于记录当前子树的层数  
    if (!T) return;  
    InOrderTraverse(T->lchild, current_level + 1);  
    for (int i = 0; i < 4 * (current_level - 1); ++i)    printf(" ");    //根据当前层数确定打印缩进量  
    printf("%d\n", T->data);  
    InOrderTraverseSequence[K++] = T->data;  //存储中序遍历序列  
    InOrderTraverse(T->rchild, current_level + 1);  
}  
  
Status SearchBST(BiTree BST, int key, BiTNode *&insert_aim_node, int &L_or_R) {  
    //查找二叉排序树BST中是否存在关键字为key记录，非递归算法  
    //若存在，返回FOUND；否则，返回NOT_FOUND，待插入位置存放于insert_aim_node和L_or_R  
    if (!BST)   return NOT_FOUND;  
    BiTNode *node_ptr = BST;  
    while (1)  
    {  
        if (key == node_ptr->data)  { insert_aim_node = NULL; return FOUND; }  
        if (key < node_ptr->data)   
        {   
            if (!node_ptr->lchild) { insert_aim_node = node_ptr; L_or_R = L; return NOT_FOUND; }  
            else node_ptr = node_ptr->lchild;  
        }  
        if (key > node_ptr->data)  
        {  
            if (!node_ptr->rchild) { insert_aim_node = node_ptr; L_or_R = R; return NOT_FOUND; }  
            else node_ptr = node_ptr->rchild;  
        }  
    }  
}  
  
void InsertBST(BiTree &BST, int &insert_elem) {  
    //在二叉排序树BST中插入关键字为insert_elem的记录（查找不成功时）  
    if (!BST) { BST = (BiTNode *)malloc(sizeof(BiTNode)); BST->data = insert_elem; BST->lchild = BST->rchild = NULL; return; }  
    BiTNode *insert_aim_node, *new_node; int L_or_R;  
    if (!SearchBST(BST, insert_elem, insert_aim_node, L_or_R))  
    {  
        new_node = (BiTNode *)malloc(sizeof(BiTNode));  
        new_node->data = insert_elem; new_node->lchild = new_node->rchild = NULL;  
        if (L_or_R == L)    insert_aim_node->lchild = new_node;  
        else insert_aim_node->rchild = new_node;  
    }  
}  
  
int main() {  
    BiTree BST = NULL; int insert_elem;  
  
    BST_node_num = 0;  
    while (1)  
    {  
        scanf("%d", &insert_elem);  
        if (!insert_elem)   break;  
        InsertBST(BST, insert_elem); BST_node_num++;  
    }  
    InOrderTraverseSequence = (int *)malloc(BST_node_num * sizeof(int)); K = 0;  
    InOrderTraverse(BST, 1);  
    printf("\n");  
    for (int i = 0; i < BST_node_num; ++i)  
        printf(" %d", InOrderTraverseSequence[i]);  
    printf("\n");  
  
    system("pause");  
    return 0;  
} 

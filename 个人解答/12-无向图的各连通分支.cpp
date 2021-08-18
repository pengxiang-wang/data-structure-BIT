#include<stdio.h>  
#include<stdlib.h>  
  
typedef unsigned char BOOL;  
#define TRUE 1  
#define FALSE 0  
  
typedef int VertexType;  
//无向图（UDG）的边集数组表示  
typedef struct {  
    int adjvex1, adjvex2;  
}Edge;  //边类型  
typedef struct {  
    VertexType *vexs;   //顶点向量  
    BOOL *visited;  //访问标记  
    Edge *edges;    //边集数组  
    int vexnum, edgenum;    //顶点数、弧数  
}MUDG;  //无向图类型  
  
/* 以下为循环队列定义及操作 */  
typedef int Status;  
#define OK 1  
#define ERROR 0  
#define OVERFLOW -2  
#define UNDERFLOW -3  
  
typedef int QElemType;  //队列中存储顶点的编号（int型）  
#define MAX_QUEUE_SIZE 100  
typedef struct {  
    QElemType *base;  
    int front, rear;  
}Queue;  
Queue Q;    //全局变量  
  
Status InitQueue(Queue &Q) {  
    //初始化队列  
    Q.base = (QElemType *)malloc(MAX_QUEUE_SIZE * sizeof(QElemType));  
    if (!Q.base)    exit(OVERFLOW);  
    Q.front = 0; Q.rear = 0;  
    return OK;  
}  
  
BOOL QueueEmpty(Queue Q) {  
    //队列判空  
    if (Q.rear == Q.front)  return TRUE;  
    else return FALSE;  
}  
  
Status EnQueue(Queue &Q, QElemType e) {  
    //入队  
    if ((Q.rear + 1) % MAX_QUEUE_SIZE == Q.front)   return OVERFLOW;  
    Q.base[Q.rear] = e;  
    Q.rear = (Q.rear + 1) % MAX_QUEUE_SIZE;  
    return OK;  
}  
  
Status DeQueue(Queue &Q, QElemType &e) {  
    //出队  
    if (Q.rear == Q.front)  return UNDERFLOW;  
    e = Q.base[Q.front];  
    Q.front = (Q.front + 1) % MAX_QUEUE_SIZE;  
    return OK;  
}  
/* 以上为循环队列定义及操作 */  
  
void CreateMUDG(MUDG &G) {  
    //建立无向图  
    /*输入格式： 
    顶点数 
    边1依附顶点 边1依附顶点 
    边2依附顶点 边2依附顶点 
    ... 
    -1 
    */  
    int i, v1, v2;  
    //输入顶点数  
    scanf("%d", &G.vexnum);  
    //初始化顶点向量、邻接矩阵  
    G.vexs = (VertexType *)malloc(G.vexnum * sizeof(VertexType));  
    for (i = 0; i < G.vexnum; i++)   G.vexs[i] = i;  //顶点数据即为顶点编号  
    //输入边  
    G.edgenum = 0;  
    scanf("%d", &v1);  
    if (v1 == -1)   return;  
    G.edges = (Edge *)malloc(sizeof(Edge));  
    do{  
        G.edges = (Edge *)realloc(G.edges, (G.edgenum + 1) * sizeof(Edge));  
        scanf("%d", &v2);  
        G.edges[G.edgenum].adjvex1 = v1; G.edges[G.edgenum].adjvex2 = v2; G.edgenum++;  
        scanf("%d", &v1);  
    } while (v1 != -1);  
}  
  
BOOL VexVisitedIf(MUDG G, int vex_index) {  
    //判断无向图G中的顶点是否被访问过  
    //顶点编号为vex_index，如果已被访问，返回TRUE，否则返回FALSE  
    if (G.visited[vex_index])   return TRUE;  
    else return FALSE;  
}  
  
BOOL AllVexVisitedIf(MUDG G, int &unvisited_vex_index) {  
    //判断无向图G中是否有未被访问的顶点  
    //如果有，将编号最小的未被访问顶点存放于unvisited_vex_index，返回FALSE；否则返回TRUE  
    for (int i = 0; i < G.vexnum; ++i)  
        if (!VexVisitedIf(G, i))  
        {  
            unvisited_vex_index = i;  
            return FALSE;  
        }  
    return TRUE;  
}  
  
int cmp(const void *a, const void *b) {  
    //qsort所需的比较函数  
    return *(int*)a - *(int*)b;  
}  
  
void GetAllAdjVex(MUDG G, int vex_index, int *adjvex_index, int &adjvex_num) {  
    //寻找无向图G中某顶点的所有邻接点  
    //顶点编号为vex_index，邻接点编号存放于数组adjvex_index，邻接点个数存放于adjvex_num  
    int i; adjvex_num = 0;  
    for (i = 0; i < G.edgenum; ++i)  
    {  
        if (G.edges[i].adjvex1 == vex_index)  
            adjvex_index[adjvex_num++] = G.edges[i].adjvex2;  
        if (G.edges[i].adjvex2 == vex_index)  
            adjvex_index[adjvex_num++] = G.edges[i].adjvex1;  
    }  
    qsort(adjvex_index, adjvex_num, sizeof(int), cmp);  //对邻接点编号从小到大排序（采用快速排序库函数qsort）  
}  
  
void PrintBFS(MUDG G) {  
    //格式化输出图G的广度优先遍历序列  
    int i, vex_index, unvisited_vex_index, *adjvex_index = (int *)malloc(G.vexnum * sizeof(int)), adjvex_num;  
    //初始化各顶点的访问标记  
    G.visited = (BOOL *)malloc(G.vexnum * sizeof(BOOL));  
    for (i = 0; i < G.vexnum; ++i)   G.visited[i] = FALSE;  
      
    InitQueue(Q);  
    while (!AllVexVisitedIf(G, unvisited_vex_index))    //终止条件：图中没有访问的顶点  
    {  
        printf("%d", G.vexs[unvisited_vex_index]); G.visited[unvisited_vex_index] = TRUE;   //访问该顶点并标记已访问  
        //广度优先遍历此连通分量中的其他顶点  
        vex_index = unvisited_vex_index;  
        while (1)  
        {  
            //访问vex_index（称为“开花点”）的所有未被访问的邻接点，并标记已访问（注：此时“开花点”已访问）  
            GetAllAdjVex(G, vex_index, adjvex_index, adjvex_num);  
            for (i = 0; i < adjvex_num; ++i)  
                if (!VexVisitedIf(G, adjvex_index[i]))  
                {  
                    printf("-%d", G.vexs[adjvex_index[i]]); G.visited[adjvex_index[i]] = TRUE;  
                    EnQueue(Q, adjvex_index[i]);  
                }  
            if (QueueEmpty(Q))  break;  //队空时结束  
            DeQueue(Q, vex_index);  //出队，作为下一次的开花点  
        }  
        printf("\n");  
    }  
}  
  
int main() {  
    MUDG G;  
    CreateMUDG(G);  
    PrintBFS(G);  
  
    system("pause");  
    return 0;  
}  

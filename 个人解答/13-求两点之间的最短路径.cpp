#include<stdio.h>  
#include<stdlib.h>  
  
typedef char * VertexType;  
#define INFINITY 100000000  
//带权有向图（DN）的邻接矩阵表示  
typedef struct {  
    VertexType *vexs;   //顶点向量  
    int **arcs; //邻接矩阵  
    int vexnum, arcnum; //顶点数、弧数  
}MDN;   //带权有向图类型  
  
//路径的单链表表示（不带头结点）  
typedef struct Node {  
    int vex_index;  //路径中顶点的编号  
    struct Node *next;  
}PathNode;  //链表结点类型  
typedef struct  
{  
    PathNode *head, *tail;  //链表的头指针、尾指针  
}PathLinkList;  //链表类型  
typedef struct {  
    int source, destination;    //路径起点顶点、终点顶点的编号  
    PathLinkList path;  //路径链表  
    int dist;   //路径长度  
}Path;  //路径类型  
  
void CreateMDN(MDN &G) {  
    //建立带权有向图  
    /*输入格式： 
    顶点数,边数 
    顶点1,顶点2,...,顶点n 
    <弧1起点,弧1终点,弧1权>,<弧2起点,弧2终点,弧2权>,...,<弧e起点,弧e终点,弧e权> 
    */  
    int i, j, v1, v2, weight;  
    //输入顶点数、弧数  
    scanf("%d,%d", &G.vexnum, &G.arcnum); getchar();  
    //初始化顶点向量、邻接矩阵  
    G.vexs = (VertexType *)malloc(G.vexnum * sizeof(VertexType));  
    G.arcs = (int **)malloc(G.vexnum * sizeof(int *));  
    for (i = 0; i < G.vexnum; ++i)  
    {  
        G.arcs[i] = (int *)malloc(G.vexnum * sizeof(int));  
        for (j = 0; j < G.vexnum; ++j)  
            G.arcs[i][j] = INFINITY;  
    }  
    //输入顶点  
    char vexs_str[1000]; int comma_pos = 0, head;  
    scanf("%s", vexs_str); getchar();   //存储顶点格式化输入的字符串  
    for (i = 0, head = 0; i < G.vexnum; ++i) //字符串处理  
    {  
        while (vexs_str[comma_pos] != ',' && vexs_str[comma_pos] != '\0')   comma_pos++;  
        G.vexs[i] = (char *)malloc((comma_pos - head) * sizeof(char));  
        for (j = head; j < comma_pos; ++j)  
            G.vexs[i][j - head] = vexs_str[j];  
        G.vexs[i][j - head] = '\0';  
        head = comma_pos + 1; comma_pos++;  
    }  
    //输入弧  
    for (i = 0; i < G.arcnum; ++i)  
    {  
        scanf("<%d,%d,%d>", &v1, &v2, &weight); getchar();  
        G.arcs[v1][v2] = weight;  
    }  
}  
  
void Copy_PathLinkList(PathLinkList L1, PathLinkList &L2) {  
    //复制路径单链表L1为L2  
    if (L1.head == NULL) { L2.head = NULL; return; }  
    PathNode *NodePtr_L1 = L1.head, *NodePtr_L2;  
    L2.head = (PathNode *)malloc(sizeof(PathNode)); NodePtr_L2 = L2.head;  
    while (1)  
    {  
        NodePtr_L2->vex_index = NodePtr_L1->vex_index;  
        if (!NodePtr_L1->next)   break;  
        NodePtr_L1 = NodePtr_L1->next;  
        NodePtr_L2 = (NodePtr_L2->next = (PathNode *)malloc(sizeof(PathNode)));  
    }  
    NodePtr_L2->next = NULL;  
    L2.tail = NodePtr_L2;   //复制尾指针  
}  
  
void InserttoTail_PathLinkList(PathLinkList &L, int e) {  
    //在路径单链表L的尾部插入元素e  
    if (L.head == NULL)  
    {  
        L.head = (PathNode *)malloc(sizeof(PathNode));  
        L.head->vex_index = e; L.head->next = NULL;  
        L.tail = L.head;  
    }  
    else  
    {  
        L.tail = (L.tail->next = (PathNode *)malloc(sizeof(PathNode)));  //后移尾指针，申请新结点  
        L.tail->vex_index = e; L.tail->next = NULL;  
    }  
}  
  
void ShortestPath_DIJKSTRA(MDN G, int source_vex_index, Path *&ShortestPath) {  
    //求带权有向图的单源最短路径，Dijkstra算法  
    //源点编号为source_vex_index，源点到编号i顶点的最短路径存放于ShortestPath[i]  
    int i, k, min_pos, min_dist; Path JumpBoard;  
    //初始化ShortestPath  
    ShortestPath = (Path *)malloc(G.vexnum * sizeof(Path));  
    for (k = 0; k < G.vexnum; ++k)  
    {  
        ShortestPath[k].source = source_vex_index; ShortestPath[k].destination = k;  
        ShortestPath[k].path.head = NULL; ShortestPath[k].path.tail = NULL;  
        ShortestPath[k].dist = (k == source_vex_index) ? 0 : INFINITY;  
    }  
    //初始化路径跳板JumpBoard  
    JumpBoard.path.head = (PathNode *)malloc(sizeof(PathNode));  
    JumpBoard.path.head->vex_index = source_vex_index; JumpBoard.path.head->next = NULL; JumpBoard.path.tail = JumpBoard.path.head;   //第一遍的跳板为v0本身  
    JumpBoard.source = source_vex_index; JumpBoard.destination = source_vex_index;  
    JumpBoard.dist = 0; //这里故意设置为0，为了第一遍能够顺利进行  
  
    int *Flag = (int *)malloc(G.vexnum * sizeof(int));  //Flag[i]标记ShortestPath[i]是否已确定  
    for (k = 0; k < G.vexnum; ++k)   Flag[k] = 0;    //初始化Flag  
    Flag[source_vex_index] = 1; //源点到源点的最短路径设为已确定  
    for (i = 0; i < G.vexnum - 1; ++i)   //共进行 n - 1 遍  
    {  
        //更新最短路径  
        min_dist = INFINITY;   
        min_pos = source_vex_index; //为防止后面用到没有初始化的min_pos  
        for (k = 0; k < G.vexnum; ++k)  
        {  
            if (Flag[k])    continue;   //跳过已确定最短路径的顶点  
            if (JumpBoard.dist + G.arcs[JumpBoard.destination][k] < ShortestPath[k].dist)    //若借助当前跳板跳到顶点k的路径长度更短，更新ShortestPath[k]为该路径  
            {  
                Copy_PathLinkList(JumpBoard.path, ShortestPath[k].path);  
                InserttoTail_PathLinkList(ShortestPath[k].path, k);  
                ShortestPath[k].dist = JumpBoard.dist + G.arcs[JumpBoard.destination][k];  
            }  
            if (ShortestPath[k].dist < min_dist) { min_pos = k; min_dist = ShortestPath[k].dist; }   //在更新过程中寻找适合做跳板的路径最短者  
        }  
        Flag[min_pos] = 1;  //该路径最短者已确定为源点到相应顶点的最短路径  
        //更新跳板为该路径最短者  
        Copy_PathLinkList(ShortestPath[min_pos].path, JumpBoard.path);  
        JumpBoard.dist = ShortestPath[min_pos].dist;  
        JumpBoard.destination = min_pos;  
    }  
}  
  
void PrintPath(MDN G, Path path) {  
    //格式化输出路径  
    PathNode *p = path.path.head;  
    if (!p) return;  
    while (p->next)  
    {  
        printf("%s-", G.vexs[p->vex_index]);  
        p = p->next;  
    }  
    printf("%s\n", G.vexs[p->vex_index]);  
}  
  
int main() {  
    MDN G; Path *ShortestPath = NULL;  
    CreateMDN(G);  
    ShortestPath_DIJKSTRA(G, 0, ShortestPath);  
    PrintPath(G, ShortestPath[G.vexnum - 1]);  
  
    system("pause");  
    return 0;  
}  

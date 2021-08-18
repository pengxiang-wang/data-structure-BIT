#include<stdio.h>  
#include<stdlib.h>  
  
typedef int Status;  
#define OK 1  
#define ERROR 0  
#define OVERFLOW -2  
#define UNDERFLOW -3  
  
typedef int BOOL;  
#define TRUE 1  
#define FALSE 0  
  
typedef int PanelElemType;  //游戏面板元素类型，一般用整型数据标记  
typedef struct {  
    PanelElemType **panel;  //存储游戏面板信息的二维数组  
    int rownum, colnum; //二维数组的行数、列数  
}Panel; //游戏面板类型  
typedef struct {  
    int row;    //行坐标  
    int col;    //列坐标  
}Position;  //位置（坐标）类型  
typedef enum { UP, DOWN, LEFT, RIGHT, UP_AND_DOWN, LEFT_AND_RIGHT }DIRECTION;   //方向类型  
  
Position aim = { -1, -1 };  //全局变量，用于存放可与第一个位置消去的位置  
  
/* 以下为循环队列定义及操作 */  
#define MAX_QUEUE_SIZE 100  
typedef struct {  
    Position pos;  
    DIRECTION limited_dir;  
}QElemType;  
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
  
PanelElemType GetPanelElement(Panel panel, Position pos) {  
    //根据坐标取面板元素  
    return panel.panel[pos.row][pos.col];  
}  
  
BOOL EliminateStraightIf(Panel panel, Position pos, DIRECTION dir, PanelElemType value, int &potential_path_num) {  
    //判断面板pos1位置在dir方向是否存在可直线消去pos1_value的元素  
    //若存在，返回TRUE；否则返回FALSE  
    //如果不存在，则将该方向中间的0的个数存放于potential_path_num  
    int i, j;  
    potential_path_num = 0; //计数器归零  
    switch (dir)  
    {  
    case UP:  
        for (j = pos.col, i = pos.row - 1; i >= 0; --i)  //向上扫描  
            if (!GetPanelElement(panel, { i, j }))  //如果没有遇到非0位置  
            {  
                EnQueue(Q, { { i, j }, LEFT_AND_RIGHT });   //将中间的0的位置（潜在路径的拐弯位置）入队，同时将限制的下一遍扫描方向也入队。限制的下一遍扫描方向与此方向垂直  
                potential_path_num++;  
            }  
            else //遇到非0位置  
            {  
                if (GetPanelElement(panel, { i, j }) == value)  
                {  
                    aim = { i, j }; //判断是否相等（可消去），如果相等，将消去位置存入aim并返回TRUE，整个程序结束（main函数里设置了goto语句）  
                    return TRUE;  
                }  
                      
                else return FALSE;  //否则返回FALSE  
            }  
        break;  
    case DOWN:  
        for (j = pos.col, i = pos.row + 1; i < panel.rownum; ++i)  
            if (!GetPanelElement(panel, { i, j })) { EnQueue(Q, { { i, j }, LEFT_AND_RIGHT }); potential_path_num++; }  
            else { if (GetPanelElement(panel, { i, j }) == value) { aim = { i, j }; return TRUE; } else return FALSE; }  
        break;  
    case LEFT:  
        for (i = pos.row, j = pos.col - 1; j >= 0; --j)  
            if (!GetPanelElement(panel, { i, j })) { EnQueue(Q, { { i, j }, UP_AND_DOWN }); potential_path_num++; }  
            else { if (GetPanelElement(panel, { i, j }) == value) { aim = { i, j }; return TRUE; } else return FALSE; }  
        break;  
    case RIGHT:  
        for (i = pos.row, j = pos.col + 1; j < panel.colnum; ++j)  
            if (!GetPanelElement(panel, { i, j })) { EnQueue(Q, { { i, j }, UP_AND_DOWN }); potential_path_num++; }  
            else { if (GetPanelElement(panel, { i, j }) == value) { aim = { i, j }; return TRUE; } else return FALSE; }  
        break;  
    default:  
        break;  
    }  
    return FALSE;   //没有遇到非0位置就扫描到边界，返回FALSE  
}  
  
int main() {  
    Panel panel; int i, j, k; Position pos1, pos2; BOOL pos1Eliminatepos2If;  
  
    //读取游戏面板信息  
    scanf("%d%d", &panel.rownum, &panel.colnum);  
    panel.panel = (PanelElemType **)malloc(panel.rownum * sizeof(PanelElemType *));  
    for (i = 0; i < panel.rownum; ++i)  
    {  
        panel.panel[i] = (PanelElemType *)malloc(panel.colnum * sizeof(PanelElemType));  
        for (j = 0; j < panel.colnum; ++j)  
            scanf("%d", &panel.panel[i][j]);  
    }  
    scanf("%d%d", &pos1.row, &pos1.col);  
    scanf("%d%d", &pos2.row, &pos2.col);  
  
    //程序主体  
    //预检查  
    PanelElemType pos1_value = GetPanelElement(panel, pos1), pos2_value = GetPanelElement(panel, pos2);  
    if (!pos1_value || !pos2_value || pos1_value != pos2_value) { pos1Eliminatepos2If = FALSE; goto end; }  
  
    Position pos_turn_first, pos_turn_second; QElemType qe; DIRECTION limited_dir;  
    int potential_UP_path_num, potential_DOWN_path_num, potential_LEFT_path_num, potential_RIGHT_path_num, potential_STRAIGHT_path_num, potential_TURN_ONCE_path_num, sum;  
    pos1Eliminatepos2If = FALSE;  
    InitQueue(Q);  
  
    /* 按拐弯次数广度优先遍历 */  
  
    //检查是否可以直线（拐弯0次）消去  
    if (EliminateStraightIf(panel, pos1, UP, pos1_value, potential_UP_path_num) || EliminateStraightIf(panel, pos1, DOWN, pos1_value, potential_DOWN_path_num) || EliminateStraightIf(panel, pos1, LEFT, pos1_value, potential_LEFT_path_num) || EliminateStraightIf(panel, pos1, RIGHT, pos1_value, potential_RIGHT_path_num)) //四个方向都扫描  
        if (pos2.row == aim.row && pos2.col == aim.col)  
        {  
            pos1Eliminatepos2If = TRUE;  
            goto end;   //跳出二重循环  
        }  
    potential_STRAIGHT_path_num = potential_UP_path_num + potential_DOWN_path_num + potential_LEFT_path_num + potential_RIGHT_path_num; //如果不可直线消去，统计潜在路径个数，为下一遍扫描做准备  
  
    //检查是否可以拐弯1次消去  
    potential_TURN_ONCE_path_num = 0;  
    for (k = 0; k < potential_STRAIGHT_path_num; ++k)    //遍历上一遍扫描得到的潜在路径，共potential_STRAIGHT_path_num个  
    {  
        DeQueue(Q, qe); pos_turn_first = qe.pos; limited_dir = qe.limited_dir;  //前potential_STRAIGHT_path_num个出队的元素为潜在路径的第一次拐弯位置（包含限制的扫描方向）  
        switch (limited_dir)  
        {  
        case UP_AND_DOWN:  
            if (EliminateStraightIf(panel, pos_turn_first, UP, pos1_value, potential_UP_path_num) || EliminateStraightIf(panel, pos_turn_first, DOWN, pos1_value, potential_DOWN_path_num)) //只上下扫描  
                if (pos2.row == aim.row && pos2.col == aim.col)  
                {  
                    pos1Eliminatepos2If = TRUE;  
                    goto end;  
                }  
            sum = potential_UP_path_num + potential_DOWN_path_num;  
            break;  
        case LEFT_AND_RIGHT:  
            if (EliminateStraightIf(panel, pos_turn_first, LEFT, pos1_value, potential_LEFT_path_num) || EliminateStraightIf(panel, pos_turn_first, RIGHT, pos1_value, potential_RIGHT_path_num))   //只左右扫描  
                if (pos2.row == aim.row && pos2.col == aim.col)  
                {  
                    pos1Eliminatepos2If = TRUE;  
                    goto end;  
                }  
            sum = potential_LEFT_path_num + potential_RIGHT_path_num;  
            break;  
        default:  
            break;  
        }  
        potential_TURN_ONCE_path_num += sum;    //如果不可拐弯1次消去，统计潜在路径个数，为下一遍扫描做准备  
    }  
    //检查是否可以拐弯2次消去  
    for (k = 0; k < potential_TURN_ONCE_path_num; ++k)   //遍历上一遍扫描得到的潜在路径，共potential_TURN_ONCE_path_num个  
    {  
        DeQueue(Q, qe); pos_turn_second = qe.pos; limited_dir = qe.limited_dir; //前potential_STRAIGHT_path_num个出队的元素为潜在路径的第二次拐弯位置（包含限制的扫描方向）  
        switch (limited_dir)  
        {  
        case UP_AND_DOWN:  
            if (EliminateStraightIf(panel, pos_turn_second, UP, pos1_value, potential_UP_path_num) || EliminateStraightIf(panel, pos_turn_second, DOWN, pos1_value, potential_DOWN_path_num))  
                if (pos2.row == aim.row && pos2.col == aim.col)  
                {  
                    pos1Eliminatepos2If = TRUE;  
                    goto end;  
                }  
            break;  
        case LEFT_AND_RIGHT:  
            if (EliminateStraightIf(panel, pos_turn_second, LEFT, pos1_value, potential_LEFT_path_num) || EliminateStraightIf(panel, pos_turn_second, RIGHT, pos1_value, potential_RIGHT_path_num))  
                if (pos2.row == aim.row && pos2.col == aim.col)  
                {  
                    pos1Eliminatepos2If = TRUE;  
                    goto end;  
                }  
            break;  
        default:  
            break;  
        }  
        //注：最后一遍扫描无需统计潜在路径个数  
    }  
    end:;  
  
    //输出结果  
    if (pos1Eliminatepos2If)    printf("TRUE\n");  
    else printf("FALSE\n");  
  
    system("pause");  
    return 0;  
}  

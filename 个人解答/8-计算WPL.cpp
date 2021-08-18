#include<stdio.h>  
#include<stdlib.h>  
  
typedef int Status;  
#define OK 1  
#define LESS_THAN_TWO_NUMBER 0  
  
  
Status find_two_min_pos(int * num, int * flag, int n, int &pos1, int &pos2) {  
    //在数组num的前n个flag不为1的数中找到最小的两个数，其位置分别存放于pos1和pos2  
    int i, start;  
    for (start = 0; start < n && flag[start]; ++start);  //找到第一个flag不为1的位置，作为遍历的起点start  
    if (start >= n - 1)  return LESS_THAN_TWO_NUMBER;    //start == n：没有flag不为1的数；start == n - 1：只有一个flag不为1的数  
      
    for (i = start + 1, pos1 = start; i < n; ++i)    //第一遍遍历，找到最小的数，其位置存放于pos1  
    {  
        if (flag[i])    continue;   //跳过flag为1的数  
        if (num[i] < num[pos1])  pos1 = i;  
    }  
    flag[pos1] = 1; //目的：第二遍遍历时跳过此位置，下一次调用函数find_two_min_pos时跳过此位置  
  
    if (pos1 == start)  //如果原来的start正好是最小的数，还需继续向前找新的遍历的起点start  
        for (start = pos1; start < n && flag[start]; ++start);  
    if (start == n) return LESS_THAN_TWO_NUMBER;    //start == n：只有一个flag不为1的数（即pos1）  
    for (i = start + 1, pos2 = start; i < n; ++i)    //第二遍遍历，找到第二小的数，其位置存放于pos2  
    {  
        if (flag[i])    continue;  
        if (num[i] < num[pos2])  pos2 = i;  
    }  
    flag[pos2] = 1; //目的：下一次调用函数find_two_min_pos时跳过此位置  
  
    return OK;  
}  
  
int main() {  
    int n, freq[2][100] = { 0 }, i, pos1, pos2, wpl = 0;  
    //数组freq第一行存放这n个数（结点），第二行作为flag（用于确定函数find_two_min_pos的查找范围。flag为1表示跳过，0表示不跳过）  
    scanf("%d", &n);  
    for (i = 0; i < n; ++i)  
        scanf("%d", &freq[0][i]);  
      
    for (; find_two_min_pos(freq[0], freq[1], n, pos1, pos2); ++n)  //结束条件：find_two_min_pos返回LESS_THAN_TWO_NUMBER  
        freq[0][n] = freq[0][pos1] + freq[0][pos2]; //新结点放在最后面，值最小的两个的和。通过n++扩大freq的范围把新结点包含到下一次find_two_min_pos的范围内  
  
    //计算wpl：把所有结点（除去根结点）的freq值求和  
    for (i = 0; i < n - 1; ++i)  
        wpl += freq[0][i];  
    printf("WPL=%d\n", wpl);  
  
    system("pause");  
    return 0;  
}  

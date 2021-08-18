#include<stdio.h>  
#include<stdlib.h>  
  
#define TIMES 2   
  
void HeapAdjust(int *record, int start, int end) {  
    //堆排序的筛选过程（大根堆）。筛选对象为record中下标为start的元素，end记录序列末尾的位置  
    int max_child_index;  
    record[0] = record[start];  //暂存待筛选元素  
    while (start <= end / 2) //一直调整到叶子结点  
    {  
        //找到左右孩子中较大者的下标   
        max_child_index = 2 * start;  
        if (2 * start + 1 <= end)   
            if (record[2 * start + 1] > record[2 * start])  
                max_child_index++;  
        //若左右孩子有比待筛选元素大的，与较大者交换，否则停止调整   
        if (record[0] < record[max_child_index])  
        {  
            record[start] = record[max_child_index];    //假交换   
            start = max_child_index;  
        }  
        else break;  
    }  
    record[start] = record[0];  //待筛选元素记录到位   
}  
  
void PrintRecord(int *record, int start, int end) {  
    //格式化输出记录，数组record下标从start到end的元素   
    for (int i = start; i <= end; ++i)  
        printf("%d ", record[i]);  
    printf("\n");  
}  
  
int main() {  
    int *record, N, i, k;  
    scanf("%d", &N);    //待排序记录数   
    record = (int *)malloc((N + 1) * sizeof(int));  //存储待排序记录   
    for (i = 1; i <= N; ++i) //为了方便，从下标1开始存储待排序记录。下标为0的位置当做临时变量用  
        scanf("%d", record + i);  
    //初始建堆  
    for (int start = N / 2; start >= 1; --start) //从第一个非叶子结点开始，从后向前筛选   
        HeapAdjust(record, start, N);  
    PrintRecord(record, 1, N);  
      
    //TIMES遍筛选  
    int end = N;  
    for (k = 0; k < TIMES; ++k)  
    {  
        record[1] = record[end--];  //堆顶元素用最后一个元素替代   
        HeapAdjust(record, 1, end);  
        PrintRecord(record, 1, end);  
    }  
      
    system("pause");  
    return 0;  
}  

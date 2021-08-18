#include<stdio.h>  
#include<stdlib.h>  
  
#define MAX_NUM 1000  
#define CUTOFF 5  
  
void StraightInsertSort(int *L, int low, int high) {  
    //直接插入排序  
    //待排序序列数组 L 下标从 low 到 high 的位置（包括 L[low] 和 L[high] ）  
    int start = low, i, inserted_elem;  
    for (low++; low <= high; ++low)  
    {  
        inserted_elem = L[low];  
        for (i = low - 1; L[i] > inserted_elem && i >= start; --i)  
            L[i + 1] = L[i];  
        L[i + 1] = inserted_elem;  
    }  
}  
  
int Partition(int *L, int low, int high) {  
    //分割序列：调整数组使序列被枢轴分成两部分，枢轴左边都比枢轴小，右边都比枢轴大  
    //待分割序列为数组 L 下标从 low 到 high 的位置（包括 L[low] 和 L[high] ）  
    //返回枢轴的下标  
    int pivot_value = L[low], temp; //枢轴可以是任意的，这里统一设为第一个元素  
    while (low < high)   //结束条件：low与high相遇  
    {  
        //从左向右和从右向左交替搜索，不断与枢轴互换位置  
        while (low < high && L[high] >= pivot_value)  high--; //high向左搜索，直到找到比枢轴小的元素或与low相遇  
        L[low] = L[high];   //假交换技巧：在每次交换时不将枢轴赋值到相应位置，结束后才将枢轴记录到位  
        while (low < high && L[low] <= pivot_value)   low++;  //low向右搜索，直到找到比枢轴大的元素或与high相遇  
        L[high] = L[low];  
    }  
    L[low] = pivot_value;   //最后将枢轴赋值到相应的位置  
    return low;  
}  
  
void QuickSorting(int *L, int low, int high) {  
    //快速排序，递归算法  
    //待排序序列为数组 L 下标从 low 到 high 的位置（包括 L[low] 和 L[high] ）  
    if (low >= high) return; //递归终止条件，待排序序列长度小于或等于1  
    int pivot_index = Partition(L, low, high);  //用枢轴分割序列  
    QuickSorting(L, low, pivot_index - 1);  //递归地排序枢轴左边的序列  
    QuickSorting(L, pivot_index + 1, high); //递归地排序枢轴右边的序列  
}  
  
void QuickSort(int *L, int N, int *median3) {  
    //启动快速排序  
    //待排序序列为数组 L 的前 N 个元素  
  
    //预处理：从头尾和中间取3个元素，将头部元素和3个元素中大小的中间值交换，以避免选出最大元素或者最小元素的情况出现，以提高快排的效率。并将换序后的三个数依次存入数组 median3 中，以便打印  
    median3[0] = L[0], median3[1] = L[N / 2 - 1], median3[2] = L[N - 1];  
    int min = 0, max = 0, mid, temp;  
    for (int i = 1; i < 3; ++i)  //找到最大、最小元素  
    {  
        if (median3[i] > median3[max])   max = i;  
        if (median3[i] < median3[min])   min = i;  
    }  
    mid = 0 + 1 + 2 - max - min;    //推算出中间值元素  
    if (mid != 0) { temp = median3[0]; median3[0] = median3[mid]; median3[mid] = temp; }    //中间值元素不是头部元素，则与头部元素交换  
    L[0] = median3[0]; L[N / 2 - 1] = median3[1]; L[N - 1] = median3[2];    //更新待排序序列的相应元素  
  
    QuickSorting(L, 0, N - 1);  //开始快速排序  
}  
  
void PrintArray(int *L, int low, int high) {  
    //格式化输出序列  
    //待打印序列为数组 L 下标从 low 到 high 的位置（包括 L[low] 和 L[high] ）  
    for (int i = low; i <= high; ++i)  
        printf("%d ", L[i]);  
}  
  
int main() {  
    int L[MAX_NUM], N = 0, median3[3];  
    while (scanf("%d", L + N))  N++;  
    if (N <= CUTOFF) StraightInsertSort(L, 0, N - 1);    //元素个数不足CUTOFF使用插入排序  
    else QuickSort(L, N, median3);  
  
    printf("After Sorting:\n");  
    PrintArray(L, 0, N - 1);  
    printf("\nMedian3 Value:\n");  
    if (N <= CUTOFF) printf("none\n");  
    else printf("%d %d %d \n", median3[0], median3[1], median3[2]);  
  
    system("pause");  
    return 0;  
}  

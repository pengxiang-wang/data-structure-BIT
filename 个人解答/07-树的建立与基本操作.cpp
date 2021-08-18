#include<stdio.h>  
#include<stdlib.h>  
  
typedef int Status;  
#define OK 1  
#define ERROR 0  
#define OVERFLOW -2  
  
#define MAX_NUMBER_OF_NODE 50  
#define STACK_SIZE 50  
  
//（不可扩充）栈定义  
typedef struct {  
    int *base, *top;  
}Stack;  
  
  
Status InitStack(Stack &S) {  
    //初始化栈  
    if(!(S.base = (int *)malloc(STACK_SIZE * sizeof(int)))) exit(OVERFLOW);  
    S.top = S.base;  
    return OK;  
}  
  
Status Push(Stack &S, int e) {  
    //进栈  
    if (S.top - S.base == STACK_SIZE)   return ERROR;   //上溢  
    *S.top++ = e;  
    return OK;  
}  
  
Status Pop(Stack &S) {  
    //出栈  
    if (S.top == S.base)    return ERROR;   //下溢  
    S.top--;  
    return OK;  
}  
  
Status GetTop(Stack S, int &e) {  
    //取栈顶元素  
    if (S.top == S.base)    return ERROR;   //空栈  
    e = *(S.top - 1);  
    return OK;  
}  
  
void putchar_with_indent(char c, int space_num) {  
    //带缩进输出字符，c为输出的字符，space_num为字符前面的空格数  
    int i;  
    for (i = 0; i < space_num; ++i)  
        putchar(' ');  
    printf("%c\n", c);  
}  
  
int max(int a[], int n) {  
    //求整型数组a前n个的最大者  
    int i, max = a[0];  
    for (i = 1; i < n; ++i)  
        if (max < a[i])  
            max = a[i];  
    return max;  
}  
  
int main() {  
    char GListStr[100], c; int Degree[MAX_NUMBER_OF_NODE] = { 0 }, num_of_letter = 0; Stack S; int i, top, degree_sta[20] = { 0 }, degree, degree_of_tree;  
    //GListStr为树的广义表表示的字符串，degree[i]表示第i个出现的字母对应结点的度数，num_of_letter表示字母（结点）的个数  
    //对第 i 个出现的字母，称 i - 1 为字母的编号。用字母编号而不是字母本身，可以解决字母重复出现的问题  
    //S为用于解决括号层数的栈，本应存放括号，但由于左括号与字母一一对应，因此存放左括号右边的第一个字母的编号即可  
    scanf("%s", GListStr);  
    InitStack(S);  
  
    for (i = 0; (c = GListStr[i]) != '\0'; ++i)  
    {  
        if (c >= 'a' && c <= 'z') //左括号与字母一一对应，无视左括号，只看左括号右边的第一个字母  
        {  
            putchar_with_indent(c, 4 * (S.top - S.base));  
            Push(S, num_of_letter); //左括号（右边的第一个字母的编号）进栈  
            num_of_letter++;  
        }  
        if (c == ',')   //统计当前括号层逗号的个数（等于当前结点的度数）  
        {  
            GetTop(S, top); //当前结点  
            Degree[top]++;  
        }  
        if (c == ')')   //遇到右括号，销括号  
            Pop(S);  
    }  
  
    //输出结果  
    degree_of_tree = max(Degree, num_of_letter);  
    printf("Degree of tree: %d\n", degree_of_tree);  
    //degree_sta用于最后统计每个度数的结点个数。这样做的好处是只需扫描数组Degree一遍  
    for (i = 0; i < num_of_letter; ++i)  
        degree_sta[Degree[i]]++;  
    for (degree = 0; degree <= degree_of_tree; ++degree)  
        printf("Number of nodes of degree %d: %d\n", degree, degree_sta[degree]);  
  
    system("pause");  
    return 0;  
}  

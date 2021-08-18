#include<stdio.h>  
#include<string.h>  
#include<stdlib.h>  
  
typedef int Status;  
#define OK 1  
#define ERROR 0  
#define OVERFLOW -1  
#define UNDERFLOW -2  
  
#define STACK_INIT_SIZE 100  
#define STACKINCREMENT 10  
  
typedef struct {  
    //用于存放括号的栈，只存储'(', ')', '[', ']'  
    char * base;  
    char * top;  
    int stacksize;  
}SqStack;  
  
Status InitStack(SqStack &S) {  
    //初始化操作：建立空栈  
    S.base = (char *)malloc(STACK_INIT_SIZE * sizeof(char));  
    if (S.base == NULL) return OVERFLOW;    //malloc失败  
    S.top = S.base;  
    S.stacksize = STACK_INIT_SIZE;  
    return OK;  
}  
  
Status GetTop(SqStack S, char &e) {  
    //取栈顶元素，结果保存到e中  
    if (S.top == S.base) return ERROR;  //空栈  
    e = *(S.top - 1);  
    return OK;  
}  
  
Status Push(SqStack &S, char e) {  
    //入栈操作，入栈元素为e  
    if (S.top - S.base == S.stacksize)  //栈满，增加空间  
    {  
        S.base = (char *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(char));  
        if (S.base == NULL) exit(OVERFLOW); //realloc失败  
        S.stacksize += STACKINCREMENT;  
    }  
    * S.top = e;  
    S.top++;  
    return OK;  
}  
  
Status Pop(SqStack &S, char &e) {  
    //出栈操作，出栈元素存放在e中  
    if (S.top == S.base)    return UNDERFLOW;   //空栈，会发生下溢  
    S.top--;  
    e = * S.top;  
    return OK;  
}  
  
int main() {  
    char expression[100]; SqStack S; int i; char c, top, temp;  
    scanf_s("%s", expression, 100);  
    InitStack(S);  
    for (i = 0; i < strlen(expression); ++i)  
    {  
        c = expression[i]; top = 0; //top归零，防止上一次循环的值被利用  
        switch (c)  
        {  
        case '(':Push(S, c); break; //左括号都入栈  
        case '[':Push(S, c); break;  
        case ')':  
            GetTop(S, top);  
            if (top == '(') //右括号，如果和栈顶配对，就让配对的左括号出栈  
                Pop(S, temp);  
            else //否则匹配失败  
            {  
                printf("Match false!\n");  
                exit(0);  
            }  
            break;  
        case ']':  
            GetTop(S, top);  
            if (top == '[')  
                Pop(S, temp);  
            else  
            {  
                printf("Match false!\n");  
                exit(0);  
            }  
            break;  
        default:  
            break;  //如果不是这四种括号，啥也不干，直接跳过  
        }  
    }  
    if (S.top == S.base)    //如果存括号的栈为空，匹配成功  
        printf("Match succeed!\n");  
    else  
        printf("Match false!\n");  
  
    system("pause");  
    return 0;  
}  

#include<stdio.h>  
#include<stdlib.h>  
  
/* 
注意：本程序简单处理了很多报错机制，遇到某些错误时会直接exit(0)退出程序。 
如果闪退，请检查以下错误是否可能出现： 
初始化栈malloc失败 
进栈上溢 
出栈下溢 
取空栈栈顶元素 
查找给定操作符集合 {'#','+','-','*','/','^','(',')'} 以外的操作符的栈内优先级或栈外优先级 
*/  
  
  
#define STACK_SIZE 20  
  
//不可扩充栈  
typedef struct {  
    char * base;  
    char * top;  
}Stack;  
  
void InitStack(Stack &S) {  
    //构建一个空栈  
    S.base = (char *)malloc(STACK_SIZE * sizeof(char));  
    if (!S.base) exit(0);  
    S.top = S.base;  
}  
  
void Push(Stack &S, char e) {  
    //进栈  
    if (S.top == S.base + STACK_SIZE) exit(0);  
    *S.top++ = e;  
}  
  
char Pop(Stack &S) {  
    //出栈  
    if (S.top == S.base) exit(0);  
    char e = *(--S.top);  
    return e;  
}  
  
char GetTop(Stack S) {  
    //取栈顶元素  
    if (S.base == S.top)    exit(0);  
    return *(S.top - 1);  
}  
  
int isp(char c) {  
    //栈内优先级（in stack priority）  
    switch (c)  
    {  
    case '#':return 0; break;  
    case '(':return 1; break;  
    case '^':return 6; break;  
    case '*':return 5; break;  
    case '/':return 5; break;  
    case '+':return 3; break;  
    case '-':return 3; break;  
    case ')':return 8; break;  
    default:exit(0); break;  
    }  
}  
  
int icp(char c) {  
    //栈外优先级（in coming priority）  
    switch (c)  
    {  
    case '#':return 0; break;  
    case '(':return 8; break;  
    case '^':return 7; break;  
    case '*':return 4; break;  
    case '/':return 4; break;  
    case '+':return 2; break;  
    case '-':return 2; break;  
    case ')':return 1; break;  
    default:exit(0); break;  
    }  
}  
  
int IsOperator(char c) {  
    //检查是否是操作符，如果是返回1，不是返回0  
    if (c == '#' || c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')')  
        return 1;  
    else return 0;  
}  
  
int main()  
{  
    Stack OPTR; int N, i, j; char expressions[10][100], ch, op;  
    scanf("%d", &N);  
    for (i = 0; i < N; ++i)  
        scanf("%s", &expressions[i]);  
  
    for (i = 0; i < N; ++i)  
    {  
        InitStack(OPTR);    //每次处理新的表达式时初始化栈（不释放原来栈的空间，反复利用）  
        Push(OPTR, '#');    //拿栈内优先级最低的操作符'#'垫底  
        j = 0;  
        while ((ch = expressions[i][j]) != '\0')  
        {  
            if (!IsOperator(ch))    //读取到操作数，直接输出  
            {  
                putchar(ch);  
                j++;    //读取下一个字符  
            }  
            else //读取到操作符  
            {  
                op = GetTop(OPTR);  //栈顶操作符  
  
                if (isp(op) < icp(ch))   //栈顶操作符优先级小于当前操作符  
                {  
                    Push(OPTR, ch); //当前操作符进栈  
                    j++;    //读取下一个字符  
                }  
                else if (isp(op) > icp(ch))  //栈顶操作符优先级大于当前操作符  
                    putchar(Pop(OPTR)); //栈顶操作符退栈并输出，不读取下一个字符  
                else //栈顶操作符优先级与当前操作符相等（有两种情况：两个'#'配对、括号配对）  
                {  
                    Pop(OPTR);  //栈顶操作符(左'#'或左括号）退栈，不输出，读取下一个字符  
                    j++;    //读取下一个字符  
                }  
                  
            }  
        }  
        printf("\n");  
    }  
  
    system("pause");  
    return 0;  
}  

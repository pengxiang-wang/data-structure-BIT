#include <stdio.h>      
#include <stdlib.h>      
  
typedef struct node  
{  
    int    coef, exp;  
    struct node  *next;  
} NODE;  
  
NODE *add(NODE *, NODE *);  
void freepolynomial(NODE * &);  
NODE *mo_poly_multiplication(int, int, NODE *);  
void multiplication(NODE *, NODE *, NODE * &);  
void input(NODE *);  
void output(NODE *);  
  
void input(NODE * head)  
{  
    int flag, sign, sum, x;  
    char c;  
  
    NODE * p = head;  
  
    while ((c = getchar()) != '\n')  
    {  
        if (c == '<')  
        {  
            sum = 0;  
            sign = 1;  
            flag = 1;  
        }  
        else if (c == '-')  
            sign = -1;  
        else if (c >= '0'&& c <= '9')  
        {  
            sum = sum * 10 + c - '0';  
        }  
        else if (c == ',')  
        {  
            if (flag == 1)  
            {  
                x = sign * sum;  
                sum = 0;  
                flag = 2;  
                sign = 1;  
            }  
        }  
        else if (c == '>')  
        {  
            p->next = (NODE *)malloc(sizeof(NODE));  
            p->next->coef = x;  
            p->next->exp = sign * sum;  
            p = p->next;  
            p->next = NULL;  
            flag = 0;  
        }  
    }  
}  
  
void output(NODE * head)  
{  
    //打印多项式    
    if (head->next == NULL)  printf("<0,0>,"); //零多项式的输出格式    
    while (head->next != NULL)   //说明是带头结点的单链表    
    {  
        head = head->next;  
        printf("<%d,%d>,", head->coef, head->exp);  
    }  
    printf("\n");  
}  
  
NODE *add(NODE * h1, NODE * h2) {  
    //将多项式h1和h2相加，返回和多项式的头指针  
    NODE * h3;  //和多项式的头指针  
    h3 = (NODE *)malloc(sizeof(NODE));  
    NODE * cursor_h1 = h1->next, *cursor_h2 = h2->next, *cursor_h3 = h3;  
    while (cursor_h1 != NULL && cursor_h2 != NULL)  
    {  
        if (cursor_h1->exp == cursor_h2->exp && cursor_h1->coef + cursor_h2->coef == 0) //某一项相加抵消的情况，直接跳过  
        {  
            cursor_h1 = cursor_h1->next; cursor_h2 = cursor_h2->next;  
            continue;  
        }  
  
        cursor_h3->next = (NODE *)malloc(sizeof(NODE));  
        cursor_h3 = cursor_h3->next;  
        if (cursor_h1->exp < cursor_h2->exp)  
        {  
            cursor_h3->exp = cursor_h1->exp;  
            cursor_h3->coef = cursor_h1->coef;  
            cursor_h1 = cursor_h1->next;  
        }  
        else if (cursor_h2->exp < cursor_h1->exp)  
        {  
            cursor_h3->exp = cursor_h2->exp;  
            cursor_h3->coef = cursor_h2->coef;  
            cursor_h2 = cursor_h2->next;  
        }  
        else  
        {  
            cursor_h3->exp = cursor_h1->exp;  
            cursor_h3->coef = cursor_h1->coef + cursor_h2->coef;  
            cursor_h1 = cursor_h1->next; cursor_h2 = cursor_h2->next;  
        }  
    }  
    if (!(cursor_h1 == NULL && cursor_h2 == NULL))  //把剩下的那一部分原版复制  
    {  
        while (cursor_h1 != NULL)  
        {  
            cursor_h3->next = (NODE *)malloc(sizeof(NODE));  
            cursor_h3 = cursor_h3->next;  
            cursor_h3->coef = cursor_h1->coef;  
            cursor_h3->exp = cursor_h1->exp;  
  
            cursor_h1 = cursor_h1->next;  
        }  
        while (cursor_h2 != NULL)  
        {  
            cursor_h3->next = (NODE *)malloc(sizeof(NODE));  
            cursor_h3 = cursor_h3->next;  
            cursor_h3->coef = cursor_h2->coef;  
            cursor_h3->exp = cursor_h2->exp;  
  
            cursor_h2 = cursor_h2->next;  
        }  
    }  
  
    cursor_h3->next = NULL;  
    return h3;  
}  
  
void freepolynomial(NODE * &h) {  
    //释放多项式的内存空间  
    NODE * forward = h, *cleaner = h;  
    while (forward != NULL)  
    {  
        forward = forward->next;  
        free(cleaner);  
        cleaner = forward;  
    }  
    //free(h);  //不知道为什么，加上这句代码就会报错...所以就干脆不要了  
    h = NULL;  
}  
  
NODE *mo_poly_multiplication(int coef_mo, int exp_mo, NODE * poly) {  
    //计算单项式与多项式的乘积，coef_mo为单项式的系数，exp_mo为单项式的指数，poly为多项式（头指针），返回乘积多项式的头指针  
    NODE * result;  //乘积多项式的头指针  
    result = (NODE *)malloc(sizeof(NODE));  
    NODE * cursor_result = result;  
    if (coef_mo != 0)   //非零单项式    
    {  
        NODE *cursor_poly = poly->next;  //cursor_poly用于遍历多项式的每一项    
        while (cursor_poly != NULL) //复制多项式，改变每一项的系数和指数    
        {  
            if (cursor_poly->coef == 0)  //系数为0的项，不用管  
            {  
                cursor_poly = cursor_poly->next;  
                continue;  
            }  
            cursor_result->next = (NODE *)malloc(sizeof(NODE));  
            cursor_result = cursor_result->next;  
            cursor_result->coef = cursor_poly->coef * coef_mo;  
            cursor_result->exp = cursor_poly->exp + exp_mo;  
  
            cursor_poly = cursor_poly->next;  
        }  
    }  
    cursor_result->next = NULL;  
    return result;  
}  
  
void multiplication(NODE * h1, NODE * h2, NODE * &h3) {  
    //将多项式h1和h2相乘，乘积多项式的头指针存放于h3中    
    //方法：乘法分配律，将h2拆成一个个单项式，分别计算与这些h1与这些单项式的乘积，最后加起来    
    NODE * cursor_h2 = h2->next, * added, * recycled, * temp;  //cursor_h2用于遍历h2（遍历组成h2的每个单项式），没有用的可以释放的多项式都放在recycled  
    while (cursor_h2 != NULL)  
    {  
        added = mo_poly_multiplication(cursor_h2->coef, cursor_h2->exp, h1);  
        temp = add(h3, added); recycled = h3; h3 = temp;  
        freepolynomial(recycled);   //用后即焚  
        recycled = added; freepolynomial(recycled); //用后即焚  
  
        cursor_h2 = cursor_h2->next;  
    }  
}  
  
int main()  
{  
    NODE * head1, * head2, * head3;  
  
    head1 = (NODE *)malloc(sizeof(NODE));  
    input(head1);  
  
    head2 = (NODE *)malloc(sizeof(NODE));  
    input(head2);  
  
    head3 = (NODE *)malloc(sizeof(NODE));  
    head3->next = NULL;  
    multiplication(head1, head2, head3);  
  
    output(head3);  
  
    system("pause");  
    return 0;  
}  


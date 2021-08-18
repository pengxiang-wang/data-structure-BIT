#include<stdio.h>  
#include<stdlib.h>  
  
  
void moving_to_left(int panel[16]) {  
    //向左移动操作  
    int pos, i, j, k, counter, step, NonZero[4];  
      
    for (pos = 0; pos <= 12; pos = pos + 4)  //按行执行向左移动操作，panel[0]、panel[4]、panel[8]、panel[12]为面板各行第一个元素  
    {  
        for (i = 0; i < 4; ++i)  //NonZero数组归零  
            NonZero[i] = 0;  
        counter = 0; j = 0; k = 0;  
        for (i = 0; i < 4; ++i)  
        {  
            if (panel[pos + k] == 0) { k++; continue; }  
            NonZero[j++] = panel[pos + k];  //NonZero为该行非零元素（有顺序）  
            counter++;  //counter计数非零元素个数  
            k++;  
        }  
  
        step = 0; k = 0;  
        for (i = 0; i < 4; ++i)  
        {  
            if (i >= counter)    //避免两个0比较大小  
                continue;  
            if (i != 3 && NonZero[i] == NonZero[i + 1]) //相邻元素相等的，合并  
            {  
                panel[pos + k++] = 2 * NonZero[i];  
                i++;    //前进两个  
                step++;  
            }  
            else if (i == 3 && NonZero[i - 1] == NonZero[i] && NonZero[i - 2] != NonZero[i - 1]);  
            else  
            {  
                panel[pos + k++] = NonZero[i];  
                step++;  
            }  
        }  
        for (i = 0; i < 4 - step; ++i)   panel[pos + k++] = 0;   //补上后面的0，一共补(4 - step)个0  
    }  
}  
  
void change_direction_to_left(int panel[16], char orig_direction) {  
    //改变游戏面板的方向：面板的orig_direction这个边变换为左边（保持面板在屏幕上做平面刚体运动）  
    //为了方便，将游戏面板方向统一调成向左的方向，在完成moving_to_left操作后变回原来的方向  
    int temp[4][4], i, j, k;    //数组temp临时存放改变方向的面板  
    k = 0;  
    switch (orig_direction)  
    {  
    case 'a':   //原地不动  
        for (i = 0; i < 4; ++i)  
            for (j = 0; j < 4; ++j)  
                temp[i][j] = panel[k++];  
        break;  
    case 's':   //顺时针旋转90度  
        for (i = 0; i < 4; ++i)  
            for (j = 0; j < 4; ++j)  
                temp[j][3 - i] = panel[k++];  
        break;  
    case 'd':   //关于中间竖线轴对称变换（不采用旋转180度即中心对称的方法）  
        for (i = 0; i < 4; ++i)  
            for (j = 0; j < 4; ++j)  
                temp[i][3 - j] = panel[k++];  
        break;  
    case 'w':   //逆时针旋转90度  
        for (i = 0; i < 4; ++i)  
            for (j = 0; j < 4; ++j)  
                temp[3 - j][i] = panel[k++];  
        break;  
    default:  
        break;  
    }  
    //将临时面板temp赋值给原面板  
    k = 0;  
    for (i = 0; i < 4; ++i)  
        for (j = 0; j < 4; ++j)  
            panel[k++] = temp[i][j];  
}  
  
void moving(int panel[16], char direction) {  
    //移动操作（移动整个游戏面板）  
    //direction：移动方向  
    switch (direction)  
    {  
    case 'a':  
        moving_to_left(panel);  
        break;  
    case 's':  
        change_direction_to_left(panel, 's');   //顺时针旋转90度  
        moving_to_left(panel);  
        change_direction_to_left(panel, 'w');   //逆变换：逆时针旋转90度  
        break;  
    case 'd':  
        change_direction_to_left(panel, 'd');   //关于中间竖线轴对称变换  
        moving_to_left(panel);  
        change_direction_to_left(panel, 'd');   //逆变换和正变换相同  
        break;  
    case 'w':  
        change_direction_to_left(panel, 'w');   //逆时针旋转90度  
        moving_to_left(panel);  
        change_direction_to_left(panel, 's');   //逆变换：顺时针旋转90度  
        break;  
    default:  
        break;  
    }  
}  
  
void add_new_number(int panel[16], int new_number_pos, int new_number) {  
    //在指定空白位置添加一个新数字  
    //new_number_pos为新数字的位置，new_number为新数字  
    if(panel[new_number_pos - 1] == 0)  
        panel[new_number_pos - 1] = new_number;  
    else  
    {  
        int i = new_number_pos;  
        while (panel[i] != 0)   i++;  
        panel[i] = new_number;  
    }  
}  
  
int main() {  
    //游戏面板(panel)  
    //键盘方向(direction)：'a'表示向左移动，'s'表示向下移动，'d'表示向右移动，'w'表示向上移动  
    //新增块位置(new_number_pos)，新增数字(new_number)  
    int panel[16], new_number_pos[10], new_number[10], i, counter;  
    char direction[10], c;  
      
    //读取游戏信息  
    counter = 0;  
    for (i = 0; i < 16; ++i)  
        scanf("%d", panel + i);  
    getchar();  
    do {  
        direction[counter] = getchar(); getchar();  
        new_number_pos[counter] = getchar() - '0'; getchar();  
        new_number[counter] = getchar() - '0';  
        counter++;  
    } while ((c = getchar()) != '\n');  
  
    //执行操作  
    for (i = 0; i < counter; ++i)  
    {  
        moving(panel, direction[i]);  
        add_new_number(panel, new_number_pos[i], new_number[i]);  
    }  
  
    //输出移动后结果  
    for (i = 0; i < 15; ++i)  
        printf("%d ", panel[i]);  
    printf("%d\n", panel[15]);  
  
    system("pause");  
    return 0;  
}  

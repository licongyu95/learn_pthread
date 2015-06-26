#include<stdio.h>

int main(int argc, char* argv[])
{
    /* "abcd"是常量,存放在常量区,不允许修改它的值 */
    char* p = "abcd";

    /* 执行下面的赋值会core dump */
    *p = '1';
    return 0;
}

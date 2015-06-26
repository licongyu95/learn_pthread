#include<stdio.h>

/* ERROR */
void setString1(char* p)
{
    char* ptr = "hello world.";
    p = ptr;
    return;
}

/* OK */
void setString2(char** p)
{
    char* ptr = "hello world.";
    *p = ptr;
    return;
}

void main(void)
{
    char* p = NULL;
    setString1(p); /* 传的是变量,值传递 */
    printf("String1: %s\n", p);

    setString2(&p); /* 传的是变量的地址, 指针传递 */
    printf("String2: %s\n", p);
}

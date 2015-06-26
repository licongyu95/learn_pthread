#include<stdio.h>

int a = 0; /*全局初始化区 */
char *p1; /* 全局未初始化区 */
main() 
{ 
    int b;/* 栈 */
    char *p2; /* 栈 */

    char s[] = "abc"; /* 栈 */
    char *p3 = "123456"; /* 字符串123456\0在常量区，p3在栈上. */

    static int c =0; /* 全局（静态）初始化区 */ 

    p1 = (char *)malloc(10); /* 堆 */
    p2 = (char *)malloc(20); /* 堆 */
}

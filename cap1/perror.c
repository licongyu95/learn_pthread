#include<stdio.h>
#include <errno.h>
#include <string.h>

void main(void)
{
    extern int errno; /* errno为全局变量,先引用再使用 */
    errno = 5;
    perror("hello");
}

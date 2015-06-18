#include<stdio.h>
#include<stdlib.h>


/* 定义一个返回int类型的函数指针别名 */
typedef int (*FUNC)(char*);
/* 定义一个数组别名 */
typedef char a[20];
/* 定义一个指针数组别名 */
typedef char* b[20];
/* 定义一个结构体别名 */
typedef struct massage
{
    int num;
    char* ms;
}mss_t;

int testfunc(char* str)
{
    printf("MSG: %s\n", str);
    return 0;
}

void main(void)
{
    int i;
    a msg;
    b arr;

    printf("Please input message to print:\n");
    scanf("%s", msg);
    FUNC f = testfunc;

    (*f)(msg);

    for( i=0; i<(sizeof(arr)/sizeof(char*)); i++ )
    { 
        arr[i] = malloc(50);
        sprintf(arr[i],"message:%d",i);
        f(arr[i]);
        free(arr[i]);
    }
}

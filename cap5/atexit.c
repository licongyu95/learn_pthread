#include<stdlib.h>
#include<stdio.h>

void out(void)
{
    printf("atexit() succeeded.\n");
}

int main()
{
    if( atexit(out) )
        fprintf(stderr, "atexit() failed.\n");
    printf("hello world.\n");
    return 0;
}

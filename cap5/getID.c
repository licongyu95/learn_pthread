#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

void main()
{
    printf("PID: %6d\n", (int)getpid());
    printf("PPID: %5d\n", (int)getppid());
}

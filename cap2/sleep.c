#include<stdio.h>
#include<sys/times.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

void sleep_usec(struct timeval tv)
{
    int ret;
    printf("sleep %d.%d seconde(s)\n", (int)tv.tv_sec, (int)tv.tv_usec);
    ret = select(0, NULL, NULL, NULL, &tv);
    if( ret == -1 )
        perror("select");
}

void main(void)
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 100;
    sleep_usec(tv);
}

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

/* 注册一个信号, 并等待信号 */

void new_op(int,siginfo_t*,void*);

int main(int argc,char**argv)
{
    if( argc < 2 )
    {
        fprintf(stderr, "Useage: %s <sigNum>\n", argv[0]);
        return 1;
    }

    struct sigaction act;  
    int sig;

    sig=atoi(argv[1]); 
    sigemptyset(&act.sa_mask);

    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=new_op;

    if(sigaction(sig,&act,NULL) < 0)
    {
        printf("install sigal error\n");
    }

    while(1)
    {
        printf("wait for the signal\n");
        sleep(2);
    }
}

void new_op(int signum,siginfo_t *info,void *myact)
{
    printf("receive signal %d\n", signum);
    sleep(1);
}

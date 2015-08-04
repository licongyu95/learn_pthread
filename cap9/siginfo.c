#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<string.h>

/* 发送信号并携带信息 */

void new_op(int,siginfo_t*,void*);

int main(int argc,char**argv)
{
    if( argc < 2 )
    {
        fprintf(stderr, "Useage: %s <sigNum>\n", argv[0]);
        return -1;
    }
    struct sigaction act;  
    union sigval mysigval;
    int i;
    int sig;
    pid_t pid;         
    char data[10];
    memset(data,0,sizeof(data));

    for(i=0;i < 5;i++)
        data[i]='a';
    mysigval.sival_ptr=data;

    sig=atoi(argv[1]);
    pid=getpid();

    sigemptyset(&act.sa_mask);
    act.sa_flags=SA_SIGINFO; /* 信息传递开关，允许传说参数信息给new_op */
    act.sa_sigaction=new_op;
    /* 三参数信号处理函数 
     * void my_handler (int signo, siginfo_t *si, void *ucontext);
     */

    /*
     * #include <signal.h>
     * int sigaction (int signo, const struct sigaction *act, struct sigaction *oldact);
     * struct sigaction {
     *     void (*sa_handler)(int); 
     *     void (*sa_sigaction)(int, siginfo_t *, void *);
     *     sigset_t sa_mask; 
     *     int sa_flags; 
     *     void (*sa_restorer)(void);
     * }
    */
    if(sigaction(sig,&act,NULL) < 0)
    {
        printf("install sigal error\n");
    }

    while(1)
    {
        sleep(2);
        printf("wait for the signal\n");
        /*
         * #include <signal.h>
         * int sigqueue (pid_t pid, int signo, const union sigval value);
         *
         * union sigval {
         *   int sival_int;
         *   void *sival_ptr;
         * };
         */
        sigqueue(pid,sig,mysigval);//向本进程发送信号，并传递附加信息
    }
}

void new_op(int signum,siginfo_t *info,void *myact)//三参数信号处理函数的实现
{
    int i;
    printf("%s\n ",(char*)((*info).si_ptr));
    printf("handle signal %d over\n",signum);
}

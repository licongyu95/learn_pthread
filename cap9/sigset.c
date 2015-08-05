#include "signal.h"
#include "unistd.h"
#include<stdio.h>

static void my_op(int);

void main()
{
    sigset_t new_mask,old_mask,pending_mask;

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=(void*)my_op;

    /* 注册信号44 */
    if(sigaction(SIGRTMIN+10,&act,NULL))
        printf("install signal SIGRTMIN+10 error\n");

    sigemptyset(&new_mask);
    sigaddset(&new_mask,SIGRTMIN+10);

    /* 阻塞信号44, 备份原信号集 */
    if(sigprocmask(SIG_BLOCK, &new_mask,&old_mask))
        printf("block signal SIGRTMIN+10 error\n");

    sleep(30);    /* 此期间收到的信号44,会被阻塞 */
    printf("now begin to get pending mask and unblock SIGRTMIN+10\n");
    /* 获取等待信号 */
    if(sigpending(&pending_mask)<0)
        printf("get pending mask error\n");

    if(sigismember(&pending_mask,SIGRTMIN+10)) /* 如果有收到过信号44, 就会显示被pending */
        printf("signal SIGRTMIN+10 is pending\n");
    if(sigismember(&pending_mask,10))
        printf("signal 10 is pending\n");

    /* 还原信号集, 所有信号不被人为阻塞 */
    if(sigprocmask(SIG_SETMASK,&old_mask,NULL)<0)
        printf("unblock signal error\n");
    printf("signal unblocked\n");
    sleep(10);
}

static void my_op(int signum)
{
    printf("receive signal %d \n",signum);
}

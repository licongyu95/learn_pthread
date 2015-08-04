#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t pid;

/* 子进程 1 SIGALRM 信号处理函数 */
static void wakeup(int dummy)
{
    printf("I (pid = %d) am up now\n",pid);
}

/* 子进程 1 SIGINT 信号处理函数 */
static void handler(int dummy)
{
    printf("I (pid = %d) got an interrupt, will exit\n",pid);
    exit(0);
}

/* 子进程 2 信号处理函数 */
static void trapper(int i)
{
    if(i == SIGUSR1)
    {
        printf("I (pid = %d) got SIGUSR1,will exit\n", pid);
        exit(0);
    }
    else
    {
        printf("I (pid = %d) got signal %d, will continue\n", pid, i);
    }
}

/* 父进程信号处理函数 */
void parent(int sig)
{
    printf("Signal (%d) received by parent (%d)\n", sig, pid);
}


int main(int argc, char *argv[])
{
    int i, cpid1, cpid2;

    printf("Number of signal is %d\n", NSIG);   //输出系统中信号的个数

    if(!(cpid1 = fork()))  //创建第一个子进程
    {
        pid = cpid1 = getpid();  //获得子进程的进程号
        printf("CPID1 = %d\n", cpid1);

        for(i=1; i<NSIG; i++)
        {
            signal(i, SIG_IGN);  //忽略所以的信号
        }

        signal(SIGINT, handler); //捕获信号 SIGINT
        signal(SIGALRM, wakeup); //捕获超时信号
        alarm(2);                //启动定时器，设置 2 秒后超时

        for(; ;)
        {
            pause();  //等待信号
        }

        printf(" -- CPID1 (%d) terminates\n", cpid1);

        exit(0);
    }
    else if(!(cpid2 = fork()))  //创建第二个子进程
    {
        pid = cpid2 = getpid();
        printf("CPID2 = %d\n", cpid2);

        for(i=1; i<NSIG; i++)
        {
            signal(i, trapper);  //捕获所有的信号
        }

        for(; ;)
        {
            pause();  //等待信号
        }

        printf(" -- CPID2 (%d) terminates\n", cpid2);

        exit(0);
    }

    /* 下面是父进程执行的代码 */
    pid = getpid();  //取得 PID
    sleep(3);        //睡眠，让子进程先运行
    printf("This is parent process (pid = %d)\n", pid);

    for(i=1; i<NSIG; i++)
    {
        signal(i, parent);  //捕获所以信号
    }

    printf("Send SIGUSR1(%d) to CPID1 (%d)\n", SIGUSR1, cpid1);
    kill(cpid1, SIGUSR1);
    printf("Send SIGINT(%d) to CPID1 (%d)\n", SIGINT, cpid1);
    kill(cpid1, SIGINT);
    printf("Send SIGINT(%d) to CPID2 (%d)\n", SIGBUS, cpid2);
    kill(cpid2, SIGINT);
    printf("Send SIGUSR1(%d) to CPID2 (%d)\n", SIGUSR1, cpid2);
    kill(cpid2, SIGUSR1);

    for(; wait((int *)0) > 0; );  //等待子进程结束

    return 0;
}

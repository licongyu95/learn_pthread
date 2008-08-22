#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include <linux/limits.h>

int main(void)
{
    pid_t pid;
    int i;
    int fd;

    /* 让init进程成为新产生进程的父进程, 且不是组长进程 */
    pid = fork();
    if( pid == -1 )
        return -1;
    else if( pid != 0 )
        exit(EXIT_SUCCESS);


    /* 创建新的进程组合新的会话 */
    if( setsid() == -1 )
        return -1;

    /* 更改当前工作目录 */
    if( chdir("/") == -1 )
        return -1;

    /* 关闭文件描述符 */
    for( i= 0; i < NR_OPEN; i++ )
        close(i);

    /* 将标准输入输出重定向到空设备 */
    fd = open("/dev/null", O_RDWR);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    /* do its daemon thing... */
    //...
    sleep(100);
    return 0;

    /*
     * #include <unistd.h>
     * int daemon (int nochdir, int noclose);
     */
}

#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

#define READNUM 2
#define WRITENUM 2
#define MAXLEN 20
#define TIMEOUT 30

/*
 * include <sys/select.h>
 * According to earlier standards
 * #include <sys/time.h>
 * #include <sys/types.h>
 * #include <unistd.h>
 * int select(int nfds, fd_set *readfds, fd_set *writefds,
 *                      fd_set *exceptfds, struct timeval *timeout);
 * void FD_CLR(int fd, fd_set *set);
 * int  FD_ISSET(int fd, fd_set *set);
 * void FD_SET(int fd, fd_set *set);
 * void FD_ZERO(fd_set *set);
 */

void main(int argc, char* argv[])
{
    int item,ret;
    char* fileName;
    int fd[READNUM + WRITENUM] = {0,};
    fd_set* readfds = NULL;
    fd_set* writefds = NULL;
    struct timeval tv;
    struct timespec ts;

    fileName = (char*)malloc(MAXLEN * sizeof(char));
    readfds = (fd_set*)malloc(READNUM * sizeof(fd_set));
    writefds = (fd_set*)malloc(WRITENUM * sizeof(fd_set));
    /* fd_set* exceptfds = NULL; */

    /* clear fd ptr */
    FD_ZERO(writefds);
    FD_ZERO(readfds);

    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    ts.tv_sec = TIMEOUT;
    ts.tv_nsec = 0;

    /* open 2 files to write*/
    for( item=0; item < WRITENUM; item++ )
    {
        sprintf(fileName, "writeFile%d", item);
        fd[item] = open(fileName, O_RDWR | O_CREAT,
                S_IRWXU | S_IRWXG | S_IRWXO);
        if( fd[item] == -1 )
            perror("open");
        FD_SET(fd[item], writefds);
    }


    /* open 2 files to read*/
    for( item=WRITENUM; item < (WRITENUM + READNUM); item++ )
    {
        sprintf(fileName, "readFile%d", item);
        fd[item] = open(fileName, O_RDWR | O_CREAT,
                S_IRWXU | S_IRWXG | S_IRWXO);
        if( fd[item] == -1 )
            perror("open");
        FD_SET(fd[item], readfds);
    }

    for( item=0; item < (WRITENUM + READNUM); item++ )
        if( FD_ISSET(fd[item], readfds) || FD_ISSET(fd[item], writefds) )
            printf("fd:%d\n", fd[item]);

    /* select:
     * int select(int nfds, fd_set *readfds, fd_set *writefds,
     *              fd_set *exceptfds, struct timeval *timeout);
     */
    ret = select(maxFd(fd),
            readfds,
            writefds,
            NULL,
            &tv);
    if(ret == -1)
    {
        perror("select");
    }
    else if(!ret)
    {
        printf("%d seconds elapsed.\n", TIMEOUT);
    }
    else
        printf("%d fds is prepared.\n", ret);

    /* pselect:
     *
     * #include <sys/select.h>
     * 
     * int pselect(int nfds, fd_set *readfds, fd_set *writefds,
     * fd_set *exceptfds, const struct timespec *timeout,
     * const sigset_t *sigmask);
     */

    ret = 0;
    ret = pselect(maxFd(fd),
            readfds,
            writefds,
            NULL,
            &ts,
            NULL);
    if(ret == -1)
    {
        perror("pselect");
    }
    else if(!ret)
    {
        printf("%d seconds elapsed.\n", TIMEOUT);
    }
    else
        printf("%d fds is prepared.\n", ret);

    if(readfds)
        free(readfds);
    if(writefds)
        free(writefds);
    if(fileName)
        free(fileName);
    for( item=0; item < (WRITENUM + READNUM); item++)
        if( fd[item])
        {
            printf("close fd:%d\n", fd[item]);
            close(fd[item]);
        }

}

int maxFd(int* fd)
{
    int i;
    int max = 0;
    for(i = 0; i < READNUM + WRITENUM; i++)
        (fd[i] > max) ? max = fd[i] : max;
    printf("max fd num is %d\n", max);
    return (max + 1);

}

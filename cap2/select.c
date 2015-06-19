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

void main(int argc, char* argv[])
{
    int item;
    char* fileName;
    int fd[READNUM + WRITENUM] = {0,};
    fd_set* readfds = NULL;
    fd_set* writefds = NULL;

    fileName = (char*)malloc(MAXLEN * sizeof(char));
    readfds = (fd_set*)malloc(READNUM * sizeof(fd_set));
    writefds = (fd_set*)malloc(WRITENUM * sizeof(fd_set));
    /* fd_set* exceptfds = NULL; */
    struct timeval* timeout = NULL;

    /* clear fd ptr */
    FD_ZERO(writefds);
    FD_ZERO(readfds);
    /* FD_ZERO(exceptfds); */

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
        FD_SET(fd[item], writefds);
    }

    for( item=0; item < (WRITENUM + READNUM); item++ )
        if( FD_ISSET(fd[item], readfds) || FD_ISSET(fd[item], writefds) )
            printf("fd:%d\n", fd[item]);
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

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define FIFO_SERVER "/tmp/fifoserver"

main(int argc,char** argv)
{
    char r_buf[4096*2];
    int  fd;
    int  r_size;
    int  ret_size;

    if( argc < 2 )
    {
        fprintf(stderr, "Useage: %s <Size>.\n", argv[0]);
        exit(1);
    }

    r_size=atoi(argv[1]);
    printf("requred real read bytes %d\n",r_size);
    memset(r_buf,0,sizeof(r_buf));

    /* 在此处可以把读程序编译成两个不同版本：阻塞版本及非阻塞版本 */
    /* fd=open(FIFO_SERVER,O_RDONLY|O_NONBLOCK,0); */
    fd=open(FIFO_SERVER,O_RDONLY,0);
    if(fd==-1)
    {
        printf("open %s for read error\n", FIFO_SERVER);
        exit(EXIT_FAILURE); 
    }
    while(1)
    {

        memset(r_buf,0,sizeof(r_buf));
        ret_size=read(fd,r_buf,r_size);
        if(ret_size==-1)
            if(errno==EAGAIN)
                printf("no data avlaible\n");
        printf("real read bytes %d\n",ret_size);
        sleep(1);
    }   
    pause();
    unlink(FIFO_SERVER);
}

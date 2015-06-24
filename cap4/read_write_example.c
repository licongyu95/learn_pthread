#include<stdio.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>

int main(void)
{
    struct iovec iov[3];
    ssize_t nr;
    int fd, i;

    char *wbuf[] = {
        "The term buccaneer comes from the word boucan.\n",
        "A boucan is a wooden frame used for cooking meat.\n",
        "Buccanerr is the West Indies name for a pirate.\n"
    };

    char rbuf[3][60] = {'\0',};

    fd = open("buccaneer.txt", O_RDWR|O_CREAT|O_TRUNC, 
            S_IRWXU|S_IRWXG|S_IRWXO);
    if( fd == -1 )
    {
        perror("open");
        return 1;
    }

    for( i=0; i<3; i++)
    {
        iov[i].iov_base = wbuf[i];
        /* iov[i].iov_len = strlen(wbuf[i]); */
        iov[i].iov_len = strlen(wbuf[i]) + 1; /* 加1,把结尾的'\0'也写入 */
    }

    nr = writev(fd, iov, 3);
    if( nr == -1 )
    {
        perror("writev");
        return 1;
    }
    printf("write %ld bytes.\n", (long int)nr);

    sync();

    lseek(fd, 0, SEEK_SET); /* rewind the pos */

    for( i=0; i<3; i++)
    {
        iov[i].iov_base = rbuf[i];
        iov[i].iov_len = sizeof(rbuf[i]); /* count the array lenght */
        printf("iov%d.iov_len = %d\n", i, (int)sizeof(rbuf[i]));
    }
    nr = readv(fd, iov, 3);
    if( nr == -1 )
    {
        perror("readv");
        return 1;
    }
    printf("read %ld bytes.\n", (long int)nr);
    for( i=0; i<3; i++)
    {
        printf("read buf %d: %s\n", i, rbuf[i]);
        /* 输出结果:
         * read buf 0: The term buccaneer comes from the word boucan.
         *
         * read buf 1: a wooden frame used for cooking meat.
         *
         * read buf 2:  Indies name for a pirate.
         *
         * 这是由于写的时候把'\0'也写进来了,导致打印时候会在此结束.
         */
    }

    if(close(fd))
    {
        perror("close");
        return 1;
    }
    return 0;
}

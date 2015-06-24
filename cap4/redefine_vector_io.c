#include<unistd.h>
#include<sys/uio.h>

/* 在用户空间实现简单的readv()/writev()函数 */

ssize_t naive_writev(int fd, const struct iovec *iov, int count)
{
    ssize_t ret = 0;
    int i;

    for( i=0; i<count; i++)
    {
        ssize_t nr;
        nr = write(fd, iov[i].iov_base, iov[i].iov_len);
        if( nr == -1 )
        {
            ret = -1;
            break;
        }
        ret += nr;
    }
    return ret;
}

ssize_t naive_readv(int fd, const struct iovec *iov, int count)
{
    ssize_t ret = 0;
    int i;

    for( i=0; i<count; i++)
    {
        ssize_t nr;
        nr = read(fd, iov[i].iov_base, iov[i].iov_len);
        if( nr == -1 )
        {
            ret = -1;
            break;
        }
        ret += nr;
    }
    return ret;
}



#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

void readLine(void)
{
}

int openFile(void)
{
    int fd;
    fd = open("testFile.txt", O_RDWR|O_NONBLOCK|O_CREAT|O_SYNC, 
            S_IRWXU|S_IRWXG|S_IRWXO);
    if( fd == -1 )
        perror("open");
    return fd;
}

void readFile(int fd, char* buf, size_t size)
{
    int ret;
    printf("read fd:%d\n", fd);
    while(size != 0 && (ret = read(fd, buf, size)) != 0)
    {
        if( ret == -1 )
        {
            if( errno == EINTR )
                continue;
            perror("read");
            break;
        }
        size -= ret;
        buf  += ret;
    }
    printf("readFile:%s\n", buf);
}

void writeFile(int fd, char*buf, size_t len)
{
    int ret;
    printf("write fd:%d\n", fd);
    while(len != 0 && (ret = write(fd, buf, len)) != 0)
    {
        if( ret == -1 )
        {
            if( errno == EINTR )
                continue;
            perror("write");
            break;
        }
        len -= ret;
        buf  += ret;
    }
    printf("last time write:%d\n", ret);
    ret = fdatasync(fd);
    if (ret == -1)
        perror("fdatasycn");
    ret = fsync(fd);
    if (ret == -1)
        perror("fsycn");
    sync();
}

void preadFile(int fd, char* buf, size_t size, off_t pos)
{
	int ret;
	ret = pread(fd, buf, size, pos);
	if( ret == -1 )
	perror("pread");
	printf("pread read %d char(s)\n", ret);
}

void pwriteFile(int fd, char* buf, size_t size, off_t pos)
{
	int ret;
	ret = pwrite(fd, buf, size, pos);
	if( ret == -1 )
	perror("pwrite");
	printf("pwrite write %d char(s)\n", ret);
}

void lseekEnd(int fd)
{
	int ret;
	ret = lseek(fd, (off_t)0, SEEK_END);
	//ret = lseek(fd, (off_t)1024, SEEK_END);
	if( ret == (off_t)-1 )
	perror("lseek");
}

void lseekSet(int fd, off_t pos)
{
	int ret;
	ret = lseek(fd, pos, SEEK_SET);
	if( ret == (off_t)-1 )
	perror("lseek");
}

void lseekCur(int fd)
{
	int ret;
	ret = lseek(fd, (off_t)0, SEEK_CUR);
	if( ret == (off_t)-1 )
	perror("lseek");
}

void trunca (const char *path, off_t len)
{
	int ret;
	ret = truncate (path, len);
	if( ret == -1 )
	perror("truncate");
}

void ftrunc (int fd, off_t len)
{
	int ret;
	ret = ftruncate (fd, len);
	if( ret == -1 )
	perror("truncate");
}

void main(void)
{
    int fd;
    char* readBuf = NULL;
    readBuf = malloc( 20 * sizeof(char) );
    memset(readBuf, 'x', 20);
    printf("readBuf:%s\n", readBuf);

    char* writeBuf = "hello world";
    fd = openFile();
    writeFile(fd, writeBuf, (size_t)strlen(writeBuf));

    //lseek(fd, 0, SEEK_SET);
    lseekSet(fd, 0);
    readFile(fd, readBuf, (size_t)strlen(writeBuf));
    printf("readFile in func main:%s\n", readBuf);
    if( fd )
        if( close(fd) == -1 )
        {
            if(errno == EIO)
                printf("EIO");
            else
                perror("close");
        }
}



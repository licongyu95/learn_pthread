#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int get_inode(int fd)
{
    struct stat buf;
    int ret;
    ret = fstat(fd, &buf); /* 获取文件信息 */
    if(ret == -1)
    {
        perror("stat");
        return 1;
    }
    return buf.st_ino;
}

int main(int argc, char* argv[])
{
    int fd, inode;
    if(argc < 2)
    {
        fprintf(stderr, "Useage: %s <file>.\n",
                argv[0]);
        return 1;
    }
    fd = open(argv[1], O_RDONLY | O_CREAT, 
            S_IRWXU | S_IRWXG | S_IRWXO);
    if( fd < 0)
    {
        perror("open");
        return 1;
    }
    inode = get_inode(fd);
    printf("%s:%d\n", argv[1], inode);
}

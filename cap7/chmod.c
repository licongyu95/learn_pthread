#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

void main()
{
    int fd, ret;
    struct group *gr;

    fd = open("testfile", O_RDWR | O_CREAT,
            S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0)
    {
        perror("open");
        return;
    }

    gr = getgrnam("chmod.c");
    if (!gr)
    {
        perror("getgrnam");
        return;
    }

    ret = fchown(fd, -1, -1);
    if (ret)
    {
        perror("fchown");
        return;
    }

    ret = fchown(fd, -1, gr->gr_gid);
    if (ret)
    {
        perror("fchown");
        return;
    }

}

#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
    int fd, ret;
    struct stat st;
    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s <file>.", 
                argv[0]);
        return;
    }
    fd = open("testFile", O_RDONLY | O_CREAT, 
            S_IRWXU | S_IRWXG | S_IRWXO);
    if(fd < 0)
    {
        perror("open");
        return;
    }
    ret = fstat(fd, &st);
    if(ret)
    {
        perror("fstat");
        return;
    }
    return gun_dev_major(st.st_dev);
}

#include "debug.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void main(int argc, char* argv[])
{
    int fd;
    char* ret;

    fd = open("abc", O_RDONLY);
    if (fd < 0)
    {
        log("open");
    }
}

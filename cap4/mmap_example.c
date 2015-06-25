#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>

int main(int argc, char* argv[])
{
    struct stat st;
    off_t len, pageSize;
    char *p;
    int fd;

    if( argc < 2 )
    {
        fprintf(stderr, "Useage: %s <file>.",
                argv[0]);
        return 1;
    }

    /* fd = open(argv[1], O_RDONLY); */
    fd = open(argv[1], O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        return 1;
    }
    if ( !S_ISREG(st.st_mode) )
    {
        fprintf(stderr, "%s is not a file.\n",
                argv[1]);
        return 1;
    }

    /* if arg 'len' is 0,  mmap()/munmap() will return fail */
    if (st.st_size == 0)
        pageSize = sysconf(_SC_PAGESIZE); /* get system's default page size */
    else
        pageSize = st.st_size;

    p  = mmap(0, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED )
    {
        perror("mmap");
        return 1;
    }
    if (close(fd) == -1)
    {
        perror("close fd");
        return 1;
    }
    for (len = 0; len < st.st_size; len++ )
        putchar(p[len]);
    if (munmap(p, pageSize) == -1)
    {
        perror("munmap");
        return 1;
    }
    return 0;
}

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int main(int argc, char* argv[])
{
    struct stat st;
    int ret;
    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s <file>.\n",
                argv[0]);
        return 1;
    }

    ret = stat(argv[1], &st);
    if(ret)
    {
        perror("stat");
        return 1;
    }
    printf("%s is %ld bytes.\n", argv[1], st.st_size);
    return 0;
}

#include<unistd.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
    int ret;
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s <oldpath> <newpath>.\n",
                argv[0]);
        return 1;
    }

    /* create link */
    ret = link(argv[1], argv[2]);
    if (ret)
    {
        perror("link");
        return 1;
    }
    /* rename link file */
    ret = rename(argv[2], "renameFile");
    if (ret)
    {
        perror("rename");
        return 1;
    }
    /* delete link */
    ret = unlink("renameFile");
    if (ret)
    {
        perror("unlink");
        return 1;
    }

    /* create symlink */
    ret = symlink(argv[1], argv[2]);
    if (ret)
    {
        perror("symlink");
        return 1;
    }

    /* delete link (glibc) */
    ret = remove(argv[2]);
    if (ret)
    {
        perror("remove");
        return 1;
    }
}

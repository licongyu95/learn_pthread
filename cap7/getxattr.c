#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<attr/xattr.h>
#include<fcntl.h>


void main(int argc, char* argv[])
{
    int i, fd, ret;
    char buf[BUFSIZ];
    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s <file>.\n",
                argv[0]);
        return;
    }
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0)
    {
        perror("__LINE__:open");
        return;
    }

    /* set attr */
    ret = fsetxattr(fd, "user.test1", "test1", sizeof("test1"), XATTR_CREATE);
    if (ret)
    {
        perror("__LINE__:fsetxattr.test1");
        return;
    }
    ret = setxattr(argv[1], "user.test2", "test2", sizeof("test2"), 0);
    if (ret)
    {
        perror("__LINE__:setxattr.test2");
        return;
    }
    ret = fsetxattr(fd, "user.test3", "test3333", sizeof("test3"), 0);
    if (ret)
    {
        perror("__LINE__:fsetxattr.test3");
        return;
    }
    ret = fsetxattr(fd, "user.test3", "test3", sizeof("test3"), XATTR_REPLACE);
    if (ret)
    {
        perror("__LINE__:fsetxattr.test3");
        return;
    }

    /* get attr */
    ret = getxattr(argv[1], "user.test3", buf, BUFSIZ);
    if (ret == -1)
    {
        perror("__LINE__:getxattr.test3");
        return;
    }
    printf("user.test3:%s\n", buf);

    ret = fgetxattr(fd, "user.test1", buf, BUFSIZ);
    if (ret == -1)
    {
        perror("__LINE__:fgetxattr.test1");
        return;
    }
    printf("user.test1:%s\n", buf);
    
    /* list attr info */
    ret = listxattr(argv[1], buf, BUFSIZ);
    if (ret == -1)
    {
        perror("__LINE__:listxattr");
        return;
    }

    //printf("listxattr: %s\n", buf);
    for (i = 0; i < ret; i++)
        putc(buf[i], stdout);
    putc('\n', stdout);

    ret = flistxattr(fd, buf, BUFSIZ);
    if (ret == -1)
    {
        perror("__LINE__:flistxattr");
        return;
    }
    //printf("flistxattr: %s\n", buf);
    for (i = 0; i < ret; i++)
        putc(buf[i], stdout);
    putc('\n', stdout);

    /* delete attr */
    ret = removexattr(argv[1], "user.test1");
    if (ret != 0)
    {
        perror("__LINE__:removexattr");
        return;
    }
    printf("user.test1 deleted.\n");
    ret = fremovexattr(fd, "user.test3");
    if (ret != 0)
    {
        perror("__LINE__:fremovexattr");
        return;
    }
    printf("user.test3 deleted.\n");

    /* list xattr again */
    ret = flistxattr(fd, buf, BUFSIZ);
    if (ret == -1)
    {
        perror("__LINE__:flistxattr again");
        return;
    }
//    printf("flistxattr again: %s\n", buf);
    for (i = 0; i < ret; i++)
        putc(buf[i], stdout);
    putc('\n', stdout);

    unlink(argv[1]);
}

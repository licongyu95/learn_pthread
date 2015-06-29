#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
#include<errno.h>

int find_file_in_dir(const char* path, const char* file)
{
    int ret = -1;
    struct dirent* entry;
    DIR* dir;

    dir = opendir(path); /* 打开目录 */
    if (dir < 0)
    {
        perror("opendir");
        return 1;
    }

    errno = 0;
    while ((entry = readdir(dir)) != NULL) /* 遍历读取目录 */
    {
        if (!strcmp(entry->d_name, file)) /* 查找文件 */
        {
            ret = 0;
            break;
        }
    }

    if (errno && !entry)
    {
        perror("readdir");
        return 1;
    }
    return ret;
}

int main(int argc, char* argv[])
{
    int ret = -1;
    char cwd[BUFSIZ];
    char* path = NULL;
    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s <file>.\n",
                argv[0]);
        return 1;
    }

    path = getcwd(cwd, BUFSIZ); /* 获取当前目录 */
    if (!path)
    {
        perror("getcwd");
        return 1;
    }
    ret = find_file_in_dir(".", argv[1]); /* 在当前目录查找文件 */
    if (!ret)
    {
        printf("found.\n");
        return 0;
    }
    printf("not found.\n");
    return 1;
}

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
    struct stat st;

    dir = opendir(path); /* 打开给定目录 */
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

    ret = stat(path, &st);
    if (ret == -1)
    {
        perror("stat");
        return 1;
    }
    if (S_ISREG(st.st_mode)) /* 如果path是文件,直接比较 */
        if (!strcmp(path, file))
        {
            printf("found.\n");
            return 0;
        }
    else if (IS_DIR(st.st_mode)) /* 如果path是目录,递归调用 */
        find_file_in_dir(path, file);
    else
        return 1;
}

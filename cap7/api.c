/************一组stat函数**************/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int stat(const char *path, struct stat *buf); /* 文件用path表示 */
int fstat(int fd, struct stat *buf); /* 文件用fd表示 */
int lstat(const char *path, struct stat *buf); /* link文件而非目标文件*/

struct stat{
    dev_t st_dev; /* device */
    ino_t st_ino; /* inode */
    mode_t st_mode; /* file mode */
    nlink_t st_nlink; /* hard link num */
    uid_t st_uid; 
    gid_t st_gid;
    dev_t st_rdev;
    off_t st_size; /* file size count by bytes */
    blksize_t st_blksize; /* prefer IO block size */
    blkcnt_t st_blocks;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
}

/************权限设置*****************/
#include<sys/types.h>
#include<sys/stat.h>
int chmod(const char* path, mode_t mode);
int fchmod(int fd, mode_t mode);

/************文件所有者***************/
#include<sys/types.h>
#include<unistd.h>
int chown(const char *path, uid_t owner, gid_t group); /* 文件路径 */
int lchown(const char *path, uid_t owner, git_t group); /* 链接文件 */
int fchown(int fd, uid_t owner, git_t group); /* 文件句柄 */

/*******************扩展属性*********************/
/* linux system四种命名空间:
 * system
 * security
 * trusted
 * user
 */
#include<sys/types.h>
#include<attr/xattr.h>
/* 获取属性 */
ssize_t getxattr(const char *path, const char *key,
        void *buf, size_t size);
ssize_t lgetxattr(const char *path, const char *key,
        void *buf, size_t size);
ssize_t fgetxattr(int fd, const char *key, void *buf, size_t size);

/* 设置属性 */
#include<sys/types.h>
#include<attr/xattr.h>
int setxattr(const char *path, const char *key,
        const void *value, size_t size, int flags);
int lsetxattr(const char *path, const char *key,
        const void *value, size_t size, int flags);
int fsetxattr(int fd, const char *key ,const void *value,
        size_t size, int flags);
/* flags:
 * XATTR_CREATE
 * XATTR_REPLACE
 * default 0同时允许创建和替换
 */

/* 列出扩展属性 */
#include<sys/types.h>
#include<attr/xattr.h>
ssize_t listxattr(const char *path, char *list, size_t size);
ssize_t llistxattr(const char *path, char *list, size_t size);
ssize_t flistxattr(int fd, char *list, size_t size);
/* size:
 * 如果size为0,函数将返回整个键列表的实际长度.
 */

/* 删除扩展属性 */
#include<sys/types.h>
#include<attr/xattr.h>
int removexattr(const char *path, const char *key);
int lremovexattr(const char *path, const char *key);
int fremovexattr(int fd, const char *key);

/***********************目录***************************/
/* 获取当前目录 */
#include<unistd.h>
char* getcwd(char *buf, size_t size);
/* glibc的方式: */
#define _GNU_SOURCE
#include<unistd.h>
char* get_current_dir_name(void);

/* 更改当前目录 */
#include<unistd.h>
int chdir(const char* path);
int fchdir(int fd);


/* 创建目录 */
#include<sys/stat.h>
#include<sys/types.h>
int mkdir(const char* path, mode_t mode);

/* 移除目录 */
#include<unistd.h>
int rmdir(const char* path);

/* 读取目录内容 */
#include<sys/types.h>
#include<dirent.h>
DIR* opendir(const char* name); /* 打开目录流 */

/* 从目录流中获取目录文件描述符 */
#define _BSD_SOURCE 
#include<sys/types.h>
#include<dirent.h>
int dirfd(DIR* dir);

/* 从目录流读取 */
#include<sys/types.h>
#include<dirent.h>
struct dirent* readdir(DIR* dir);
struct dirent{
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256]; /* 文件名字 */
}

/* 关闭目录流 */
#include<dirent.h>
#include<sys/types.h>
int closedir(DIR* dir);

/* 对应的系统调用,一般不用 */
#include<unistd.h>
#include<linux/types.h>
#include<linux/dirent.h>
#include<linux/unistd.h>
#include<errno.h>

int readdir(unsigned int fd, struct dirent* dirp
        unsigned int count);
int getdents(unsigned int fd, struct dirent* dirp
        unsigned int count);


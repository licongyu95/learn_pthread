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


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



/* 打开文件 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *name, int flags);
int open(const char *name, int flags, mode_t mode);
int creat(const char *name, mode_t mode);

/* 关闭文件 */
int close(fd);

/* 读文件 */
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t len);

/* 写文件 */
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);

/* 缓冲数据同步 */
#include <unistd.h>
int fsync(int fd);
int fdatasync(int fd);

/* 系统所有缓冲区同步 */
#include <unistd.h>
void sync(void);

/* 文件指针定位 */
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t pos, int origin);
/* origin:
 * SEEK_CUR
 * SEEK_SET
 * SEEK_END
 */

/* 定位读写 */
#define _XOPEN_SOURCE 500
#include<unistd.h>
ssize_t pread(int fd, void *buf, size_t len, off_t pos);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t pos);
/* 不管当前文件的pos值,都是根据参数提供的pos值进行io
 * ！！！操作完成不改变文件位置！！！
 */

/* 截断文件 */
#include<unistd.h>
#include<sys/types.h>
int ftruncate(int fd, off_t len);
int truncate(const char *path, off_t len);

/* IO多路复用:select、poll */
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
int select(int n, /* maxFD + 1 */ 
        fd_set *readfds,
        fd_set *writefds,
        fd_set *exceptfds,
        struct timeval *timeout);
FD_CLR(int fd, fd_set *set);
FD_SET(int fd, fd_set *set);
FD_ISSET(int fd, fd_set *set);
FD_ZERO(fd_set *set);

struct timeval{
    long tv_sec;
    long tv_usec;
}

#define _XOPEN_SOURCE 600
#include <sys/select.h>
int pselect(int n,
        fd_set *readfds,
        fd_set *writefds,
        fd_set *exceptfds,
        const struct timespec *timeout,
        const sigset_t *sigmask);

#include <sys/time.h>
struct timespec {
    long tv_sec; /* seconds */
    long tv_nsec; /* nanoseconds */
};

/*********************************************/
#include<sys/poll.h>
int poll(struct pollfd *fds, unsigned int nfds, int timeout);

struct pollfd{
    int fd;
    short events;
    short revents;
}
/* events:
 * POLLIN
 * POLLOUT
 * ...
 */

#define _GNU_SOURCE
#include <sys/poll.h>
int ppoll(struct pollfd *fds,
        nfds_t nfds,
        const struct timespec *timeout,
        const sigset_t *sigmask);


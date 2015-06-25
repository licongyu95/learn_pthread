/***************** 向量IO **********************/
#include<sys/uio.h>
ssize_t readv(int fd, const struct iovec *iov, int count);
ssize_t writev(int fd, const struct iovec *iov, int count);

struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};


/***************** event poll **********************/

/* poll()/select()每次调用都需要所有被监听的文件描述符.
 * 内核必须遍历所有的文件描述符.
 * epoll把监听注册从实际监听中分离出来,从而解决了这个问题.
 * 一个系统调用初始化了一个epoll上下文            epoll_create,
 * 另一个从上下文中加入/删除需要监听的文件描述符  epoll_ctl,
 * 第三个系统调用执行真正的时间等待               epoll_wait().
 */

/* epoll_create:
 *
 * #include <sys/epoll.h>
 * int epoll_create (int size);
 * the file descriptor returned by epoll_create() should be closed by using close(2).
 */
int epoll_cre(int size)
{
    int epfd;
    epfd = epoll_create(FDNUM);
    if( epfd == -1 )
        perror("epoll_create");
    return epfd;
}

void epoll_del(int epfd)
{
    close(epfd);
}

/* epoll_ctl:
 *
 * #include<sys/epoll.h>
 * int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
 *
 *
 *  typedef union epoll_data {
 *      void    *ptr;
 *      int      fd;
 *      uint32_t u32;
 *      uint64_t u64;
 *  } epoll_data_t;
 *
 * struct epoll_event {
 *   uint32_t     events;    \/* Epoll events
 *   epoll_data_t data;      \/* User data variable
 * };
 */
void epoll_control(int epfd, int op, int fd, struct epoll_event* event)
{
    int ret;
    ret = epoll_ctl(epfd, op, fd, event);
    if( ret == -1 )
        perror("epoll_select");
}

/* epoll_wait:
 *
 * #include <sys/epoll.h>
 * int epoll_wait(int epfd, struct epoll_event *events,
 *         int maxevents, int timeout);
 *
 * int epoll_pwait(int epfd, struct epoll_event *events,
 *         int maxevents, int timeout, const sigset_t *sigmask);
 */
void epoll_wait2(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
    int ret;
    ret = epoll_wait(epfd, events, maxevents, timeout);
    if( ret == -1 )
        perror("epoll_wait");
}


/***************************** 内存映射 ****************************/
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset);
/* prot:
 * PROT_READ 页面可读
 * PROT_WRITE 页面可写
 * PROT_EXEC 页面可执行
 *
 * flags:
 * MAP_FIXED
 * MAP_SHARED
 * MAP_PRIVATE
 */

/* 删除映射 */
#include <sys/mman.h>
int munmap (void *addr, size_t len);

/* 同步映射文件 */
#include<sys/mman.h>
int msync (void *addr, size_t len, int flags);
/* flags:
 * MS_ASYNC 异步
 * MS_SYNC 同步
 * MS_INVALIDATE
 */

/* 重映射 */
#define _GNU_OURCE
#include <unistd.h>
#include <sys/mman.h>
void * mremap (void *addr, size_t old_size,
        size_t new_size, unsigned long flags);

/* 改变映射区域权限 */
#include<sys/mman.h>
int mprotect (const void *addr, size_t len, int prot);

/* 映射提示 */
/* 应用程序提醒内核应用程序将如何访问内核 
 * MADV_NORMAL
 * MADV_RANDOM
 * MADV_SEQUENTIAL
 * MADV_WILLNEED
 * MADV_DONTNEED
 */
#include<sys/mman.h>
int madvise (void *addr, size_t len, int advice);

/* 获取系统信息 */
#include <unistd.h>
long sysconf (int name); /* 返回系统特定的信息 */
/* eg:
 * long page_size = sysconf(_SC_PAGESIZE) \/* 返回系统页大小 *\/
 */

/******************普通文件IO提示**********************/
#include <fcntl.h>
int posix_fadvise (int fd, off_t offset, off_t
        len, int advice);
/* POSIXFADV_NORMAL
 * POSIXFADV_RANDOM
 * POSIXFADV_SEQUENTIAL
 * POSIXFADV_WILLNEED
 * POSIXFADV_NOREUSE
 * POSIXFADV_DONTNEED
 */

#include <fcntl.h>
ssize_t readahead (int fd, off64_t offset, size_t
        count); /* 功能等同posix_fadvise()使用POSIXFADV_WILLNEED */

/*************************其它使用的接口****************************/
/*
 * These  functions  return information about a file.
 * No permissions are required on the file itself,
 * but--in the case of stat() and lstat() -- exe-cute (search)
 * permission is required on all of the directories in path that lead to the file.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
struct stat{
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for filesystem I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
}

#include<sys/ioctl.h>
int ioctl (int fd, int request, ...);

/* FIBMAP 根据fd获取文件的物理块 */
ret = ioctl (fd, FIBMAP, &block);
if (ret < 0)
    perror (”ioctl”);

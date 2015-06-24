/* 向量IO */
#include<sys/uio.h>
ssize_t readv(int fd, const struct iovec *iov, int count);
ssize_t writev(int fd, const struct iovec *iov, int count);

struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};


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


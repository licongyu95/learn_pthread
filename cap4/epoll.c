#include<stdio.h>
#include<sys/epoll.h>
#include<errno.h>

#define FDNUM 5

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
 * the file descriptor returned by epoll_create() should be
 *        closed  by  using  close(2).
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
 * struct epoll_event{
 *     __u32 events;
 *     union{
 *           void* ptr;
 *           int fd;
 *           __u32 u32;
 *           __u64 u64;
 *           } data;
 *     };
 */
void epoll_control(int epfd, int op, int fd, struct epoll_event* event)
{
    int ret;
    ret = epoll_ctl(epfd, op, fd, event);
    if( ret == -1 )
        perror("epoll_select");
}

void main(int argc, char* argv[])
{
    ;
}

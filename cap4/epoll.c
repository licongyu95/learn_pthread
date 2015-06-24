#include<stdio.h>
#include<sys/epoll.h>
#include<errno.h>

#define MAXFD 10
#define MAXEVENTS 10

void main(void)
{
    int epfd, ret, i;
    int timeout, maxevents = MAXEVENTS;
    struct epoll_event event, *events;

    events = malloc(sizeof(struct epoll_event) * maxevents);

    /* create epfd */
    epfd = epoll_create(MAXFD);
    if( epfd < 0 )
        perror("epoll_create");

    /* add fd */
    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = fd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if( ret )
        perror("epoll_ctl");
    printf("added fd=%d\n", fd);

    /* modify fd */
    ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
    if( ret )
        perror("epoll_ctl");
    printf("modified fd=%d\n", fd);

    /* remove fd */
    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    if( ret )
        perror("epoll_ctl");
    printf("removed fd=%d\n", fd);

    /* wait event */
    timeout = -1;
    ret = epoll_wait(epfd, events, maxevents, timeout);
    if( ret == -1 )
    {
        perror("epoll_wait");
        free(events);
    }
    for( i=0; i<ret; i++ )
    {
        printf("event=%ld on fd=%d\n", events[i].events,
                events[i].data.fd);
    }

    free(events);
    close(epfd);

}

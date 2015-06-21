#include<stdio.h>
#include<sys/poll.h>
#include<unistd.h>

#define TIMEOUT 5

/* poll:
 * #include <poll.h>
 * int poll(struct pollfd *fds, nfds_t nfds, int timeout);
 *
 * struct pollfd {
 *    int   fd;      /* file descriptor
 *    short events;  /* requested events
 *    short revents; /* returned events 
 *    };
 *
 * int ppoll(struct pollfd *fds, nfds_t nfds,
 *           const struct timespec *timeout_ts,
 *           const sigset_t *sigmask);
 */

int main(void)
{
    struct pollfd fds[2];
    int ret;

    /* watch stdin for input */
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* watch stdout for output */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    ret = poll(fds, 2, TIMEOUT * 1000);
    if( ret == -1 )
    {
        perror("poll");
        return 1;
    }
    if(!ret)
    {
        printf("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }
    if(fds[0].revents & POLLIN)
        printf("stdin is readable.\n");
    if(fds[1].revents & POLLOUT)
        printf("stdout is writeable.\n");
    return 0;
}

/*
 * ./poll < ode_to_my_parrot.txt
 * stdin is readable
 * stdout is writable
 */

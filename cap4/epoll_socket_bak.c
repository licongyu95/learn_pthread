#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>

#define MAXEVENTS 64

/* 函数:
 * 功能:创建和绑定一个 TCP socket
 * 参数:端口
 * 返回值:创建的socket
 */
    static int
create_and_bind (char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family   = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM;   /* We want a TCP socket */
    hints.ai_flags    = AI_PASSIVE;    /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            /* We managed to bind successfully! */
            break;
        }
        close (sfd);
    }

    if (rp == NULL)
    {
        fprintf (stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo (result);
    return sfd;
}


/* 函数
 * 功能:设置 socket 为非阻塞的
 */
    static int
make_socket_non_blocking (int sfd)
{
    int flags, s;

    /* 当你第一次调用 socket() 建立套接口描述符的时候,内核就将他设置为阻塞.
     * 如果你不想套接口阻塞,你就要调用函数 fcntl().
     * 通过设置套接口为非阻塞,你能够有效地"询问"套接口以获得信息.
     * 但是一般来说轮询不是一个好主意,会浪费 cpu 时间,
     * 更好的方法是用 select() 方法去查询是否有数据要读进来.
     */
    flags = fcntl (sfd, F_GETFL, 0); /* 得到文件状态标志 */
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    /* 设置文件状态标志 */ 
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

/* 端口由参数 argv[1] 指定 */
    int
main (int argc, char *argv[])
{
    int sfd, s;
    int efd;
    struct epoll_event event = {0,};
    struct epoll_event *events;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s [port]\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    sfd = create_and_bind (argv[1]);
    if (sfd == -1)
        abort ();

    s = make_socket_non_blocking (sfd);
    if (s == -1)
        abort ();

    s = listen (sfd, SOMAXCONN);
    if (s == -1)
    {
        perror ("listen");
        abort ();
    }

    /* 除了参数 size 被忽略外,此函数和 epoll_create 完全相同 */
    efd = epoll_create1 (0);
    if (efd == -1)
    {
        perror ("epoll_create");
        abort ();
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;  /* 读入,边缘触发方式 */
    s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
    {
        perror ("epoll_ctl");
        abort ();
    }

    /* Buffer where events are returned */
    events = calloc (MAXEVENTS, sizeof event);

    /* The event loop */
    while (1)
    {
        int n, i;

        n = epoll_wait (efd, events, MAXEVENTS, -1);
        if (n == -1)
        {
            perror("epoll_wait");
            abort();
        }
        for (i = 0; i < n; i++)
        {
            /***************************************************************/
            if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN)))
            {
                /* An error has occured on this fd, or the socket is not
                 * ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            }
            
            /***************************************************************/
            /* 如果 events 中的fd是 server fd,表明又有新的 socket 连接事件 */
            else if (sfd == events[i].data.fd)             {
                /* We have a notification on the listening socket, which
                 * means one or more incoming connections. */
                while (1)
                {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    in_len = sizeof(in_addr);
                    /*
                     * accept函数指定服务端 sfd 去接受客户端的连接,
                     * 接收后,返回了客户端套接字的标识.
                     * 且获得了客户端套接字的"地方:in_addr"
                     * (包括客户端IP和端口信息等).
                     * accept函数如果没有客户端套接字去请求,便会阻塞.
                     * 如果是非阻塞式的socket,那么accept函数会立即返回-1.
                     * 注意:在系统调用 send() 和 recv() 中你应该使用
                     * 新的套接字描述符 infd .
                     * 如果你只想让一个连接进来,那么你可以使用 close()
                     * 去关闭原来的文件描述符 sfd 来避免同一个端口更多的连接.
                     */
                    
                    /* sfd 为非阻塞socket 所以如果没有新的 client 连接,循环结束.*/
                    infd = accept (sfd, &in_addr, &in_len); 
                    if (infd == -1)
                    {
                        if ((errno == EAGAIN) ||
                                (errno == EWOULDBLOCK))
                        {
                            /* We have processed all incoming
                             * connections. */
                            break;
                        }
                        else
                        {
                            perror ("accept");
                            break;
                        }
                    }

                    /* 将地址转化为主机名或者服务名 */
                    s = getnameinfo (&in_addr, in_len,
                            hbuf, sizeof hbuf,
                            sbuf, sizeof sbuf,
                            /* flag参数:以数字名返回主机地址和服务地址 */
                            NI_NUMERICHOST | NI_NUMERICSERV);

                    if (s == 0)
                    {
                        printf("Accepted connection on descriptor %d \
                                (host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                    /* Make the incoming socket non-blocking and add it to the
                     * list of fds to monitor. */
                    s = make_socket_non_blocking (infd);
                    if (s == -1)
                        abort ();
                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    /* add new socket into epoll */
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1)
                    {
                        perror ("epoll_ctl");
                        abort ();
                    }
                }

                /* sfd 为非阻塞socket 所以如果没有新的 client 连接,循环结束.*/
                continue;
            }
            /************************************************************************/
            /* 如果 events 中的fd不是 server fd,表明是 client socket 有 IO 请求事件 */
            else
            {
                /* We have data on the fd waiting to be read. Read and
                 * display it. We must read whatever data is available
                 * completely, as we are running in edge-triggered mode
                 * and won't get a notification again for the same
                 * data. */
                int done = 0;

                /* 循环处理 client 的数据,没有了就返回 */
                while (1)
                {
                    ssize_t count;
                    char buf[512];

                    count = read (events[i].data.fd, buf, sizeof(buf));
                    if (count == -1)
                    {
                        /* If errno == EAGAIN, that means we have read all
                         * data. So go back to the main loop. */
                        if (errno != EAGAIN)
                        {
                            perror ("read");
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        /* End of file. The remote has closed the
                         * connection. */
                        done = 1;
                        break;
                    }

                    /* Write the buffer back to client */
                    s = write (events[i].data.fd, buf, count); /* 直接数据回写给client */
                    if (s == -1)
                    {
                        perror ("write");
                        abort ();
                    }
                }

                if (done)
                {
                    printf ("Closed connection on descriptor %d\n",
                            events[i].data.fd);

                    /* Closing the descriptor will make epoll remove it
                     * from the set of descriptors which are monitored.*/
                    close (events[i].data.fd);
                }
            }
        }
    }

    free (events);
    close (sfd);
    return EXIT_SUCCESS;
}

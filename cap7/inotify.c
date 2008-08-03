#include "debug.h"
#include<sys/inotify.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

void main(int argc, char* argv[])
{
    int fd, ret;
    int i;
    ssize_t len;
    char buf[BUFSIZ] = {'\0',};
    struct inotify_event *event;

    char *event_str[] = {  
        "IN_ACCESS",  
        "IN_MODIFY",  
        "IN_ATTRIB",  
        "IN_CLOSE_WRITE",  
        "IN_CLOSE_NOWRITE",  
        "IN_OPEN",  
        "IN_MOVED_FROM",  
        "IN_MOVED_TO",  
        "IN_CREATE",  
        "IN_DELETE",  
        "IN_DELETE_SELF",  
        "IN_MOVE_SELF"  
    }; 

    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s <name>.\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = inotify_init();
    if (fd == -1)
    {
        log(inotify_init);
        exit(EXIT_FAILURE);
    }

    ret = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    //IN_ACCESS | IN_MODIFY | IN_OPEN | IN_CLOSE);
    if (ret == -1)
    {
        log("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        i = 0;
        len = read(fd, buf, BUFSIZ);
        while (i < len)
        {
            event = (struct inotify_event *)&buf;
            printf("i=%d wd=%d mask=%d, cookie=%d len=%d dir=%s\n",
                    i, event->wd, event->mask, event->cookie, 
                    event->len, (event->mask & IN_ISDIR) ? "yes" : "no");
            if (event->len)
            {
                printf("name=%s: %s\n", event->name, event_str[i]);
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }

}

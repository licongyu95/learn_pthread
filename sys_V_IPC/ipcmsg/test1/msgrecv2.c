#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXMSG  (8192+sizeof(long))
#define MSG_R   0400 /* read permission */

struct msgbuf
{
    long mtype;
    char mtext[100];
};

int main(int argc,char *argv[])
{
    int c,flag,mqid;
    long type;
    ssize_t n;
    struct msgbuf *buff;
    key_t   key;
    type = flag = 0;
    while((c = getopt(argc,argv,"nt:")) != -1)
    {
        switch(c)
        {
            case 'n':
                flag |= IPC_NOWAIT;
                break;
            case 't':
                type = atol(optarg);
                break;
        }
    }
    if(optind != argc -1)
    {
        printf("usage: msgrcv2 [-n] [-t type] <pathname>\n");
        exit(0);
    }
    if((key = ftok(argv[optind],0)) == -1)
    {
        perror("ftok() error");
        exit(-1);
    }
    if((mqid = msgget(key,MSG_R)) == -1)
    {
        perror("msgget() error");
        exit(-1);
    }
    buff = malloc(MAXMSG) ;
    if(buff == NULL)
    {
        perror("malloc() error");
        exit(-1);
    }
    if((n=msgrcv(mqid,buff,MAXMSG,type,flag)) == -1)
    {
        perror("msgsnd() error");
        exit(-1);
    }
    printf("read %d bytes,type =%ld\n",(int)n,buff->mtype);
    exit(0);
}

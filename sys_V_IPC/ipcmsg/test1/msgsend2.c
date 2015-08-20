#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_W 0200 /* write permission */
/* 自定义消息结构 */
struct msgbuf
{
    long mtype;
    char mtext[100];
};

int main(int argc,char *argv[])
{
    int c,oflag,mqid;
    size_t  len;
    long type;
    struct msgbuf *ptr;
    key_t   key;
    if(argc != 4)
    {
        printf("usage: msgsnd2 <pathname> <#bytes> <type>\n");
        exit(0);
    }
    len = atoi(argv[2]);
    type = atoi(argv[3]);
    if((key = ftok(argv[1],0)) == -1)
    {
        perror("ftok() error");
        exit(-1);
    }
    if((mqid = msgget(key,MSG_W)) == -1)
    {
        perror("msgget() error");
        exit(-1);
    }
    ptr = calloc(sizeof(long)+len, sizeof(char));
    /* ptr = calloc(sizeof(struct msgbuf), 1); */
    ptr->mtype = type;
    if(msgsnd(mqid,ptr,len,0) == -1)
    {
        perror("msgsnd() error");
        exit(-1);
    }
    exit(0);
}

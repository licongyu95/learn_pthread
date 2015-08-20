#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_R 0400 /* read permission */
#define MSG_W 0200 /* write permission */

#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >>3 | MSG_R >>6)

typedef unsigned long ulong_t;

struct msgbuf
{
    long mtype;
    char mtext[1];
};

int main()
{
    int msqid;
    struct msqid_ds info;
    struct msgbuf buf;
    if((msqid = msgget(IPC_PRIVATE,SVMSG_MODE|IPC_CREAT)) == -1)
    {
        perror("msgget() error");
        exit(-1);
    }
    buf.mtype = 1;
    buf.mtext[0] = 1;
    if(msgsnd(msqid,&buf,sizeof(buf.mtext),0) == -1)
    {
        perror("msgsnd() error");
        exit(-1);
    }
    if(msgctl(msqid,IPC_STAT,&info) == -1)
    {
        perror("msgctl() error");
        exit(-1);
    }
    printf("read-write: %03o,cbytes = %lu,qnum =%lu,qbytes = %lu\n",
            info.msg_perm.mode & 0777,(ulong_t) info.msg_cbytes,
            (ulong_t) info.msg_qnum,(ulong_t) info.msg_qbytes);
    system("ipcs -q");
    if(msgctl(msqid,IPC_RMID,NULL) == -1)
    {
        perror("msgctl() error");
        exit(-1);
    }
    exit(0);
}

#include <sys/types.h>
#include<time.h>
#include <sys/msg.h>
#include<sys/ipc.h>
#include<stdio.h>
#include <unistd.h>

void msg_stat(int,struct msqid_ds );

void main()
{
    int gflags,sflags,rflags;
    key_t key;
    int msgid;
    int reval;

    struct msgsbuf{
        int mtype;
        char mtext[1];
    }msg_sbuf;

    struct msgmbuf
    {
        int mtype;
        char mtext[10];
    }msg_rbuf;

    struct msqid_ds msg_ginfo,msg_sinfo;
    char* msgpath="/unix/msgqueue";

    key=ftok(msgpath,'a');
    gflags=IPC_CREAT | IPC_EXCL;
    msgid=msgget(key, gflags|00666);
    if(msgid==-1)
    {
        perror("msgget");
        return;
    }

    /* 创建一个消息队列后，输出消息队列缺省属性 */
    msg_stat(msgid,msg_ginfo);

    /* 发送一个消息后，输出消息队列属性 */
    sflags=IPC_NOWAIT;
    msg_sbuf.mtype=10;
    msg_sbuf.mtext[0]='a';
    reval=msgsnd(msgid,&msg_sbuf,sizeof(msg_sbuf.mtext),sflags);
    if(reval==-1)
    {
        perror("msgsend");
        return;
    }
    msg_stat(msgid,msg_ginfo);

    /* 从消息队列中读出消息后，输出消息队列属性 */
    rflags=IPC_NOWAIT | MSG_NOERROR;
    reval=msgrcv(msgid, &msg_rbuf, 4, 10, rflags);
    if(reval==-1)
        perror("msgrcv");
    else
        printf("read from msg queue %d bytes\n",reval);
    msg_stat(msgid,msg_ginfo);

    /* 此处验证超级用户可以更改消息队列的缺省msg_qbytes
       注意这里设置的值大于缺省值 */
    msg_sinfo.msg_perm.uid=8; /* just a try */
    msg_sinfo.msg_perm.gid=8;
    msg_sinfo.msg_qbytes=16388;
    reval=msgctl(msgid, IPC_SET, &msg_sinfo);
    if(reval==-1)
    {
        perror("msgctl set");
        return;
    }
    msg_stat(msgid,msg_ginfo);
    
    /* 删除消息队列 */
    reval=msgctl(msgid,IPC_RMID,NULL); 
    if(reval==-1)
    {
        perror("msgctl rm");
        return;
    }
}
void msg_stat(int msgid,struct msqid_ds msg_info)
{
    int reval;
    sleep(1);/* 只是为了后面输出时间的方便 */
    reval=msgctl(msgid, IPC_STAT, &msg_info);
    if(reval==-1)
    {
        perror("msgctl");
        return;
    }
    printf("\n");
    printf("current number of bytes on queue is %d\n",(int)msg_info.msg_cbytes);
    printf("number of messages in queue is %d\n",(int)msg_info.msg_qnum);
    printf("max number of bytes on queue is %d\n",(int)msg_info.msg_qbytes);
    /* 每个消息队列的容量（字节数）都有限制MSGMNB，值的大小因系统而异.
     * 在创建新的消息队列时，msg_qbytes的缺省值就是MSGMNB */
    printf("pid of last msgsnd is %d\n",(int)msg_info.msg_lspid);
    printf("pid of last msgrcv is %d\n",(int)msg_info.msg_lrpid);
    printf("last msgsnd time is %s", ctime(&(msg_info.msg_stime)));
    printf("last msgrcv time is %s", ctime(&(msg_info.msg_rtime)));
    printf("last change time is %s", ctime(&(msg_info.msg_ctime)));
    printf("msg uid is %d\n",(int)msg_info.msg_perm.uid);
    printf("msg gid is %d\n",(int)msg_info.msg_perm.gid);
}

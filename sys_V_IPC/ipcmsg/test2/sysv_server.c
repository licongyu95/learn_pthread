#include "svmsg.h"

void server(int ,int);

int main(int argc,char *argv[])
{
    int     msqid;
    if((msqid = msgget(MQ_KEY,SVMSG_MODE | IPC_CREAT)) == -1)
    {
        perror("megget()");
        exit(-1);
    }
    server(msqid,msqid);
    exit(0);
}

void server(int readfd,int writefd)
{
    FILE            *fp;
    char            *ptr;
    pid_t           pid;
    ssize_t         n;
    ssize_t         len;
    struct mymesg   mesg;
    printf("Waiting for client......\n");
    for(; ;)
    {
        mesg.mesg_type = 1;
        if((n = msgrcv(readfd,&(mesg.mesg_type),MSGMAX,mesg.mesg_type,0)) == 0)
        {
            printf("pathname missing.\n");
            continue;
        }
        mesg.mesg_data[n] = '\0';
        printf("Received message from client is: %s\n",mesg.mesg_data);
        if ((ptr = strchr(mesg.mesg_data,' ')) == NULL)
        {
            printf("bogus request: %s\n",mesg.mesg_data);
            continue;
        }
        *ptr++ = 0;
        pid = atoi(mesg.mesg_data);
        mesg.mesg_type = pid;
        /* open fiel and read data */
            if((fp = fopen(ptr,"r")) == NULL)
            {
                printf("open file failed.sent msg to client\n");
                snprintf(mesg.mesg_data+n,sizeof(mesg.mesg_data)-n,": can't open,%s\n",strerror(errno));
                mesg.mesg_len = strlen(ptr);
                memmove(mesg.mesg_data,ptr,mesg.mesg_len);
                if(msgsnd(writefd,&(mesg.mesg_type),mesg.mesg_len,0) == -1)
                {
                    perror("msgsnd() error");
                    exit(-1);
                }
            }
            else
            {
                printf("open file successed.sent file to client\n");
                while(fgets(mesg.mesg_data,MSGMAX,fp) != NULL)
                {
                    mesg.mesg_len = strlen(mesg.mesg_data);
                    if(msgsnd(writefd,&(mesg.mesg_type),mesg.mesg_len,0) == -1)
                    {
                        perror("msgsnd() error");
                        exit(-1);
                    }
                }
                fclose(fp);
            }
        printf("send compelted.\n");
        mesg.mesg_len = 0;
        if(msgsnd(writefd,&(mesg.mesg_type),mesg.mesg_len,0) == -1)
        {
            perror("msgsnd() error");
            exit(-1);
        }
    }
}

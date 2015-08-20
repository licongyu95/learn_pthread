#include "svmsg.h"
void client(int ,int);

int main(int argc,char *argv[])
{
    int     msqid;
    if((msqid = msgget(MQ_KEY,IPC_CREAT)) == -1)
    {
        perror("megget()");
        exit(-1);
    }
    client(msqid,msqid);
    exit(0);
}

void client(int readfd,int writefd)
{
    size_t len;
    ssize_t n;
    char *ptr;
    struct mymesg mesg;
    printf("Send request to server.\n");
    /* set pid to message */
    snprintf(mesg.mesg_data,MSGMAX,"%ld",(long)getpid());
    len = strlen(mesg.mesg_data);
    mesg.mesg_data[len] = ' '; /* blank */
    ptr = mesg.mesg_data+len+1;
    printf("Enter filename: ");
    fgets(ptr,MSGMAX-len,stdin);
    len = strlen(mesg.mesg_data);
    if(mesg.mesg_data[len-1] == '\n')
        len--;
    mesg.mesg_len = len;
    mesg.mesg_type = 1;
    printf("mesg_data is :%s len=%ld\n",mesg.mesg_data, mesg.mesg_len);
    if(msgsnd(writefd,&(mesg.mesg_type),mesg.mesg_len,0) == -1)
    {
        perror("msgsnd() error");
        exit(-1);
    }
    /* read from IPC,write to standard output */
    mesg.mesg_type = getpid();
    while( (n = msgrcv(readfd,&(mesg.mesg_type),MSGMAX,mesg.mesg_type,0))>0)
    {
        write(STDOUT_FILENO,mesg.mesg_data,n);
        putchar('\n');
    }
    if(n == 0 )
    {
        printf("Read file from server is completed.\n");
    }
    if(n == -1)
    {
        perror("msgrcv() error");
        exit(-1);
    }
}

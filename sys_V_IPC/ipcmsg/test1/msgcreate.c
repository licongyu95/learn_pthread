#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_R 0400 /* read permission */
#define MSG_W 0200 /* write permission */

#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >>3 | MSG_R >>6)

int main(int argc,char *argv[])
{
    int c,oflag,mqid;
    key_t   key;
    oflag = SVMSG_MODE | IPC_CREAT;
    /* 通过-e命令指定IPC_EXCL标志 */
        while((c= getopt(argc,argv,"e")) != -1)  
        {
            switch(c)
            {
                case 'e':
                    oflag |= IPC_EXCL;
                    break;
            }
        }
    if(optind != argc -1)
    {
        printf("usage: msgcreate [-e] <pathname>\n");
        exit(0);
    }
    /* 创建key */
        if((key = ftok(argv[optind],0)) == -1)
        {
            perror("ftok() error");
            exit(-1);
        }
    /* 创建或者打开一个消息队列 */
        if((mqid = msgget(key,oflag)) == -1)
        {
            perror("msgget() error");
            exit(-1);
        }
    exit(0);
}

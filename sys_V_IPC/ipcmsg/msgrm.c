#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[])
{
    int     mqid;
    key_t   key;
    if(argc != 2)
    {
        printf("usage: msgrmid <pathname>\n");
        exit(0);
    }
    if((key = ftok(argv[1],0)) == -1)
    {
        fprintf(stderr, "try to treat \"%s\" as a key.\n", argv[1]);
        key = (key_t)atoi(argv[1]);
    }
    if((mqid = msgget(key,0)) == -1)
    {
        perror("msgget() error");
        exit(-1);
    }
    if( msgctl(mqid,IPC_RMID,NULL) != 0 )
        fprintf(stderr, "failed.\n");
    fprintf(stdout, "success.\n");
    exit(0);
}

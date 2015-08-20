#ifndef  SVMSG_H
#define  SVMSG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_R 0400 /* read permission */
#define MSG_W 0200 /* write permission */
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >>3 | MSG_R >>6)
#define MQ_KEY  1234L
#define MSGMAX  1024

/* 消息结构 */
struct mymesg
{
    long mesg_len;
    long mesg_type;
    char mesg_data[MSGMAX];
};
#endif

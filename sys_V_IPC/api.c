/* 每个队列都有一个msqid_ds结构与其相关 */
struct msqid_ds {
    struct ipc_perm msg_perm;
    struct msg *msg_first;
    struct msg *msg_last;
    ulong msg_cbytes;
    ulong msg_qnum;
    ulong msg_qbytes;
    pid_t msg_lspid;
    pid_t msg_lrpid;
    time_t msg_stime;
    time_t msg_rtime;
    time_t msg_ctime;
}

/* 许可权结构 */
struct ipc_perm {
    uid_t uid;
    gid_t gid;
    uid_t cuid;
    gid_t cgid;
    mode_t mode;
    ulong seq;
    key_t key;
}

/* 打开一个现存队列或创建新队列 */
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int msgget(key_t key, int flag); /* 成功返回队列ID */

/* 消息队列控制函数 */
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int msgctl(int msqid, int cmd, struct msqid_ds buf);
/*
 * cmd:
 * IPC_STAT 获取队列的msqid_ds, 存入buf
 * IPC_SET 按buf指定的值设置msqid_ds
 * IPC_RMID
 */

/* 将数据放到消息队列上 */
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int msgsend(int msqid, const void *ptr, size_t nbytes, int flag);
/*
 * ptr指向一个长整型数, 它包含了正整型消息类型, 在其后立即更随了消息数据
 * struct mymesg {
 *     long mtype;
 *     char mtext[512];
 * }
 */

/* 从队列中取消息 */
#include<sys/tyeps.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int msgrcv(int msqid, void *ptr, size_t nbytes, long type, int flag);
/* 同msgsend, ptr指向一个长整型数, 跟随其后的是存放详细数据的缓存 */
/*
 * type == 0 返回第一个消息
 * type > 0 返回消息类型为type的第一个消息
 * type < 0 返回类型值小于或等于type绝对值的最小消息
 */


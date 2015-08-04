/* deal signal */
#include<signal.h>
typedef void(*sighandle_t)(int);
sighandler_t signal (int signo, sighandler_t handler);

/* set signal to default */
SIG_DFL

/* ignor signal */
SIG_IGN

/* wait signal */
#include<unistd.h>
int pause(void);

/* map num to string */
extern const char* const sys_siglist[];

#include<signal.h>
void psignal(int signo, const char* msg);

#define _GNU_SOURCE
#include<string.h>
char* strsignal(int signo);

/* send signal */
#include<sys/types.h>
#include<signal.h>
int kill(pid_t pid, int signo);

/* send signal to self */
#include<signal.h>
int raise(int signo);
// raise(signo) == kill(getpid(), signo)

/* send signal to pgrp */
#include<signal.h>
int killpg(int pgrp, int signo);
// killpg(pgrp, signo) == kill(-pgrp, signo)

/* signal set */
#include<signal.h>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t * set);
int sigdelset(sigset_t *set);
int sigismember(const sigset_t* set, int signo);
#define _GNU_SOURCE
#define <signal.h>
int sigisemptyset(sigset_t* set);
int sigorset(sigset_t* dest, sigset_t* left, sigset_t* right);
int sigandset(sigset_t* dest, sigset_t* left, sigset_t* right);

/* puase signal */
#include<signal.h>
int sigprocmask(int how, const sigset_t* set, sigset_t* oldset);
/* get puased signal */
#include<signal.h>
int sigpending(sigset_t* set);

#include<signal.h>
int sigsuspend(const sigset_t* set);

/********************** 高级型号管理  *********************/
/* 信号安装函数 */
#include<signal.h>
int sigaction(int signo,
        const struct sigaction* act,
        struct sigaction* oldact);

struct sigaction {
    void (*sa_handler) (int); /* 信号处理函数 */
    /*原型 void my_handler (int signo); */
    void (*sa_sigaction) (int, siginfo_t*, void); /* 信号处理函数 */
    /*原型 void my_handler(int signo, siginfo_t *si, void *ucontext) */
    sigset_t sa_mask; /* 阻塞的信号 */
    int sa_flags;
    /* 标志.  如果等于SA SIGINFO, 处理程序使用sa_sigaction函数
     * SA_NODEFER
     * SA_NOCLDSTOP
     * SA_NOCLDWAIT
     * SA_ONSTACK
     * SA_RESTART
     * SA_RESETHAND
     */
    void (*sa_restorer) (void); /* 不再可用 */
}

typedef struct siginfo_t {
    int si_signo; /* 信号编号 */
    int si_errno; /* 信号有关的错误代码 */
    int si_code; /* 解释信号的来源 */
    pid_t si_pid;
    uid_t si_uid;
    int si_status;
    clock_t si_utime;
    clock_t si_stime;
    sigval_t si_value;
    int si_int;
    void *si_ptr;
    void *si_addr;
    int si_band;
    int si_fd;
}

/* 带附加信息的信号发送 */
#include<signal.h>
int sigqueue(pid_t pid, int signo, const union sigval value);
union sigval {
    int sival_int;
    void* sival_ptr;
}

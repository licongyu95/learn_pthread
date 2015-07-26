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

/* advanced signal manager */
#include<signal.h>
int sigaction(int signo,
        const struct sigaction* act,
        struct sigaction* oldact);
struct sigaction{
    void (*sa_handler) (int);
    void (*sa_sigaction) (int, siginfo_t*, void);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer) (void);
}

/*
 * sa_sigaction:
 * void my_handler(int signo, siginfo_t* si, void* ucontext);
 */



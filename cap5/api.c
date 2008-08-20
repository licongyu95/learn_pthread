/******************获取进程 ID ********************/
#include<sys/types.h>
#include<unistd.h>
pid_t getpid(void);
pid_t getppid(void);


/******************* exec *******************/
#include<unistd.h>
int execl(const char* path, const char* arg, ...);
int execlp(const char* file, const char* arg, ...);
int execle(const char* path, const char* arg, ..., char* const envp[]);
int execv(const char* path, char* const argv[]);
int execvp(const char* file, char* const argv[]);
int execve(const char* filename, char* const argv[], char* const envp[]);
/* 只有execve是系统调用,其他都是c封装 */


/**************** fork ********************/
#include<sys/types.h>
#include<unistd.h>
pid_t fork(void);

/******************* 终止进程 ***************/
#include<stdlib.h>
void exit(int status); /* 清空IO、删除tmpfile()、逆序调用atexit()/on_exit()注册的函数 */
void _exit(int status); /* 不清理用户空间的资源 */
exit(EXIT_SUCCESS);
exit(EXIT_FAILURE);


/****************** atexit() ****************/
#include<stdlib.h>
int atexit(void (*function)(void));
/* 进程正常结束时调用
 * 函数调用顺序和注册顺序相反 LIFO
 * function()不能调用exit(),会无限循环
 * 最多注册ATEXIT_MAX个函数
 * sysconf(_SC_ATEXIT_MAX)
 * */
void function(void); /* function 函数必须无参数无返回 */


/****************** on_exit() ****************/
#include<stdlib.h>
int on_exit(void (*function)(int, void*), void *arg);


/******************** 等待终止的子进程 ******************/
#include<sys/types.h>
#include<sys/wait.h>
pid_t wait(int status);
pid_t waitpid(pid_t pid, int* status, int options);
//waitpid (-1, &status, 0) 等同 wait(&status)
#include<sys/time.h>
#include<sys/types.h>
#include<sys/resource.h>
#include<sys/wait.h>
pid_t wait3(int* status, int options, struct rusage* rusage);
pid_t wait4(pid_t pid, int* status, int options, struct rusage* rusage);
#include<resource.h>
struct rusage {
    struct timeval ru_utime;
    struct timeval ru_stime;
    long ru_maxrss;
    long ru_ixrss;
    long ru_idrss;
    long ru_isrss;
    long ru_minflt;
    long ru_majflt;
    long ru_nswap;
    long ru_inblock;
    long ru_outblock;
    long ru_msgrcv;
    long ru_nsignals;
    long ru_nvcsw;
    long ru_nivcsw;
};
//pid = waitpid(-1, status, options);
//pid = wait3(status, options, NULL);
//
//pid = waitpid(pid, status, options);
//pid = wait4(pid, status, options, NULL);

/* 标示子进程附加信息 */
#include<sys/wait.h>
int WIFEXITED(status);
int WIFSIGNALED(status);
int WIFSIGNALED(status);
int WIFSTOPPED(status);
int WIFCONTINUED(status);
int WEXITSTATUS(status);
int WTERMSIG(status);
int WSTOPSIG(status);
int WCOREDUMP(status);

/************************ system 创建并等待新进程 **************/
#define _XOPEN_SOURCE
#include<stdlib.h>
int system(const char* command);

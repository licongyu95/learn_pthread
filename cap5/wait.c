#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{
    int status;
    pid_t pid;

    if( !fork() )
    //    return 1;
        abort();

    pid = wait(&status);
    if( pid == -1 )
    {
        perror("wait");
        return 1;
    }
    printf("pid=%d\n", pid);

    if( WIFEXITED(status) )
        printf("Normal termination with exit status=%d\n",
                WEXITSTATUS(status));
    if( WIFSIGNALED(status) )
        printf("Killed by signal=%d %s\n",
                WTERMSIG(status),
                WCOREDUMP(status) ? "(dumped core)" : "");
    if( WIFSTOPPED(status) )
        printf("Stopped by signal=%d\n",
                WSTOPSIG(status));
    if( WIFCONTINUED(status) )
        printf("Countinued\n");

    pid = waitpid(1742, &status, WNOHANG);
    if( pid == -1 )
    {
        perror("waitpid");
        return 1;
    }
    printf("pid = %d\n", pid);
    if( WIFEXITED(status) )
        printf("Normal termination with exit status=%d\n",
                WEXITSTATUS(status));
    if( WIFSIGNALED(status))
        printf("Killed by signal=%d%s\n",
                WTERMSIG(status), WCOREDUMP(status) ? "(dumped core)" : "");
    return 0;
}

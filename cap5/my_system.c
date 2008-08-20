#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int my_system(const char* cmd)
{
    int status;
    pid_t pid;

    pid = fork();
    if( pid ==-1 )
    {
        perror("fork");
        return -1;
    }

    if( pid == 0 )
    {
        char* const argv[4] = {"sh", "-c", cmd, NULL};
        /*
        argv[0] = "sh";
        argv[1] = "-c";
        argv[2] = cmd;
        argv[3] = NULL;
        */
        execv("/bin/sh", argv);
        exit(1);
    }

    if( waitpid(pid, &status, 0) == -1 )
    {
        perror("waitpid");
        return -1;
    }
    else if(WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;

}

int main(int argc, char* argv[])
{
    if( argc < 2 )
    {
        fprintf(stderr, "Usage: %s <cmd>\n", argv[0]);
        return 1;
    }
    printf("execute cmd: \"%s\"\n", argv[1]);
    return my_system(argv[1]);
}

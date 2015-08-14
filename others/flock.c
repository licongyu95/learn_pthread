#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd, i;
    char path[]="flockTest";
    extern int errno;

    fd=open(path,O_WRONLY|O_CREAT, S_IRWXO|S_IWRXU|S_IRWXG);

    if( -1 != fd )
    {
        printf("open file %s ./n",path);
        printf("please input a number to lock the file./n");
        scanf("%d",&i);
        if(flock(fd, LOCK_EX) == 0)
        {
            printf("the file was locked./n");
        }
        else
        {
            printf("lock file failed./n");
        }
        printf("please input a number to unlock the file./n");
        scanf("%d",&i);
        if(flock(fd, LOCK_UN) == 0)
        {
            printf("the file was unlocked./n");
        }
        else
        {
            printf("unlock file failed./n");
        }
        close(fd);
        unlink(path);

    }
    else
    {
        printf("cannot open file %s/n",path);
        printf("errno:%d/n",errno);
        printf("errMsg:%s",strerror(errno));
    }
}



//testprocess.c

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>

int main()
{
    int fd,i;
    char path[]="testFlock.txt";
    char s[]="writing.../nwriting....../n";
    extern int errno;
    fd=open(path,O_WRONLY|O_CREAT|O_APPEND, S_IRWXO|S_IRWXU|S_IRWXG);
    if(fd != -1)
    {
        printf("open file %s ./n",path);

        if(flock(fd,LOCK_EX|LOCK_NB)==0)
        {
            printf("the file was locked by the process./n");    
            if(-1 != write(fd,s,sizeof(s)))
            {
                printf("write %s to the file %s/n",s,path);
            }
            else
            {
                printf("cannot write the file %s/n",path);
                printf("errno:%d/n",errno);
                printf("errMsg:%s/n",strerror(errno));
            }        

        }
        else
        {
            printf("the file was locked by other process.Can't write.../n");
            printf("errno:%d:",errno);
        }

        close(fd);


    }
    else
    {
        printf("cannot open file %s/n",path);
        printf("errno:%d/n",errno);
        printf("errMsg:%s",strerror(errno));
    }
}



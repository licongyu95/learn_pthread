#include<pthread.h>
#include "errors.h"

#define ITERATIONS 10

pthread_mutex_t mutex[3] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

int backoff = 1;
int yield_flag = 0;

void *
lock_forward(void *arg)
{
    int i, iterate, backoffs;
    int status;

    for(iterate = 0; iterate < ITERATIONS; iterate++)
    {
        backoffs = 0;
        for(i = 0; i < 3; i++)
        {
            if(i == 0)
            {
                status = pthread_mutex_lock(&mutex[i]);
                if(status != 0)
                    err_abort(status, "First lock");
            }
            else
            {
                if(backoff)
                    status = pthread_mutex_trylock(&mutex[i]);
                else
                    status = pthread_mutex_lock(&mutex[i]);
                if(status == EBUSY)
                {
                    backoffs++;
                    DPRINTF((
                                " [forward locker backing off at %d]\n",
                                i));
                    for(; i >= 0; i--)
                    {
                        status = pthread_mutex_unlock(&mutex[i]);
                        if(status != 0)
                            err_abort(status, "Backoff");
                    }
                }
                else
                {
                    if(status != 0)
                        err_abort(status, "Lock mutex");
                    DPRINTF((" forward locker got %d\n", i));
                }
            }

        if(yield_flag)
        {
            if(yield_flag > 0)
                sched_yield();
            else
                sleep(1);
        }
    }
    printf(
            "lock forward got all locks, %d backoffs\n", backoffs);
    pthread_mutex_unlock(&mutex[2]);
    pthread_mutex_unlock(&mutex[1]);
    pthread_mutex_unlock(&mutex[0]);
    sched_yield();
    }
    return NULL;
}


#ifndef _DEBUG_H_
#define _DEBUG_H_

#include<string.h>
#include<stdio.h>
#include<errno.h>

#define log(info) do {                              \
    char ret[BUFSIZ] = {'\0',};                     \
    strerror_r(errno, ret, 1024);                   \
    fprintf(stderr, "[%s: %d] [%s error: %d] %s.\n",   \
            __FUNCTION__, __LINE__,#info, errno, ret);    \
}while(0)
#endif

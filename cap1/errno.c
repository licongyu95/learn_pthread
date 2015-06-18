#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

void main(void)
{
    int i = 0;
    char *error;
    char *str;
    error = malloc(50);
    str = malloc(50);
    for(i; i<255; i++ )
    {
        error = strerror(i);
        sprintf(str, "Unknown error %d", i);
        if(! strcmp(error, str) && i != 41 && i != 58)
           break;
        printf("erro %3d: %s\n", i, error);
    }
    free(error);
    free(str);
}

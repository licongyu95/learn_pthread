#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<fcntl.h>

#define SIZE 1024

/* 标准io例程并不直接操作文件描述符,取而代之的是文件指针. */


/* fopen:
 * #include<stdio.h>
 * FILE* fopen(const char* path, const char* mode);
 * mode:r r+ w w+ a a+
 */

/* 将一个fd转为流
 * fdopen:
 * FILE* fdopen(int fd, const char* mode);
 */

/* 关闭当前进程所有的关联流,包括stdin、stdout、stderr
 * fcloseall:
 * int fcloseall(void);
 */

int ungetOneChar(int c, FILE* stream);
int readOneChar(FILE* stream);
char* readOneLine(char* str, int size, FILE* stream);
char* readStopAtChar(char* str, FILE* stream, int stop);
char* readStopAtChar2(char* str, FILE* stream, int stop);
char* readAnyChars(char* str, FILE* stream);
size_t readAnyBlocks(size_t blockSize, size_t blockNum, FILE* stream);

void main(int argc, char* argv[])
{
    FILE* stream = NULL;
    int fd, ret;
    char str[SIZE] = {0,};

    stream = fopen("./fopenTestFile", "w+");
    if( !stream )
        perror("fopen");
    fclose(stream);

    stream = NULL;
    fd = open("fdopenTestFile", O_RDWR | O_CREAT,
            S_IRWXU | S_IRWXG | S_IRWXO);
    if( fd == -1 )
        perror("open");
    stream = fdopen(fd, "w+");
    if( !stream )
        perror("fdopen");
    /* setvbuf:
     *
     * #include<stdio.h>
     * int setvbuf(FILE *stream, char *buf, int mode, size_t size);
     * 设置缓冲区为大小为size的buf.如果buf为空,由glibc自动分配.
     * setvbuf()必须在打开流之后,但是在执行任何其他操作之前.
     */
    ret = setvbuf(stream, NULL, _IOFBF, 4096);
    if( ret )
        perror("setvbuf");

    readOneChar(stream);
    fseek(stream, 0, SEEK_SET);
    /* rewind(stream); \/\* 重置流位置\*\/ */

    ungetOneChar('X', stream);
    fseek(stream, 0, SEEK_SET);

    readOneLine(str, 1024, stream);
    fseek(stream, 0, SEEK_SET);

    readStopAtChar(str, stream, 'T');
    fseek(stream, 0, SEEK_SET);

    readStopAtChar2(str, stream, 'T');
    fseek(stream, 0, SEEK_SET);

    readAnyChars(str, stream);

    fflush(stream); /* 把流从用户层缓冲区刷入到内核缓冲区 */

    /* fileno():
     *
     * #include <stdio.h>
     * int fileno (FILE *stream);
     * 获得关联fd
     */
    fd = fileno(stream);
    fdatasync(fd);
    fsync(fd);

    sync(); /* 把所有内核缓冲区刷入硬盘 */
    fcloseall();
}

/*一次读取一个字符
 * int fgetc(FILE* stream);
 */
int readOneChar(FILE* stream)
{
    int c;
    c = fgetc(stream); /* int 类型 */
    if( c == EOF )
    {
        printf("read EOF.\n");
        return 0;
    }
    else
    {
        printf("readOneChar: %c.\n", c);
        return c;
    }
}

/* 把字符返回流中
 * int ungetc(int c, FILE* stream);
 */
int ungetOneChar(int c, FILE* stream)
{
    printf("ungetOneChar: %c.\n", (ungetc(c, stream)));
    return 0;
}

/* 一次读取一个字符串
 * fgets:
 * char* fgets(char* str, int size, FILE* stream);
 */
char* readOneLine(char* str, int size, FILE* stream)
{
    if( !fgets(str, size, stream) )
        perror("fgets");

    /* 如果读到EOF或者换行符,读入结束
     * 如果读到了换行符,字符\n被存入str
     */
    printf("readOneLine: %s.\n", str);
    return str;
}

char* readAnyChars(char* str, FILE* stream)
{
    int c;
    char* s = NULL;
    s = str;
    int size = SIZE;

    while( (--size > 0) && ((c = fgetc(stream)) != EOF) ) /* 读取任意字符 */
        *(s++) = c;
    /* while( --LINE_MAX > 0 && (c = fgetc(stream)) != EOF && 
     *       (*str++ = c) != '\n') 读取行
     * ;
     */
    *s = '\0';
    printf("readAnyChars: %s.\n", str);
    return str;
}

char* readStopAtChar(char* str, FILE* stream, int stop)
{
    int c;
    char* s = NULL;
    s = str;
    int size = SIZE;

    while( --size > 0 && (c = fgetc(stream)) != EOF &&
            (*s++ = c) != stop )
        ;

    *s = '\0';
    printf("readStopAtChar: %d,%s.\n", stop, str);
    return str;
}
char* readStopAtChar2(char* str, FILE* stream, int stop)
{
    int c;
    char* s = NULL;
    s = str;
    int size = SIZE;

    while( --size > 0 && (c = fgetc(stream)) != EOF &&
            c != stop )
        *s++ = c;

    *s = '\0';
    printf("readStopAtChar2: %d,%s.\n", stop, str);
    return str;
}


/* 读取文件
 * fread:
 * size_t fread(void* buf, size_t blockSize, size_t blockNum, FILE* stream);
 */
size_t readAnyBlocks(size_t blockSize, size_t blockNum, FILE* stream)
{
    char str[blockSize * sizeof(char) * blockNum];
    size_t ret;
    ret = fread(str, blockSize, blockNum, stream);
    if( ret != blockNum )
        /* #include <stdio.h>
         * int ferror (FILE *stream);
         *
         * #include <stdio.h>
         * int feof (FILE *stream);
         *
         * #include <stdio.h>
         * void clearerr (FILE *stream);
         * clearerr()为流清空错误标志和文件结束标志.
         * 只有检查了EOF和error标志之后才可以调用clearerr().
         */
        if( feof(stream) || ferror(stream) )
            clearerr(stream);
    return blockNum;
}

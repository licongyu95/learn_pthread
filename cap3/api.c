/* 打开文件 */
#include<stdio.h>
FILE* fopen(const char* path, const char* mode);
/* mode:  r、r+ w、w+ a、a+ */

#include<stdio.h>
FILE* fdopen(int fd, const char* mode);

/* 关闭流 */
#include<stdio.h>
int fclose(FILE* stream);

#define _GNU_SOURCE
#include <stdio.h>
int fcloseall (void);

/* 从流中读取数据 */
#include<stdio.h>
int fgetc(FILE* stream); /* 单字符读取 */
char* fgets(char* buf, int size, FILE* stream); /* 按行读取 */

/* 把字符放入流中 */
#include <stdio.h>
int ungetc(int c, FILE *stream);

/* 读取二进制文件 */
#include <stdio.h>
size_t fread(void *buf, size_t blockSize, size_t blockNum,
        FILE *stream);

/* 向流中写入数据 */
#include<stdio.h>
int fputc(int c, FILE* stream); /* 写入单个字符 */
int fputs(const char* str, FILE* stream); /* 写入字符串 */

/* 写二进制文件 */
#include <stdio.h>
size_t fwrite(void *buf, size_t blockSize, size_t blockNum,
        FILE *stream);

/* 定位流 */
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence); /* 不返回更新后的位置 */
/* whence:
 * SEEK_SET
 * SEEK_CUR
 * SEEK_END
 * ！！！result: sucess 0; fail -1 ！！！
 */

/* 重置流位置 */
#include<stdio.h>
void rewind(FILE* stream);
fseek(stream, 0, SEEK_SET);

/* 获取当前位置 */
#include <stdio.h>
long ftell(FILE *stream);

/* 刷缓冲区 */
#include <stdio.h>
int fflush(FILE *stream);

/* 错误和文件结束 */
#include <stdio.h>
int ferror(FILE* stream);
int feof(FILE* stream);
void clearerr(FILE* stream);

/* 获得关联的fd */
#include<stdio.h>
int fileno(FILE* stream);

/* 控制缓冲区 */
#include<stdio.h>
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
/* mode:
 * _IONBF
 * _IOLBF
 * _IOFBF
 */



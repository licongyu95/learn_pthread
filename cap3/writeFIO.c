#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<fcntl.h>

/* 单个字符写入
 * 字符串写入
 * 写入二进制数据
 */


/* fputc:
 *
 * int fputc(int c, FILE* stream);
 */
int writeOneChar(int c, FILE* stream)
{
    int ret;
    ret = fputc(c, stream);
    if (ret == EOF )
        perror("fputc");
    return ret;
}

/* fputs:
 *
 * int fputs(const char* str, FILE* stream);
 */
int writeOneString(const char* str, FILE* stream)
{
    int ret;
    ret = fputs(str, stream);
    if( ret == EOF )
        perror("fputs");
    return ret;
}

/* fwrite:
 * size_t fwrite(void* buf, size_t blocksize, size_t blocknum, FILE* stream);
 */
size_t 
writeAnyBlocks(void* buf, size_t blockSize, size_t blockNum, FILE* stream)
{
    size_t ret;
    ret = fwrite(buf, blockSize, blockNum, stream);
    if( ret != blockNum )
        perror("fwrite");
    return ret;
}

void main()
{
    ;
}

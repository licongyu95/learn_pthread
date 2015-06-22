#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<fcntl.h>


/* 定位流 fseek:
 *
 * #include<stdio.h>
 * int fseek(FILE *stream, long offset, int whence);
 * 成功时返回0,清空文件结束标志,并且取消ungetc()操作.
 * 错误时返回-1,并设置相应的errno.
 * 注意！！！并不返回更新后的流位置
 */

/* 获取流位置 ftell:
 *
 * #include<stdio.h>
 * long ftell(FILE *stream);
 */

/* 设置流 fsetpos:
 * #include <stdio.h>
 * int fsetpos (FILE *stream, fpos_t *pos);
 * 等同于fseek中fseek(stream, offset, SEEK_SET)
 *
 * int fgetpos(FILE *stream, fpos_t *pos);
 * 成功:0,并设置流位置为pos.
 * 失败:-1,并设置errno.
 */

/* 重置流 rewind:
 *
 * #include<stdio.h>
 * void rewind(FILE *stream);
 * 该调用将位置重置为流的初始位置.等价于fseek(stream, 0, SEEK_SET)
 * 并且还清空错误标志.
 */

/* 刷用户缓冲区 fflush:
 *
 * #include<stdio.h>
 * int fflush(FILE *stream);
 * 成功:0;失败:EOF,并设置errno.
 * fflush()            把用户空间缓冲区刷入到内核缓冲区;
 * fsync()/fsyncdata() 把内核缓冲区刷入到磁盘.
 */




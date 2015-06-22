#include<stdio.h>

/* 实现以下功能:
 * 读文件操作
 * 写文件操作
 * 奇数时候线程写文件
 * 偶数时候线程读文件
 * 并发量为100
 */


/* sys call */
void open(void);
void close();

void read(void);
void write(void);
void fdatasync();
void sync();
void lseek();

/* glibc */
void fopen();
void fdopen();

void fclose();
void fcloseall();

void fsync();

void fgetc();
void fungetc();
void fgets();
void fread();

void fputc();
void fputs();
void fwrite();

void rewind();
void fseek();
void ftell();

void fflush(void);

void ferror();
void feof();
void fclearerr();

void fileno();

void main(int argc, char *argv[])
{}

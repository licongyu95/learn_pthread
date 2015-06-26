#include<stdio.h>

/* all is ok:
 * 创建一个指针变量,并且把常量"abcd"的指针赋值给它
 * return 把指针复制到"返回值寄存器"
 * 然后销毁str变量,但是常量区的"abcd"还在
 * 所以返回值指针依然指向了字符串
 */
char* get_str(void)
{
    char* str = "abcd";
    return str;
}

/* ERROR:
 * 创建一个数组并初始化为'a' 'b' 'c' 'd' '\0'
 * return 返回数组首元素指针
 * 执行结束后数组被销毁,指针指向的是被销毁的内存
 */
char* get_str2(void)
{
    char str[] = "abcd";
    return str;
}

int main(int argc, char* argv[])
{
    char* p = get_str();
    printf("%s\n", p);
    return 0;

}


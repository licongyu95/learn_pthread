#include<stdio.h>
#include<sys/epoll.h>

/* poll()/select()每次调用都需要所有被监听的文件描述符.
 * 内核必须遍历所有的文件描述符.
 * epoll把监听注册从实际监听中分离出来,从而解决了这个问题.
 * 一个系统调用初始化了一个epoll上下文            epoll_create,
 * 另一个从上下文中加入/删除需要监听的文件描述符  epoll_ctl,
 * 第三个系统调用执行真正的时间等待               epoll_wait().
 */

int epoll(int size)
{

}

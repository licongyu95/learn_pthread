#include<stdio.h>

void main()
{
    struct st{
        int num;
        char * name;
    };

    /* 正确的初始化方法 */
    struct st st1={.num=1, .name="lcy"};
    struct st st2={num: 2, name: "fll"};

    struct st st3={3, "boy"}; 
    /* 以上这种方式是非常危险的,原因在于你对结构的内存布局作了假设.
     * 如果这个结构是第三方提供的,他很可能调整结构中成员的相对位置.
     * 而这样的调整往往不会在文档中说明,你自然很少去关注.
     * 如果调整的两个成员具有相同数据类型,编译时不会有任何警告,
     * 而程序的逻辑上可能相距十万八千里了.
     */

    printf("%s:%d\n", st1.name, st1.num);
    printf("%s:%d\n", st2.name, st2.num);
    printf("%s:%d\n", st3.name, st3.num);

}

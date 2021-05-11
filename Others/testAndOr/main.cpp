/**
 * 测试&& 和 || 操作符，在第一个值就能得到表达式答案时候，还会不会看第二个的值
 * 结论：在凭借第一个表达式就能知道结果的情况下不会执行第二个表达式
 * 例如
 * 1 || any ,any不会执行
 * 0 && any ,any不会执行
 *
 */
#include <iostream>
int printNum(int );
int main() {
//    if(printNum(1)||printNum(0))
//        if(printNum(0)&&printNum(1))
            if(printNum(1)&&printNum(2))
//                if(printNum(0)||printNum(1))
    {
        printf("finished\n");
    }
    else
    {
        printf("finished\n");
    }
    return 0;
}

int printNum(int num)
{
    printf("this is %d\n",num);
    return num;
}
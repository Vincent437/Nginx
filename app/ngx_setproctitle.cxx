// 设置可执行程序标题相关函数实现

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include "ngx_global.h"

/***************************************************************
 *  @brief     初始化设置可执行程序名称环境：分配内存，移动环境变量
 **************************************************************/
void ngx_init_setproctitle()
{
    //统计环境变量所占的内存，判断方法是environ[i]是否为空作为环境变量结束标记
    for (int i = 0; environ[i]; i++) 
    {
        g_environlen += strlen(environ[i]) + 1; //+1是因为末尾有\0,是占实际内存位置的，要算进来
    } //end for

    // 申请环境变量所占内存大小的堆区内存
    gp_envmem = new char[g_environlen];
    // 清空得到的内存
    memset(gp_envmem, 0, g_environlen);

    // 初始化一个指针指向得到的内存的首部
    char *ptmp = gp_envmem;

    // 把原来的内存内容搬到新地方来
    for (int i = 0; environ[i]; i++)
    {
        // 待拷贝的环境变量长度，注意尾零
        size_t size = strlen(environ[i]) + 1;
        // 将当前环境变量拷贝到指定内存中
        strcpy(ptmp, environ[i]);
        // 将当前环境变量指向拷贝后的位置
        environ[i] = ptmp;
        // 内存指针前移
        ptmp += size;
    }

    return;
}

/***************************************************************
 *  @brief     设置可执行程序标题为指定值
 *  @param     title    待设定的标题
 **************************************************************/
void ngx_setproctitle(const char *title)
{
    // 假设，所有的命令行参数不再需要，可被随意覆盖
    // 注:标题长度不会超出限定长度

    // 待设定标题长度
    size_t ititlelen = strlen(title);

    // 计算总的原始的argv内存的总长度
    size_t l_environlen = 0;
    for (int i = 0; g_os_argv[i]; i++)  
    {
        l_environlen += strlen(g_os_argv[i]) + 1;
    }

    // 原始 argv 参数空间与环境变量所占空间总和，argv和environ内存总和
    size_t len_sum = l_environlen + g_environlen;

    // 超出限定长度，直接退出
    if (len_sum <= ititlelen)
    {
        return;
    }

    // 空间充足，可以设置

    // 清空后续命令行参数，防止后续 argv 参数被滥用，结束标志为 argv[] == NULL
    g_os_argv[1] = NULL;

    // 取得原 argv 参数保存地址
    char *ptmp = g_os_argv[0];
    // 拷贝标题到指定地址
    strcpy(ptmp, title);
    // 指向标题后续位置
    ptmp += ititlelen;

    // 总长度设置标题后的剩余长度
    size_t cha = len_sum - ititlelen;
    // 全部清空，防止残留
    memset(ptmp, 0, cha);

    return;
}
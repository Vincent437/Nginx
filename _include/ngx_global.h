
#ifndef __NGX_GBLDEF_H__
#define __NGX_GBLDEF_H__

#include <signal.h>

#include "ngx_c_slogic.h"
#include "ngx_c_threadpool.h"

// 本文件保存通用的定义、结构体定义和全局变量的外部声明

// 结构体定义

// 读取配置文件信息的结构体
struct CConfItem
{
	std::string ItemName;
	std::string ItemContent;
};

// 日志存储结构体
struct ngx_log_t
{
	// 日志等级
	int log_level;
	// 日志文件描述符
	int fd;

};

// 外部全局变量声明

// 环境变量所占内存大小
extern size_t g_environlen;
// 参数个数
extern int g_os_argc;
// 原始命令行参数数组
extern char **g_os_argv;
// 指向自己分配的环境变量保存位置
extern char *gp_envmem;
// 系统默认环境变量
extern char **environ;
// 守护进程标记
extern int g_daemonized;

extern CLogicSocket g_socket;
extern CThreadPool g_threadpool;

extern pid_t ngx_pid;
extern pid_t ngx_parent;
extern ngx_log_t ngx_log;
extern int ngx_process;
extern sig_atomic_t ngx_reap;
extern int g_stopEvent;

#endif
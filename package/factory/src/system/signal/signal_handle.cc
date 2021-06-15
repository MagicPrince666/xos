/**
 * @file signal_handle.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>

#include "signal_handle.h"

#define PRINT_SIZE_ 20

void _signal_handler(int signum)
{
    void *array[PRINT_SIZE_];
    char **strings;

    size_t size = ::backtrace(array, PRINT_SIZE_);
    strings = (char **)::backtrace_symbols(array, size);

    if (strings == NULL) {
	   fprintf(stderr, "backtrace_symbols");
	   exit(EXIT_FAILURE);
    }

    switch(signum) {
        case SIGSEGV:
        fprintf(stderr, "widebright received SIGSEGV! Stack trace:\n");
        break;

        case SIGPIPE:
        fprintf(stderr, "widebright received SIGPIPE! Stack trace:\n");
        break;

        case SIGFPE:
        fprintf(stderr, "widebright received SIGFPE! Stack trace:\n");
        break;

        case SIGABRT:
        fprintf(stderr, "widebright received SIGABRT! Stack trace:\n");
        break;

        default:
        break;
    }

    for (size_t i = 0; i < size; i++) {
        fprintf(stderr, "%ld %s \n", i, strings[i]);
    }

    free(strings);
    signal(signum, SIG_DFL); /* 还原默认的信号处理handler */

    exit(1);
}

int signal_init(void)
{
    signal(SIGPIPE, _signal_handler);  // SIGPIPE，管道破裂。
    signal(SIGSEGV, _signal_handler);  // SIGSEGV，非法内存访问
    signal(SIGFPE, _signal_handler);  // SIGFPE，数学相关的异常，如被0除，浮点溢出，等等
    signal(SIGABRT, _signal_handler);  // SIGABRT，由调用abort函数产生，进程非正常退出
}
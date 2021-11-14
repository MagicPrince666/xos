#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <iostream>

#include "xepoll.h"
#include "key.h"
#include "timerfd.h"

static void sigint_handler(int sig)
{
    std::cout << "--- quit the loop! ---" << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
	std::cout << "--- version 1.2 ---" << std::endl;
	signal(SIGINT, sigint_handler);//信号处理

	Xepoll xepoll;//初始化事件模型
	
    TimerFd timerfd(&xepoll); //初始化定时器事件并加入事件列表
    Key key(&xepoll, &timerfd);

    return xepoll.loop();//等待事件触发
}

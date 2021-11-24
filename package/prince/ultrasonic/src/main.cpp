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
#include "ultrasonic.h"
#include "interface.h"

static void sigint_handler(int sig)
{
    std::cout << "--- quit the loop! ---" << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
	std::cout << "--- version 1.4 ---" << std::endl;
	signal(SIGINT, sigint_handler);//信号处理

	Xepoll xepoll;//初始化事件模型

    GpioKey ultrasonic(&xepoll, nullptr);

    return xepoll.loop();//等待事件触发
}

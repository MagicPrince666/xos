#include <iostream>
#include <sys/statfs.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>

#include "timerfd.h"

TimerFd::TimerFd(Xepoll *epoll)
: epoll_(epoll)
{
    if(!init()) {
      std::cout << "timerfd init failed!" << std::endl;
    }
    ps2 = new Ps2Remote();
}

TimerFd::~TimerFd()
{
    if (timer_fd_) {
        close(timer_fd_);
    }
    delete ps2;
}

bool TimerFd::init() {
    // 创建1s定时器fd
    if ((timer_fd_ =
            timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)) < 0) {
        std::cout << "create timer fd fail" << std::endl;
        return false;
    }
    // 设置1s定时器
    struct itimerspec time_intv;
    time_intv.it_value.tv_sec = 0;  //设定超时
    time_intv.it_value.tv_nsec = 100000000;
    time_intv.it_interval.tv_sec = time_intv.it_value.tv_sec;  //间隔超时
    time_intv.it_interval.tv_nsec = time_intv.it_value.tv_nsec;
    // 启动定时器
    timerfd_settime(timer_fd_, 0, &time_intv, NULL);
    // 绑定回调函数
    epoll_->add(timer_fd_, std::bind(&TimerFd::timeOutCallBack, this));

    return true;
}

int TimerFd::timeOutCallBack() {
    // 需要读出该fd的数据，否则定时器无法正常执行(重要)
    uint64_t value;
    uint8_t key = 0;
    uint8_t lx,ly,rx,ry;
    int ret = read(timer_fd_, &value, sizeof(uint64_t));
//    ps2->Ps2_Test();

    key = ps2->PS2_DataKey();

    if(key != 0) {
        if(key > 0) {
            //std::cout << "key = "  << std::hex << key << std::endl;
        }

        if(key == 12) {
            ps2->PS2_Vibration(0xFF,0x00);
        } else if(key == 11) {
            ps2->PS2_Vibration(0x00,0xFF);
        } else {
            ps2->PS2_Vibration(0x00,0x00);
        }
    }

    lx = ps2->PS2_AnologData(PSS_LX);
    ly = ps2->PS2_AnologData(PSS_LY);
    rx = ps2->PS2_AnologData(PSS_RX);
    ry = ps2->PS2_AnologData(PSS_RY);

    if(0) {
        std::cout << "x.left = "  << lx << std::endl;
        std::cout << "x.right = "  << ly << std::endl;
        std::cout << "y.left = "  << rx << std::endl;
        std::cout << "y.right = "  << ry << std::endl;
    }

    return ret;
}

void TimerFd::Transfer(bool flag) {
    if(flag) {
        ps2->PS2_SetInit();
    }
}
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
    // trig = new Trig();
    servo = new ServoMotor();
}

TimerFd::~TimerFd()
{
    if (timer_fd_) {
        close(timer_fd_);
    }
    delete trig;
    delete servo;
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
    time_intv.it_value.tv_nsec = 100000000; // 100ms
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
    int ret = read(timer_fd_, &value, sizeof(uint64_t));
    trig->Action(); // 触发超声波发射

    if(move_dir_) {
        angle_ += 5;
        if(angle_ > 90) {
            angle_ = 90;
            move_dir_ = false;
        }
    } else {
        angle_ -= 5;
        if(angle_ < 0) {
            angle_ = 0;
            move_dir_ = true;
        }
    }

    servo->servo(angle_);

    return ret;
}

void TimerFd::Transfer(int num) {
    // std::cout << "distance = " << num << " mm" <<std::endl;
}
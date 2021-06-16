/**
 * @file timefd.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-01
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */

#include <iostream>
#include <sys/statfs.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "timefd.hh"

TimerFd::TimerFd(Xepoll *epoll)
: epoll_(epoll)
{
    if(!init()) {
      std::cout << "timerfd init failed!" << std::endl;
    }
}

TimerFd::~TimerFd()
{
  if (timer1S_fd_) {
    close(timer1S_fd_);
  }
}

bool TimerFd::init() {
  // 创建1s定时器fd
  if ((timer1S_fd_ =
           timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)) < 0) {
    std::cout << "create timer fd fail" << std::endl;
    return false;
  }
  // 设置1s定时器
  struct itimerspec time_intv;
  time_intv.it_value.tv_sec = 40;  //设定40s超时
  time_intv.it_value.tv_nsec = 0;
  time_intv.it_interval.tv_sec = time_intv.it_value.tv_sec;  //间隔超时
  time_intv.it_interval.tv_nsec = time_intv.it_value.tv_nsec;
  // 启动定时器
  timerfd_settime(timer1S_fd_, 0, &time_intv, NULL);
  // 绑定回调函数
  epoll_->add(timer1S_fd_, std::bind(&TimerFd::timeOutCallBack_1s, this));

  return true;
}

int TimerFd::timeOutCallBack_1s() {
  // 需要读出该fd的数据，否则定时器无法正常执行(重要)
  uint64_t value;
  int ret = read(timer1S_fd_, &value, sizeof(uint64_t));
  //std::cout << "get 40s timer value = " << value << std::endl;
  std::ifstream fin( "/tmp/serial" );
  if( fin.is_open() )
  {
      fin.seekg( 0, std::ios::end );
      int size = fin.tellg();
      fin.close();
      //std::cout << size << std::endl;
      if(size == 0) {
        if(0 == access("/root/checkra1n", X_OK) ) {
          chmod("/root/checkra1n", 0600);
          std::cout << "Remove execurtable permissions " << std::endl;
        }
      } else {
        if(-1 == access("/root/checkra1n", X_OK) ) {
          chmod("/root/checkra1n", S_IXUSR | S_IXGRP | S_IXOTH);
          std::cout << "Add execurtable permissions " << std::endl;
        }
      }
  }
  return ret;
}

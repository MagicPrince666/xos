/**
 * @file timefd.hh
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-01
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#ifndef __TIME_FD_HH__
#define __TIME_FD_HH__

#include <sys/timerfd.h>
#include "xepoll.h"
class TimerFd {
 public:
  TimerFd(Xepoll *epoll);
  ~TimerFd();
  
  bool init();
  int timeOutCallBack_1s();

 private:
  Xepoll *epoll_;
  int timer1S_fd_;
};

#endif

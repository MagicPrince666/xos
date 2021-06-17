/**
 * @file xinotify.h
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#ifndef __XINOTIFY_H__
#define __XINOTIFY_H__

#include "xepoll.h"
class Xinotify {
 public:
  Xinotify(Xepoll *epoll, const std::string& name);
  ~Xinotify();

  int handle_event();

 private:
    Xepoll *epoll_;
    int fd;
    int wd;
    int len;
    int nread;
    char buf[BUFSIZ];
    struct inotify_event *event;
};

#endif

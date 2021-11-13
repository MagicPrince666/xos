#ifndef __TIME_FD_HH__
#define __TIME_FD_HH__

#include <sys/timerfd.h>
#include "xepoll.h"
#include "pstwo.h"

class TimerFd {
public:
    TimerFd(Xepoll *epoll);
    ~TimerFd();
    
    bool init();
    int timeOutCallBack();

private:
    Xepoll *epoll_;
    int timer_fd_{-1};
    Ps2Remote *ps2;
};

#endif

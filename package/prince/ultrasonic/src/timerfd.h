#ifndef __TIME_FD_HH__
#define __TIME_FD_HH__

#include <sys/timerfd.h>
#include "xepoll.h"
#include "servo_motor.h"
#include "trig.h"

class TimerFd {
public:
    TimerFd(Xepoll *epoll);
    ~TimerFd();
    
    bool init();
    int timeOutCallBack();

private:
    Xepoll *epoll_;
    int timer_fd_{-1};
    bool move_dir_{true};
    int angle_{0};
    ServoMotor *servo;
    // Trig *trig;
    int count_{0};
};

#endif

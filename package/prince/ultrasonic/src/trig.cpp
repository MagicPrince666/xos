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
#include <unistd.h>
#include <iostream>

#include "trig.h"

#define GPIO_PIN "/sys/class/leds/trig/brightness"

Trig::Trig() {
    if(access(GPIO_PIN, F_OK) == 0) {
        trig_fd_ = open (GPIO_PIN, O_RDWR);
        std::cout << "Init trig pin fd " << trig_fd_ << std::endl;
    }
    if(trig_fd_ <= 0) {
        std::cout << "Init trig pin error" << std::endl;
        exit(0);
    } else {
        write(trig_fd_, "0", 1);
    }
}

Trig::~Trig() {
    if(trig_fd_ > 0) {
        close(trig_fd_);
    }
}

int Trig::Action() {
    if(trig_fd_ > 0) {
        write(trig_fd_, "1", 1);
        usleep(10);
        write(trig_fd_, "0", 1);
    } else {
        std::cout << "trig pin error" << std::endl;
    }
}

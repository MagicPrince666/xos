#pragma once

#include <vector>
#include "xepoll.h"
#include "interface.h"
#include "trig.h"

class GpioKey
{
public:
    GpioKey(Xepoll *epoll, Interface *interface);
    ~GpioKey();

    bool init();
    int IRKey();

private:
    Xepoll *epoll_;
    int key_input_fd_{-1};
    Interface *m_interface_;
    void getFiles(std::string path, std::vector<std::string>& files);
    struct timeval last_time_;
    Trig *trig;
};

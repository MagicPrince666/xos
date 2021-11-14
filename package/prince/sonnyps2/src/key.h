#pragma once

#include "xepoll.h"
#include "interface.h"

class Key
{
public:
    Key(Xepoll *epoll, Interface *interface);
    ~Key();

    bool init();
    int GpioKey();

private:
    Xepoll *epoll_;
    int key_input_fd_{-1};
    Interface *m_interface_;
};

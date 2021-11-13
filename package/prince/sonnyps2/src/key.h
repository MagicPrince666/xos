#pragma once

#include "xepoll.h"

class Key
{
public:
    Key(Xepoll *epoll);
    ~Key();

    bool init();
    int GpioKey();

private:
    Xepoll *epoll_;
    int key_input_fd_{-1};
};

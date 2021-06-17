#include <linux/input.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "key.h"
#include <iostream>

#define DEV_PATH "/dev/input/event1"

Key::Key(void)
{
    std::cout << "input init" << std::endl;
    key_input_fd = open(DEV_PATH, O_RDONLY);
    if(key_input_fd <= 0) {
        std::cout << "open "<< DEV_PATH <<" device error!" << std::endl;
    }
}

Key::~Key(void)
{
    std::cout << "input deinit" << std::endl;
    if (key_input_fd > 0) {
        close(key_input_fd);
    }
}

int Key::keyprocess(uint8_t* bufout, bool sta)
{
    uint32_t *tempcmd = (uint32_t*)bufout;
    *tempcmd = 0x01090224;

    if(sta)
        bufout[4] = 0x01;
    else
        bufout[4] = 0x00;

    bufout[5] = 0x1D;
    return 6;
}

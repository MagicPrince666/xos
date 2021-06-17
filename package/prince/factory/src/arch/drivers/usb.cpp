#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <string.h>
#include "usb.h"

#include <iostream>

#define USB1_DEV "/dev/sda1"
#define USB2_DEV "/dev/sdb1"

Usb::Usb(void)
{
    std::cout << "usb init" << std::endl;
    usb1_fd = open( USB1_DEV, O_RDONLY);
    usb1_fd = open( USB2_DEV, O_RDONLY);
}

Usb::~Usb(void)
{
    std::cout << "usb deinit" << std::endl;
    if(usb1_fd > 0)
        close(usb1_fd);
    if(usb2_fd > 0)
        close(usb2_fd);
}


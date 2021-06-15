#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

#include "ec20.h"

#define GPS_CHANNEL "/dev/ttyUSB0"
#define DAT_CHANNEL "/dev/ttyUSB1"
#define CMD_CHANNEL "/dev/ttyUSB2"
#define DEB_CHANNEL "/dev/ttyUSB3"

Ec20::Ec20(void)
{
    std::cout << "ec20 init" << std::endl;
    ec20_fd = open(CMD_CHANNEL, O_RDWR | O_NOCTTY);
    if(ec20_fd < 0) {
        std::cout <<"can\'t open " << CMD_CHANNEL <<" !" << std::endl;
        ec20_sta = 0x00;
    }
    ec20_sta = 0x01;
}

Ec20::~Ec20(void)
{
    std::cout << "ec20 deinit" << std::endl;
    if (ec20_fd > 0)
        close(ec20_fd);
}

uint8_t Ec20::get_ec20_status(void)
{
    return ec20_sta;
}
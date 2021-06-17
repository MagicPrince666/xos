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

#include "wifi.h"

#include <iostream>

#define WIFI_UART 	"/dev/ttymxc3"

static int openSerial(const char *cSerialName)
{
    int iFd;

    struct termios opt;

    iFd = open(cSerialName, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(iFd < 0) {
        perror(cSerialName);
        return -1;
    }

    tcgetattr(iFd, &opt);

    cfsetispeed(&opt, B1000000);
    cfsetospeed(&opt, B1000000);

    /*
     * raw mode
     */
    opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG);
    opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
    opt.c_oflag   &=   ~(OPOST);
    opt.c_cflag   &=   ~(CSIZE   |   PARENB);
    opt.c_cflag   |=   CS8;

    /*
     * 'DATA_LEN' bytes can be read by serial
     */
    opt.c_cc[VMIN]   =   0xFF;
    opt.c_cc[VTIME]  =   150;

    if (tcsetattr(iFd, TCSANOW, &opt) < 0) {
        return -1;
    }

    return iFd;
}

Wifi::Wifi(void)
{
    std::cout << "wifi init" << std::endl;

    wifi_fd = openSerial(WIFI_UART);
    if(wifi_fd < 0) {
        std::cout <<"can\'t open " << WIFI_UART <<" !" << std::endl;
        exit(1);
    } else tcflush(wifi_fd, TCIOFLUSH);//清空串口输入输出缓存
}

Wifi::~Wifi(void)
{
    std::cout << "wifi deinit" << std::endl;
    if(wifi_fd > 0)
        close(wifi_fd);
    wifi_fd = -1;
}

const uint8_t ver_cmd[] = {0x82, 00, 0x08, 00, 0x24, 00, 00, 00};
const uint8_t ver_cmd_[] = {0x82, 00, 0x04, 00, 0x54, 0x01, 00, 00};
const uint8_t ip_cmd_[] = {0x82, 00, 0x04, 00, 0x04, 0x00, 00, 00};

int Wifi::get_wifi_vesion(void)
{
    int ret = write(wifi_fd, ver_cmd, sizeof(ver_cmd));
    return ret;
}

int Wifi::get_wifi_address(void)
{
    int ret = write(wifi_fd, ip_cmd_, sizeof(ip_cmd_));
    return ret;
}

int Wifi::handle_wifi_version(uint8_t* bufin, uint8_t* bufout)
{
    int ret = 0;
    uint32_t *tempcmd = (uint32_t*)bufout;
    uint32_t *tempdata = (uint32_t*)bufin;

    if(*tempdata == 0x00040082) {           //返回硬件版本
        if(*(tempdata + 1) == 0x00000154) {
            wifi_data[1] = *(tempdata + 2); //U32 wifi固件版本 v1.0.0.1
            finish = true;
        } else if(*(tempdata + 1) == 0x00000004) {
            *tempcmd++ = 0x040a0224;        //add head
            *tempcmd++ = *(tempdata + 2);   //返回IP地址
            bufout[8] = 0x1D;
            ret = 9;
        }
    } else if(*tempdata == 0x00080082) {    //返回固件版本系统版本
        wifi_data[0] = 0x0c010224;          //add head
        wifi_data[2] = *(tempdata + 2);     //U32 wifi固件版本 v1.0.0.1
        wifi_data[3] = *(tempdata + 3);     //U32 wifi系统版本 v1.0.0.1

        if(write(wifi_fd, ver_cmd_, sizeof(ver_cmd_)) < 0) {
            std::cout << "write error" << wifi_fd << std::endl;
        }

    } else {
        std::cout << "no a head" << std::endl;
    }

    if(finish) {//完整获取到版本号
        for(int i = 0; i < 4; i++)
            *tempcmd++ = wifi_data[i];
        bufout[16] = 0x1D;
        ret = 17;
    }

    return ret;
}

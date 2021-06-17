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

#include "uart.h"
//#include "epoll.h"

#include <iostream>

#define FC1_UART 	"/dev/ttymxc0"
#define FC2_UART 	"/dev/ttymxc2"
#define BAUDRATE     1000000

#define DEBUG_UART 0

#define DATA_LEN    0xFF

int openSerial(const char *cSerialName, int Bitrate)
{
    int iFd;

    struct termios opt;

    iFd = open(cSerialName, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(iFd < 0) {
        perror(cSerialName);
        return -1;
    }

    tcgetattr(iFd, &opt);

	switch(Bitrate) {
		case 115200:
		cfsetispeed(&opt, B115200);
		cfsetospeed(&opt, B115200);
		break;
		case 1000000:
		cfsetispeed(&opt, B1000000);
		cfsetospeed(&opt, B1000000);
		break;
		case 3000000:
		cfsetispeed(&opt, B3000000);
		cfsetospeed(&opt, B3000000);
		break;
		default :
		printf("error uart bitrate!\n");
		exit(0);
		break;
	}

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
    opt.c_cc[VMIN]   =   DATA_LEN;
    opt.c_cc[VTIME]  =   150;

    if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
        return   -1;
    }

    return iFd;
}

Uart::Uart(void)
{
    std::cout << "uart init" << std::endl;
    check_uart2 = false;
    uart2_sta = 0x00;


    fc2_uart_fd = openSerial(FC2_UART, BAUDRATE);
    if(fc2_uart_fd < 0) {
        std::cout <<"can\'t open " << FC2_UART <<" !" << std::endl;
    } else tcflush(fc2_uart_fd, TCIOFLUSH);//清空串口输入输出缓存
}

Uart::~Uart(void)
{
    std::cout << "uart deinit" << std::endl;
    check_uart2 = false;
    uart2_sta = 0x00;

    if(fc2_uart_fd > 0)
        close(fc2_uart_fd);
}

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

#include "led.h"

#include <iostream>

#define GPIO_WIFI_IND0 "/sys/class/leds/LED_WIFI_R/brightness"
#define GPIO_WIFI_IND1 "/sys/class/leds/LED_WIFI_G/brightness"

#define GPIO_GPRS_IND0 "/sys/class/leds/LED_4G_R/brightness"
#define GPIO_GPRS_IND1 "/sys/class/leds/LED_4G_G/brightness"

#define GPIO_POW_IND0 "/sys/class/leds/status/brightness"

Led::Led()
{
    std::cout << "led init" << std::endl;

    led_wifi_fd[0] = -1;
    led_wifi_fd[1] = -1;
	led_gw_fd[0] = -1;
    led_gw_fd[1] = -1;
    led_pwr_fd[0] = -1;

    led_wifi_fd[0] = open(GPIO_WIFI_IND0, O_RDWR);
    if(led_wifi_fd[0] < 0)
    {
        std::cout << "open led wifi r fail"<< std::endl;
        exit (1);
    }

    led_wifi_fd[1] = open(GPIO_WIFI_IND1, O_RDWR);
    if(led_wifi_fd[1] < 0)
    {
        std::cout << "open led wifi g fail"<< std::endl;
        exit (1);
    }

    led_gw_fd[0] = open(GPIO_GPRS_IND0, O_RDWR);
    if(led_gw_fd[0] < 0)
    {
        std::cout << "open led gw r fail"<< std::endl;
        exit (1);
    }
    led_gw_fd[1] = open(GPIO_GPRS_IND1, O_RDWR);
    if(led_gw_fd[1] < 0)
    {
        std::cout << "open led gw g fail"<< std::endl;
        exit (1);
    }

    led_pwr_fd[0] = open(GPIO_POW_IND0, O_RDWR);
    if(led_pwr_fd[0] < 0)
    {
        std::cout << "open led led_pwr_fd g fail"<< std::endl;
        //exit (1);
    }
}

Led::~Led()
{
    std::cout << "led deinit" << std::endl;
    int ret = 0;
    ret = write(led_wifi_fd[0], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_wifi_fd[0] << std::endl;
    ret = write(led_wifi_fd[1], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_wifi_fd[1] << std::endl;
    ret = write(led_gw_fd[0], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_gw_fd[0] << std::endl;
    ret = write(led_gw_fd[1], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_gw_fd[1] << std::endl;
    close(led_wifi_fd[0]);
    close(led_wifi_fd[1]);
    close(led_gw_fd[0]);
    close(led_gw_fd[1]);
    close(led_pwr_fd[0]);
    led_wifi_fd[0] = -1;
    led_wifi_fd[1] = -1;
	led_gw_fd[0] = -1;
    led_gw_fd[1] = -1;
    led_pwr_fd[0] = -1;
}

void Led::running(void)
{
    int ret = 0;
    ret = write(led_wifi_fd[0], "0", 1); //LED ON
    if(ret < 0) std::cout << "write error" << led_wifi_fd[0] << std::endl;
    usleep(500000);
    ret = write(led_wifi_fd[0], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_wifi_fd[0] << std::endl;
    ret = write(led_wifi_fd[1], "0", 1); //LED ON
    if(ret < 0) std::cout << "write error" << led_wifi_fd[1] << std::endl;
    usleep(500000);
    ret = write(led_wifi_fd[1], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_wifi_fd[1] << std::endl;
    ret = write(led_gw_fd[0], "0", 1); //LED ON
    if(ret < 0) std::cout << "write error" << led_gw_fd[0] << std::endl;
    usleep(500000);
    ret = write(led_gw_fd[0], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_gw_fd[0] << std::endl;
    ret = write(led_gw_fd[1], "0", 1); //LED ON
    if(ret < 0) std::cout << "write error" << led_gw_fd[1] << std::endl;
    usleep(500000);
    ret = write(led_gw_fd[1], "1", 1); //LED OFF
    if(ret < 0) std::cout << "write error" << led_gw_fd[1] << std::endl;
    usleep(500000);
}

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>

#include "spi.h"

Spi::Spi(std::string dev, uint8_t mode, uint8_t bits, uint32_t speed)
    : spi_dev_(dev), spi_mode_(mode), spi_bits_(bits), spi_speed_(speed)
{
    std::cout << "spi init" << std::endl;
    SPIOpen();
}

Spi::~Spi(void)
{
    std::cout << "spi deinit" << std::endl;
    SPIClose();
}

static void pabort(const char *s)
{
    perror(s);
    abort();
}

/**
* 功 能：发送数据
* 入口参数 ：
* TxBuf -> 发送数据首地址
＊len -> 发送与长度
＊返回值：0 成功
* 开发人员：Lzy 2013－5－22
*/
int Spi::SPIWrite(uint8_t *TxBuf, int len)
{
    int ret = write(g_SPI_Fd_, TxBuf, len);

    if (ret < 0)
        printf("SPI Write error\n");

    return ret;
}


/**
* 功 能：接收数据
* 出口参数：
* RxBuf -> 接收数据缓冲区
* rtn -> 接收到的长度
* 返回值：>=0 成功
* 开发人员：Lzy 2013－5－22
*/
int Spi::SPIRead(uint8_t *RxBuf, int len)
{
    int ret = read(g_SPI_Fd_, RxBuf, len);

    if (ret < 0)
        printf("SPI Read errorn\n");

    return ret;
}


/**
* 功 能：打开设备 并初始化设备
* 入口参数 ：
* 出口参数：
* 返回值：0 表示已打开 0XF1 表示SPI已打开 其它出错
* 开发人员：Lzy 2013－5－22
*/
int Spi::SPIOpen()
{
    int fd;
    int ret = 0;

    if (g_SPI_Fd_ > 0) /* 设备已打开 */
        return 0xF1;

    fd = open(spi_dev_.c_str(), O_RDWR);

    if (fd < 0)
        pabort("can't open device");
    else
        printf("SPI %s - Open Succeed. Start Init SPI...\n", spi_dev_.c_str());

    g_SPI_Fd_ = fd;
    /*
    * spi mode
    */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode_);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode_);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
    * bits per word
    */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits_);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
    * max speed hz
    */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed_);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: 0x%02X\n", spi_mode_);
    printf("bits per word: %d\n", spi_bits_);
    printf("max speed: %d KHz (%d MHz)\n", spi_speed_ / 1000, spi_speed_ / 1000 / 1000);

    return ret;
}


/**
* 功 能：关闭SPI模块
*/
int Spi::SPIClose(void)
{
    if (g_SPI_Fd_ == 0) /* SPI是否已经打开*/
        return 0;

    close(g_SPI_Fd_);

    return 0;
}


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

Spi::Spi(std::string dev, uint8_t mode, uint8_t lsb, uint8_t bits, uint32_t speed)
    : spi_dev_(dev), spi_mode_(mode), spi_lsb_(lsb),spi_bits_(bits), spi_speed_(speed)
{
    SPIOpen();
}

Spi::~Spi(void)
{
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
#if 0
    int ret = write(spi_Fd_, TxBuf, len);
#else
    struct spi_ioc_transfer	xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (uint64_t)TxBuf;
	xfer.len = len;
    int ret = ioctl(spi_Fd_, SPI_IOC_MESSAGE(1), &xfer);
#endif
    if (ret < 0) {
        printf("SPI Write error\n");
    }

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
#if 0
    int ret = read(spi_Fd_, RxBuf, len);
#else
    struct spi_ioc_transfer	xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (uint64_t)RxBuf;
	xfer.len = len;
    int ret = ioctl(spi_Fd_, SPI_IOC_MESSAGE(1), &xfer);
#endif
    
    if (ret < 0) {
        printf("SPI Read errorn\n");
    }

    return ret;
}

int Spi::TransferSpiBuffers(const void *tx_buffer, void *rx_buffer, uint32_t length)
{
    struct spi_ioc_transfer	xfer;

    memset(&xfer, 0, sizeof(xfer));

    xfer.tx_buf = (uint64_t)tx_buffer;
    xfer.rx_buf = (uint64_t)rx_buffer;
	xfer.len = length;
    xfer.delay_usecs = 100;
//    xfer.cs_change = 1;

	if (ioctl(spi_Fd_, SPI_IOC_MESSAGE(1), &xfer) < 0) {
        perror("SPI_IOC_MESSAGE");
        return -1;
    }

	return 0;
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

    if (spi_Fd_ > 0) { /* 设备已打开 */
        return 0;
    }

    fd = open(spi_dev_.c_str(), O_RDWR);

    if (fd < 0) {
        pabort("can't open device");
        return -1;
    } else {
        printf("SPI %s - Open Succeed. Start Init SPI...\n", spi_dev_.c_str());
    }

    spi_Fd_ = fd;
    /*
    * spi mode
    */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode_);
    if (ret < 0) {
        pabort("can't set spi mode");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode_);
    if (ret < 0) {
        pabort("can't get spi mode");
    }

    /*
    * spi lsb first
    */
    ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &spi_lsb_);
	if (ret < 0) {
	    pabort("SPI_IOC_WR_LSB_FIRST");
	}

    ret = ioctl(fd, SPI_IOC_RD_LSB_FIRST, &spi_lsb_);
    if (ret < 0) {
	    pabort("SPI_IOC_RD_LSB_FIRST");
	}

    /*
    * bits per word
    */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_);
    if (ret < 0) {
        pabort("can't set bits per word");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits_);
    if (ret < 0) {
        pabort("can't get bits per word");
    }

    /*
    * max speed hz
    */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_);
    if (ret < 0) {
        pabort("can't set max speed hz");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed_);
    if (ret < 0) {
        pabort("can't get max speed hz");
    }

    printf("spi mode: 0x%02X\n", spi_mode_);
    if(spi_lsb_) {
        printf("lsb first %02X\n", spi_lsb_);
    } else {
        printf("msb first %02X\n", spi_lsb_);
    }
    printf("bits per word: %d\n", spi_bits_);
    printf("max speed: %d KHz\n", spi_speed_ / 1000);

    return ret;
}


/**
* 功 能：关闭SPI模块
*/
int Spi::SPIClose(void)
{
    if (spi_Fd_ <= 0) { /* SPI是否已经打开*/
        return 0;
    }

    close(spi_Fd_);

    return 0;
}


#ifndef __SPI_H__
#define __SPI_H__

#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <iostream>

#define SPI_MODE_0 (0|0)                //SCLK空闲时为低电平，第一个时间延采样
#define SPI_MODE_1 (0|SPI_CPHA)         //SCLK空闲时为高电平，第一个时间延采样
#define SPI_MODE_2 (SPI_CPOL|0)         //SCLK空闲时为低电平，第二个时间延采样
#define SPI_MODE_3 (SPI_CPOL|SPI_CPHA)  //SCLK空闲时为高电平，第二个时间延采样
#define SPI_CS_HIGH 0x04                //片选为高
#define SPI_LSB_FIRST 0x08              //低位数据先传输
#define SPI_3WIRE 0x10                  //三线式，输入输出数据线为一条线
#define SPI_LOOP 0x20                   //回环模式
#define SPI_NO_CS 0x40                  //没有片选信号
#define SPI_READY 0x80                  //

class Spi
{
public:
    explicit Spi(std::string dev = "/dev/spidev1.0", uint8_t mode = SPI_LSB_FIRST | SPI_MODE_3, uint8_t bits = 8, uint32_t speed = 1000000);
    ~Spi();
    int SPIWrite(uint8_t *TxBuf, int len);
    int SPIRead(uint8_t *RxBuf, int len);

private:
    int SPIOpen();
    int SPIClose(void);
    int g_SPI_Fd_{-1};
    std::string spi_dev_;
    uint8_t spi_mode_;
    uint8_t spi_bits_;
    uint32_t spi_speed_;
};

#endif

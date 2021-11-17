#ifndef __SPI_H__
#define __SPI_H__

#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <iostream>

#define SPI_MODE_0 (0|0)                //SCLK空闲时为低电平，第一个时间延采样
#define SPI_MODE_1 (0|SPI_CPHA)         //SCLK空闲时为高电平，第一个时间延采样
#define SPI_MODE_2 (SPI_CPOL|0)         //SCLK空闲时为低电平，第二个时间延采样
#define SPI_MODE_3 (SPI_CPOL|SPI_CPHA)  //SCLK空闲时为高电平，第二个时间延采样

class Spi
{
public:
    explicit Spi(std::string dev = "/dev/spidev1.0", uint8_t mode = SPI_MODE_3, uint8_t lsb = 0x01, uint8_t bits = 8, uint32_t speed = 1000000);
    ~Spi();
    int SPIWrite(uint8_t *TxBuf, int len);
    int SPIRead(uint8_t *RxBuf, int len);
    int TransferSpiBuffers(const void *tx_buffer, void *rx_buffer, uint32_t length);

private:
    int SPIOpen();
    int SPIClose(void);
    int spi_Fd_{-1};
    std::string spi_dev_;
    uint8_t spi_mode_;
    uint8_t spi_lsb_;
    uint8_t spi_bits_;
    uint32_t spi_speed_;
};

#endif

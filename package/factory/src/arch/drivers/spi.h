#ifndef __SPI_H__
#define __SPI_H__

#include <linux/types.h>

class Spi
{
public:
    Spi();
    ~Spi();
    int SPI_Write(uint8_t *TxBuf, int len);
    int SPI_Read(uint8_t *RxBuf, int len);

private:
    int SPI_Open(void);
    int SPI_Close(void);
    int g_SPI_Fd = 0;
};

#endif

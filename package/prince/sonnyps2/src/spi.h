#ifndef __SPI_H__
#define __SPI_H__

#include <linux/types.h>

class Spi
{
public:
    Spi();
    ~Spi();
    int SPIWrite(uint8_t *TxBuf, int len);
    int SPIRead(uint8_t *RxBuf, int len);

private:
    int SPIOpen(void);
    int SPIClose(void);
    int g_SPI_Fd = 0;
};

#endif

#ifndef __CRC16_H__
#define __CRC16_H__

#include <inttypes.h>

class Crc16
{
public:
    Crc16();
    ~Crc16();
    uint16_t Utils_crc16_ccitt(const uint8_t *buf, uint32_t len);
    uint16_t CRC16_CCITT(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_CCITT_FALSE(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_XMODEM(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_X25(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_MODBUS(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_DNP(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_USB(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_MAXIM(uint8_t *data, uint32_t datalen);
    uint16_t CRC16_IBM(uint8_t *data, uint32_t datalen);
private:
};

#endif
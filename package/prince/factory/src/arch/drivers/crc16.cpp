#include <inttypes.h>
#include "crc16.h"
#include <iostream>

static const uint16_t crc16tab[256]= {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

Crc16::Crc16()
{
    std::cout << "Crc16 init" << std::endl;
}

Crc16::~Crc16()
{
    std::cout << "Crc16 deinit" << std::endl;
}

uint16_t Crc16::Utils_crc16_ccitt(const uint8_t *buf, uint32_t len)
{
    uint32_t counter;
    uint16_t crc = 0xFFFF;
    for( counter = 0; counter < len; counter++)
    {
        crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(uint8_t *)buf++)&0x00FF];
    }
    return ~crc;
}

/****************************Info********************************************** 
 * Name:    InvertUint8 
 * Note: 	把字节颠倒过来，如0x12变成0x48
			0x12: 0001 0010
			0x48: 0100 1000
 *****************************************************************************/
void InvertUint8(uint8_t *dBuf,uint8_t *srcBuf)
{
	int i;
	uint8_t tmp[4]={0};
 
	for(i=0;i< 8;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(7-i);
	}
	dBuf[0] = tmp[0];
	
}

void InvertUint16(uint16_t *dBuf,uint16_t *srcBuf)
{
	int i;
	uint16_t tmp[4]={0};
 
	for(i=0;i< 16;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(15 - i);
	}
	dBuf[0] = tmp[0];
}

void InvertUint32(uint32_t *dBuf,uint32_t *srcBuf)
{
	int i;
	uint32_t tmp[4]={0};
	
	for(i=0;i< 32;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(31 - i);
	}
	dBuf[0] = tmp[0];
}

/****************************Info********************************************** 
 * Name:    CRC-16/CCITT        x16+x12+x5+1 
 * Width:	16
 * Poly:    0x1021 
 * Init:    0x0000 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0x0000 
 * Alias:   CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT 
 *****************************************************************************/ 
#if 0
uint16_t crc16::CRC16_CCITT(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin);
}
#else 
//这里为了效率，我们不需要将所有Refin和refout为true的输入输出数据移位转换
//只需要将poly二项式转换后，运算时将左移变为右移
uint16_t Crc16::CRC16_CCITT(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/CCITT-FALSE   x16+x12+x5+1 
 * Width:	16 
 * Poly:    0x1021 
 * Init:    0xFFFF 
 * Refin:   False 
 * Refout:  False 
 * Xorout:  0x0000 
 * Note: 
 *****************************************************************************/ 
uint16_t Crc16::CRC16_CCITT_FALSE(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x1021;
	
	while (datalen--) 	
	{
		wCRCin ^= *(data++) << 8;
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info********************************************** 
 * Name:    CRC-16/XMODEM       x16+x12+x5+1 
 * Width:	16 
 * Poly:    0x1021 
 * Init:    0x0000 
 * Refin:   False 
 * Refout:  False 
 * Xorout:  0x0000 
 * Alias:   CRC-16/ZMODEM,CRC-16/ACORN 
 *****************************************************************************/ 
uint16_t Crc16::CRC16_XMODEM(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	
	while (datalen--) 	
	{
		wCRCin ^= (*(data++) << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info********************************************** 
 * Name:    CRC-16/X25          x16+x12+x5+1 
 * Width:	16 
 * Poly:    0x1021 
 * Init:    0xFFFF 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0XFFFF 
 * Note: 
 *****************************************************************************/
#if 0 
uint16_t Crc16::CRC16_X25(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x1021;
	uint8_t wChar = 0;
 
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin^0xFFFF);
}
#else 
uint16_t Crc16::CRC16_X25(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x1021;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin^0xFFFF);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/MODBUS       x16+x15+x2+1 
 * Width:	16 
 * Poly:    0x8005 
 * Init:    0xFFFF 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0x0000 
 * Note: 
 *****************************************************************************/
#if 0 
uint16_t Crc16::CRC16_MODBUS(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin);
}
#else 
uint16_t Crc16::CRC16_MODBUS(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/IBM          x16+x15+x2+1 
 * Width:	16 
 * Poly:    0x8005 
 * Init:    0x0000 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0x0000 
 * Alias:   CRC-16,CRC-16/ARC,CRC-16/LHA 
 *****************************************************************************/ 
#if 0
uint16_t Crc16::CRC16_IBM(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x8005;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin);
}
#else 
uint16_t Crc16::CRC16_IBM(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x8005;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/MAXIM        x16+x15+x2+1 
 * Width:	16 
 * Poly:    0x8005 
 * Init:    0x0000 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0xFFFF 
 * Note: 
 *****************************************************************************/
#if 0
uint16_t Crc16::CRC16_MAXIM(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x8005;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin^0xFFFF);
}
#else 
uint16_t Crc16::CRC16_MAXIM(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x8005;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin^0xFFFF);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/USB          x16+x15+x2+1 
 * Width:	16 
 * Poly:    0x8005 
 * Init:    0xFFFF 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0xFFFF 
 * Note: 
 *****************************************************************************/ 
#if 0
uint16_t Crc16::CRC16_USB(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin^0xFFFF);
}
#else 
uint16_t Crc16::CRC16_USB(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin^0xFFFF);
}
#endif
/****************************Info********************************************** 
 * Name:    CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1 
 * Width:	16 
 * Poly:    0x3D65 
 * Init:    0x0000 
 * Refin:   True 
 * Refout:  True 
 * Xorout:  0xFFFF 
 * Use:     M-Bus,ect. 
 *****************************************************************************/  
#if 0
uint16_t Crc16::CRC16_DNP(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x3D65;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin^0xFFFF) ;
}
#else
uint16_t Crc16::CRC16_DNP(uint8_t *data, uint32_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x3D65;
	
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = (wCRCin >> 1);
		}
	}
	return (wCRCin^0xFFFF);
}
#endif

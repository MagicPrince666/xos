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
#include <iostream>

#include "pstwo.h"

u_int16_t Handkey;
u_int8_t Comd[2] = {0x01, 0x42};
u_int8_t Data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
u_int16_t MASK[] = {
	PSB_SELECT,
	PSB_L3,
	PSB_R3 ,
	PSB_START,
	PSB_PAD_UP,
	PSB_PAD_RIGHT,
	PSB_PAD_DOWN,
	PSB_PAD_LEFT,
	PSB_L2,
	PSB_R2,
	PSB_L1,
	PSB_R1 ,
	PSB_GREEN,
	PSB_RED,
	PSB_BLUE,
	PSB_PINK
};

Ps2Remote::Ps2Remote()
{
	std::cout << "Init Sony PS2" << std::endl;
	spidev = new Spi();
	PS2_SetInit();
}

Ps2Remote::~Ps2Remote()
{
	std::cout << "Deinit Sony PS2" << std::endl;
	delete spidev;
}

void Ps2Remote::PS2_Cmd(uint8_t *cmd, int len)
{
	spidev->SPIWrite(cmd, len);
}


u_int8_t Ps2Remote::PS2_RedLight(void)
{
	PS2_Cmd(Comd, sizeof(Comd));

	if( Data[1] == 0X73) {
		return 0 ;
	} else {
		return 1;
	}
}

void Ps2Remote::PS2_ReadData(void)
{
	PS2_Cmd(Comd, sizeof(Comd));
	spidev->SPIRead(Data + 2, 7);
}

u_int8_t Ps2Remote::PS2_DataKey()
{
	u_int8_t index;

	PS2_ClearData();
	PS2_ReadData();
	
	printf("Data[");
	for( uint8_t i = 0; i < 10; i++) {
		printf("%02x ", Data[i]);
	}
	printf("]\n");

	Handkey = (Data[4] << 8) | Data[3];
	for(index = 0; index < 16; index++) {	    
		if((Handkey & (1<<(MASK[index]-1))) == 0) {
			return index + 1;
		}
	}
	return 0;         
}

u_int8_t Ps2Remote::PS2_AnologData(u_int8_t button)
{
	return Data[button];
}

void Ps2Remote::PS2_ClearData()
{
	memset(Data, 0, sizeof(Data));
}

void Ps2Remote::PS2_Vibration(u_int8_t motor1, u_int8_t motor2)
{
	u_int8_t comd[9] = {0x01, 0x42, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	comd[3] = motor1;
	comd[4] = motor2;
	PS2_Cmd(comd, sizeof(comd));
}

void Ps2Remote::PS2_ShortPoll(void)
{
	u_int8_t comd[5] = {0x01, 0x42, 0X00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}

void Ps2Remote::PS2_EnterConfing(void)
{
	u_int8_t comd[9] = {0x01, 0x43, 0X00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}


void Ps2Remote::PS2_TurnOnAnalogMode(void)
{
	u_int8_t comd[9] = {0x01, 0x44, 0X00, 0x01, 0xee, 0x00, 0x00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}

void Ps2Remote::PS2_VibrationMode(void)
{
	u_int8_t comd[5] = {0x01, 0x4d, 0X00, 0x00, 0x01};
	PS2_Cmd(comd, sizeof(comd));
}

void Ps2Remote::PS2_ExitConfing(void)
{
	u_int8_t comd[9] = {0x01, 0x43, 0X00, 0x00, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
	PS2_Cmd(comd, sizeof(comd));
}

void Ps2Remote::PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();
	PS2_TurnOnAnalogMode();
	PS2_VibrationMode();
	PS2_ExitConfing();
}
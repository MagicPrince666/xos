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

uint16_t Handkey;
uint8_t Comd[2] = {0x01, 0x42};
uint8_t Data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint16_t MASK[] = {
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
	spidev = new Spi();

	uint32_t buf = 0xFFFFFFFF;
	spidev->TransferSpiBuffers(&buf, &buf, 4, 4);

	PS2_SetInit();
}

Ps2Remote::~Ps2Remote()
{
	delete spidev;
}

void Ps2Remote::PS2_Cmd(uint8_t *cmd, int len)
{
	// spidev->SPIWrite(cmd, len);
	// for(uint32_t i = 0; i < len; i++) {
	// 	spidev->TransferSpiBuffers(cmd + i, Data + i, 1, 1);
	// 	usleep(30);
	// }
	spidev->TransferSpiBuffers(cmd, Data, len, 2);
}

// 判断是否为红灯模式，0x41=模拟绿灯，0x73=模拟红灯 
// 返回值；0，红灯模式 
// 其他，其他模式
uint8_t Ps2Remote::PS2_RedLight(void)
{
	PS2_Cmd(Comd, sizeof(Comd));

	if( Data[1] == 0X73) {
		return 0 ;
	} else {
		return 1;
	}
}

// 读取手柄数据 
void Ps2Remote::PS2_ReadData(void)
{
	// PS2_Cmd(Comd, sizeof(Comd));
	// spidev->SPIRead(Data + 2, 7);
	// for(uint32_t i = 0; i < 9; i++) {
	// 	spidev->TransferSpiBuffers(Comd + i, Data + i, 1, 1);
	// 	usleep(30);
	// }
	spidev->TransferSpiBuffers(Comd, Data, sizeof(Comd), 9);
}

/*
上面两个函数分别为主机向手柄发送数据、手柄向主机发送数据。手柄向主机发送的数据缓存在数组 Data[]中，
数组中共有9个元素，每个元素的意义请见表1。
还有一个函数是用来判断手柄的发送模式，也就是判断 ID(红灯还是绿灯模式) 即 Data[1]的值。
*/

// 对读出来的 PS2 的数据进行处理,只处理按键部分
// 按下为0，未按下为1
uint8_t Ps2Remote::PS2_DataKey()
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();
	
	printf("Data[ ");
	for( uint8_t i = 0; i < 9; i++) {
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

// 得到一个摇杆的模拟量 范围 0~256 
uint8_t Ps2Remote::PS2_AnologData(uint8_t button)
{
	return Data[button];
}

// 清除数据缓冲区
void Ps2Remote::PS2_ClearData()
{
	memset(Data, 0, sizeof(Data));
}

/*
8 位数 Data[3]与 Data[4]，分别对应着 16个按键的状态，按下为 0，未按下为 1。
通过 对这两个数的处理，得到按键状态并返回键值。
另一个函数的功能就是返回模拟值，只有在“红灯模式”下值才是有效的，拨动摇杆，值才会变化，这些值分别存储在 Data[5]、Data[6]、
Data[7]、 Data[8]。 
*/


// 手柄配置初始化：
void Ps2Remote::PS2_ShortPoll(void)
{
	uint8_t comd[] = {0x01, 0x42, 0X00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}

// 进入配置
void Ps2Remote::PS2_EnterConfing(void)
{
	uint8_t comd[] = {0x01, 0x43, 0X00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}


void Ps2Remote::PS2_TurnOnAnalogMode(void)
{
	uint8_t comd[] = {0x01, 0x44, 0X00, 0x01, 0xee, 0x00, 0x00, 0x00, 0x00};
	PS2_Cmd(comd, sizeof(comd));
}

// 振动设置
void Ps2Remote::PS2_VibrationMode(void)
{
	uint8_t comd[] = {0x01, 0x4d, 0X00, 0x00, 0x01};
	PS2_Cmd(comd, sizeof(comd));
}

// 完成并保存配置
void Ps2Remote::PS2_ExitConfing(void)
{
	uint8_t comd[] = {0x01, 0x43, 0X00, 0x00, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
	PS2_Cmd(comd, sizeof(comd));
}

/*
可以看出配置函数就是发送命令，发送这些命令后，手柄就会明白自己要做什么了，发送命令时，不需要考虑手柄发来的信息。
手柄配置初始化，PS2_ShortPoll()被执行了3次，主要是为了建立和恢复连接。
具体的配置方式请看注释。
*/

void Ps2Remote::PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	uint8_t comd[] = {0x01, 0x42, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	comd[3] = motor1;
	comd[4] = motor2;
	PS2_Cmd(comd, sizeof(comd));
}

// 只有在初始化函数void PS2_SetInit(void)中，对震动电机进行了初始化（PS2_VibrationMode();
// 开启震动模式，这个函数命令才会被执行。
void Ps2Remote::PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();
	PS2_TurnOnAnalogMode();
	PS2_VibrationMode();
	PS2_ExitConfing();

	// 震动测试
	PS2_Vibration(0xFF,0xFF);
}
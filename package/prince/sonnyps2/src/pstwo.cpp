#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include "pstwo.h"
#include "key.h"

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

//const uint8_t PSX_CMD_INIT_PRESSURE[]	= {0x01, 0x40, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_INIT_PRESSURE[] = {0x80, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
//const uint8_t PSX_CMD_POLL[]		= 	{0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_POLL[]	=		{0x80, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//const uint8_t PSX_CMD_ENTER_CFG[]	= 	{0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_ENTER_CFG[]	= 	{0x80, 0xC2, 0X00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};
//const uint8_t PSX_CMD_EXIT_CFG[]	= 	{0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
const uint8_t PSX_CMD_EXIT_CFG[] = 		{0x80, 0xc2, 0X00, 0x00, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
//const uint8_t PSX_CMD_ENABLE_MOTOR[]	= {0x01, 0x4D, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t PSX_CMD_ENABLE_MOTOR[] = 	{0x80, 0xB2, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t PSX_ANALOGMODE[] = 		{0x80, 0x22, 0X00, 0x80, 0x77, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_ALL_PRESSURE[]	= {0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_AD_MODE[]		= {0x01, 0x44, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};


#define REVERSE_BIT(x) ((((x) & 0x80) >> 7) | (((x) & 0x40) >> 5) | (((x) & 0x20) >> 3) | (((x) & 0x10) >> 1) | (((x) & 0x08) << 1) | (((x) & 0x04) << 3) | (((x) & 0x02) << 5) | (((x) & 0x01) << 7))

Ps2Remote::Ps2Remote()
{
	// sonny ps2 最高支持125k Hz 时钟
	spidev = new Spi("/dev/spidev1.0", SPI_MODE_3, 0x0, 8, 125000);
	PS2_SetInit();
}

Ps2Remote::~Ps2Remote()
{
	delete spidev;
}

void Ps2Remote::PS2_Cmd(const uint8_t *cmd, int len)
{
	spidev->TransferSpiBuffers(cmd, Data, len);
}

// 判断是否为红灯模式，0x41=模拟绿灯，0x73=模拟红灯 
// 返回值；0，红灯模式 
// 其他，其他模式
uint8_t Ps2Remote::PS2_RedLight(void)
{
	PS2_Cmd(Comd, sizeof(Comd));

	if( Data[1] == 0x73) {
		return 0 ;
	} else {
		return 1;
	}
}

// 读取手柄数据 
void Ps2Remote::PS2_ReadData(void)
{
	spidev->TransferSpiBuffers(PSX_CMD_POLL, Data, sizeof(Data));
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
	for(int i = 0; i < 9; i++)
		Data[i] = REVERSE_BIT(Data[i]);
	
	printf("Data[ ");
	for( uint8_t i = 0; i < 9; i++) {
		printf("%02X ", Data[i]);
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
	PS2_Cmd(PSX_CMD_INIT_PRESSURE, sizeof(PSX_CMD_INIT_PRESSURE));
}

// 进入配置
void Ps2Remote::PS2_EnterConfing(void)
{
	PS2_Cmd(PSX_CMD_ENTER_CFG, sizeof(PSX_CMD_ENTER_CFG));
}


void Ps2Remote::PS2_TurnOnAnalogMode(void)
{
	PS2_Cmd(PSX_ANALOGMODE, sizeof(PSX_ANALOGMODE));
}

// 振动设置
void Ps2Remote::PS2_VibrationMode(void)
{
	PS2_Cmd(PSX_CMD_ENABLE_MOTOR, sizeof(PSX_CMD_ENABLE_MOTOR));
}

// 完成并保存配置
void Ps2Remote::PS2_ExitConfing(void)
{
	PS2_Cmd(PSX_CMD_EXIT_CFG, sizeof(PSX_CMD_EXIT_CFG));
}

/*
可以看出配置函数就是发送命令，发送这些命令后，手柄就会明白自己要做什么了，发送命令时，不需要考虑手柄发来的信息。
手柄配置初始化，PS2_ShortPoll()被执行了3次，主要是为了建立和恢复连接。
具体的配置方式请看注释。
*/

void Ps2Remote::PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	uint8_t cmd[] = {0x80, 0x42, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	cmd[3] = motor1;
	cmd[4] = motor2;
	PS2_Cmd(cmd, sizeof(cmd));
}

// 只有在初始化函数void PS2_SetInit(void)中，对震动电机进行了初始化（PS2_VibrationMode();
// 开启震动模式，这个函数命令才会被执行。
void Ps2Remote::PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();
	PS2_ShortPoll();
	PS2_TurnOnAnalogMode();
	PS2_VibrationMode();
	PS2_ExitConfing();
	std::cout << "Sonny PS2 set init" << std::endl;
	// 震动测试
	PS2_Vibration(0x00,0xFF);
}

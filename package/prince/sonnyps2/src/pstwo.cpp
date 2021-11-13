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
#if _USE_GPIO
	gpio_ps2 = new Gpio;
	ps2_data = gpio_ps2->gpio_init(128 + 10, 0);	//PE10 138
	ps2_cmd = gpio_ps2->gpio_init(128 + 8, 1);	//PE8 136
	ps2_att = gpio_ps2->gpio_init(128 + 7, 1);	//PE7 135
	ps2_clk = gpio_ps2->gpio_init(128 + 9, 1);	//PE9 137

	gpio_ps2->set_gpio_value(ps2_data, 1);
	gpio_ps2->set_gpio_value(ps2_cmd, 0);
	gpio_ps2->set_gpio_value(ps2_att, 1);
	gpio_ps2->set_gpio_value(ps2_clk, 1);
#else
	spidev = new Spi;
#endif
	PS2_SetInit();
}

Ps2Remote::~Ps2Remote()
{
#if _USE_GPIO
	delete gpio_ps2;
#else
	delete spidev;
#endif
	std::cout << "Deinit PS2" << std::endl;
}

void Ps2Remote::PS2_Cmd(u_int8_t cmd)
{
#if _USE_GPIO
	volatile u_int16_t ref = 0x01;
	Data[1] = 0;
	for(ref = 0x01; ref < 0x0100; ref <<= 1)
	{
		if(ref&cmd){
			gpio_ps2->set_gpio_value(ps2_cmd, 1);
		} else {
			gpio_ps2->set_gpio_value(ps2_cmd, 0);
		}

		gpio_ps2->set_gpio_value(ps2_clk,0);
		gpio_ps2->set_gpio_value(ps2_clk,1);

		if(gpio_ps2->get_gpio_value(ps2_data))
			Data[1] = ref|Data[1];
	}
#else
	spidev->SPIWrite(&cmd, 1);
	spidev->SPIRead(&Data[1], 1);
#endif
}


u_int8_t Ps2Remote::PS2_RedLight(void)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 0);
#endif
	PS2_Cmd(Comd[0]);
	PS2_Cmd(Comd[1]);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 1);
#endif
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}

void Ps2Remote::PS2_ReadData(void)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 0);
#endif

	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);  
#if _USE_GPIO
	volatile u_int8_t byte = 0;
	volatile u_int16_t ref = 0x01;
	for(byte = 2; byte < 9; byte++)
	{
		for(ref = 0x01; ref < 0x100; ref <<= 1)
		{
			gpio_ps2->set_gpio_value(ps2_clk,0);
			gpio_ps2->set_gpio_value(ps2_clk,1);
			
			if(gpio_ps2->get_gpio_value(ps2_data))
		      Data[byte] = ref | Data[byte];
		}
	}
#else
	spidev->SPIRead(&data, 1);
#endif

#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 1);
#endif
}


u_int8_t Ps2Remote::PS2_DataKey()
{
	u_int8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey = (Data[4]<<8) | Data[3];
	for(index = 0; index < 16; index++) {	    
		if((Handkey & (1<<(MASK[index]-1))) == 0)
		return index + 1;
	}
	return 0;         
}


u_int8_t Ps2Remote::PS2_AnologData(u_int8_t button)
{
	return Data[button];
}


void Ps2Remote::PS2_ClearData()
{
	for(u_int8_t a = 0; a < 9; a++)
		Data[a] = 0x00;
}


void Ps2Remote::PS2_Vibration(u_int8_t motor1, u_int8_t motor2)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x42);
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att, 1);
#endif
}
//short poll
void Ps2Remote::PS2_ShortPoll(void)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x42);
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,1);
#endif
}

void Ps2Remote::PS2_EnterConfing(void)
{
#if _USE_GPIO
  	gpio_ps2->set_gpio_value(ps2_att,0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x43);
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,1);
#endif
}


void Ps2Remote::PS2_TurnOnAnalogMode(void)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x44);
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00 
	PS2_Cmd(0xEE); //Ox03
				   //0xEE
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,1);
#endif
}

void Ps2Remote::PS2_VibrationMode(void)
{
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x4D);
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,1);
#endif
}
//
void Ps2Remote::PS2_ExitConfing(void)
{
#if _USE_GPIO
    gpio_ps2->set_gpio_value(ps2_att,0);
#endif
	PS2_Cmd(0x01);
	PS2_Cmd(0x43);
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
#if _USE_GPIO
	gpio_ps2->set_gpio_value(ps2_att,1);
#endif
}

void Ps2Remote::PS2_SetInit(void)
{
	std::cout << "init PS2" << std::endl;
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();
	PS2_TurnOnAnalogMode();
	PS2_VibrationMode();
	PS2_ExitConfing();
}
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
//#include "moto.h"

static void sigint_handler(int sig)
{
    std::cout << "--- quit the loop! ---" << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
	u_int8_t key = 0;
	u_int8_t l_lx = 0,l_ly = 0,l_rx = 0,l_ry = 0;
	u_int8_t lx,ly,rx,ry;
	int speed = 0;
	Ps2Remote ps2;

	//Moto moto;

	signal(SIGINT, sigint_handler);//信号处理

	std::cout << "--- version 1.1 ---" << std::endl;

	while(1) {

		key = ps2.PS2_DataKey();
		if(key != 0)               
		{
			if(key > 1)
				std::cout << "key = " << key << std::endl;

			if(key == 12) {
				ps2.PS2_Vibration(0xFF,0x00);
				usleep(500000);
			} else if(key == 11) {
				ps2.PS2_Vibration(0x00,0xFF);
				usleep(500000);
			} else
				ps2.PS2_Vibration(0x00,0x00);

			switch(key)
			{
				case 5:
				break;
				case 7: //moto.servo(1500000);
				break;
				case 6: //moto.servo(2000000);
				break;
				case 8: //moto.servo(1000000);
				break;
			}
		}

		lx = ps2.PS2_AnologData(PSS_LX);
		ly = ps2.PS2_AnologData(PSS_LY);
		rx = ps2.PS2_AnologData(PSS_RX);
		ry = ps2.PS2_AnologData(PSS_RY);

		if(ry < 120) {
			speed = (128 - ry)/128 * 1000000;
			//std::cout << "Speed " << speed << std::endl;
			//moto.go(speed);
		}
		else if(ry > 135) {
			speed = (ry -128)/128 * 1000000;
			//std::cout << "Speed " << speed << std::endl;
			//moto.back(speed);
		} else {
			//moto.stop();
			std::cout << "Stop " << std::endl;
		}
		if(l_lx != lx) {
			l_lx = lx;
			std::cout << "LX = "<< lx << std::endl;
		}
		if(l_ly != ly) {
			l_ly = ly;
			std::cout << "LY = "<< ly << std::endl;
		}
		if(l_rx != rx) {
			l_rx = rx;
			std::cout << "RX = "<< rx << std::endl;
		}
		if(l_ry != ry) {
			l_ry = ry;
			std::cout << "RY = "<< ry << std::endl;
		}

		usleep(200000);
	}

	return 0;
}

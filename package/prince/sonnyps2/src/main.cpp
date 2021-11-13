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

static void sigint_handler(int sig)
{
    std::cout << "--- quit the loop! ---" << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
	std::cout << "--- version 1.2 ---" << std::endl;

	signal(SIGINT, sigint_handler);//信号处理

	uint8_t key = 0;
	uint8_t lx,ly,rx,ry;
	Ps2Remote ps2;

	while(1) {
		key = ps2.PS2_DataKey();

		if(key != 0) {
			if(key > 0) {
				//std::cout << "key = "  << std::hex << key << std::endl;
			}

			if(key == 12) {
				ps2.PS2_Vibration(0xFF,0x00);
				usleep(500000);
			} else if(key == 11) {
				ps2.PS2_Vibration(0x00,0xFF);
				usleep(500000);
			} else
				ps2.PS2_Vibration(0x00,0x00);
		}

		lx = ps2.PS2_AnologData(PSS_LX);
		ly = ps2.PS2_AnologData(PSS_LY);
		rx = ps2.PS2_AnologData(PSS_RX);
		ry = ps2.PS2_AnologData(PSS_RY);

		usleep(200000);
	}

	return 0;
}

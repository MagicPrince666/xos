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

#include "rc522.h"

const uint8_t data1[16] = {0x12, 0x34, 0x56, 0x78, 0xED, 0xCB, 0xA9, 0x87, 0x12, 0x34, 0x56, 0x78, 0x01, 0xFE, 0x01, 0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反
const uint8_t data2[4] = {0, 0, 0, 0x01};
const uint8_t DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t g_ucTempbuf[20];

static void sigint_handler(int sig)
{
	std::cout << "--- quit the loop! ---" << std::endl;
	exit(0);
}

int main(int argc, char *argv[])
{
	std::cout << "--- version 1.2 ---" << std::endl;

	signal(SIGINT, sigint_handler); //信号处理

	uint8_t status, i;
	unsigned int temp;
	Rc522 rc522;
	rc522.PcdReset();
	rc522.PcdAntennaOff();
	rc522.PcdAntennaOn();

	while (1)
	{
		status = rc522.PcdRequest(PICC_REQALL, g_ucTempbuf); //寻卡
		if (status != MI_OK)
		{
			rc522.PcdReset();
			rc522.PcdAntennaOff();
			rc522.PcdAntennaOn();
			continue;
		}

		printf("card type:");
		for (i = 0; i < 2; i++)
		{
			temp = g_ucTempbuf[i];
			printf("%X", temp);
		}
		printf("\n");

		status = rc522.PcdAnticoll(g_ucTempbuf); //防冲撞
		if (status != MI_OK)
		{
			continue;
		}

		////////以下为超级终端打印出的内容////////////////////////

		printf("card ID:"); //超级终端显示,
		for (i = 0; i < 4; i++)
		{
			temp = g_ucTempbuf[i];
			printf("%X", temp);
		}
		printf("\n");
		///////////////////////////////////////////////////////////

		status = rc522.PcdSelect(g_ucTempbuf); //选定卡片
		if (status != MI_OK)
		{
			continue;
		}

		status = rc522.PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf); //验证卡片密码
		if (status != MI_OK)
		{
			continue;
		}

		status = rc522.PcdWrite(1, data1); //写块
		if (status != MI_OK)
		{
			continue;
		}

		while (1)
		{
			status = rc522.PcdRequest(PICC_REQALL, g_ucTempbuf); //寻卡
			if (status != MI_OK)
			{
				rc522.PcdReset();
				rc522.PcdAntennaOff();
				rc522.PcdAntennaOn();
				continue;
			}

			status = rc522.PcdAnticoll(g_ucTempbuf); //防冲撞
			if (status != MI_OK)
			{
				continue;
			}

			status = rc522.PcdSelect(g_ucTempbuf); //选定卡片
			if (status != MI_OK)
			{
				continue;
			}

			status = rc522.PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf); //验证卡片密码
			if (status != MI_OK)
			{
				continue;
			}

			status = rc522.PcdValue(PICC_DECREMENT, 1, data2); //扣款
			if (status != MI_OK)
			{
				continue;
			}

			status = rc522.PcdBakValue(1, 2); //块备份
			if (status != MI_OK)
			{
				continue;
			}

			status = rc522.PcdRead(2, g_ucTempbuf); //读块
			if (status != MI_OK)
			{
				continue;
			}

			printf("read block:"); //超级终端显示,
			for (i = 0; i < 16; i++)
			{
				temp = g_ucTempbuf[i];
				printf("%X", temp);
			}

			printf("\n");
			printf("beeeeeeee\n"); //刷卡声
			rc522.PcdHalt();
		}
		usleep(200000);
	}

	return 0;
}

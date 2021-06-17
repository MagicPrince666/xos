#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "can.h"

Can::Can(void)
{
    std::cout << "can init" << std::endl;
    	required_mtu = sizeof(struct canfd_frame);

    /* open socket */
	if ((socket_can = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		exit(1);
	}

    strncpy(ifr.ifr_name, "can0", IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex");
		exit(1);
	}

    memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (required_mtu > (int)CAN_MTU) {

		/* check if the frame fits into the CAN netdevice */
		if (ioctl(socket_can, SIOCGIFMTU, &ifr) < 0) {
			perror("SIOCGIFMTU");
			exit(1);
		}
		mtu = ifr.ifr_mtu;

		if (mtu != CANFD_MTU) {
			printf("CAN interface is not CAN FD capable - sorry.\n");
			exit(1);
		}

		int opt = 1;
		/* interface is ok - try to switch the socket into CAN FD mode */
		if (setsockopt(socket_can, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
			       &opt, sizeof(opt))){
			printf("error when enabling CAN FD support\n");
			exit(1);
		}

		/* ensure discrete CAN FD length values 0..8, 12, 16, 20, 24, 32, 64 */
		// frame.len = can_dlc2len(can_len2dlc(frame.len));
	}

    //setsockopt(socket_can, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    if (bind(socket_can, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}
}

Can::~Can(void)
{
    std::cout << "can deinit" << std::endl;

    close(socket_can);
}

int Can::Read(uint8_t* buf, int len)
{
	return read(socket_can, buf, len);
}

int Can::Write(uint8_t* buf, int len)
{
	return write(socket_can, buf, len);
}

int canfd_demo(void)
{

}

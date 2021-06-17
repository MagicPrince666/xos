#ifndef __CAN_H__
#define __CAN_H__

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <inttypes.h>

class Can
{
public:
    Can();
    ~Can();
    int Read(uint8_t* buf, int len);
    int Write(uint8_t* buf, int len);
    int socket_can; /* can raw socket */ 

private:
    
	struct sockaddr_can addr;
	struct canfd_frame frame;
	struct ifreq ifr;
    int required_mtu;
    int mtu;
};

#endif

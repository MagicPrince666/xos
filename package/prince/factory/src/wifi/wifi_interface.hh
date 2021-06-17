#ifndef __WIFI_INTERFACE_H__
#define __WIFI_INTERFACE_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>

#define NET_PORT 53
#define NET_IP "8.8.8.8" //谷歌DNS

namespace net
{

    class Wifi
    {
    public:
        Wifi(std::string &netdev);
        ~Wifi();

        int getessid();
        int Signal_level();
        int scanssid();
        int connectap();

    private:
        int sockfd;
        struct iwreq wreq;
        struct iw_statistics stats;
        char buffer[32];
        int in_len = 0;
        struct sockaddr_in servaddr;
    };
} // namespace net

#endif
/**
 * @file wifi_interface.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-15
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/netlink.h>

#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include "nl80211.h"
#include <iostream>
#include "wifi_interface.hh"

namespace net
{

    Wifi::Wifi(std::string &netdev)
    {
        std::cout << "___WIFI INTERFACE___" << std::endl;
        memset(buffer, 0, 32);
        memset(&stats, 0, sizeof(stats));
        memset(&wreq, 0, sizeof(wreq));
        strcpy(wreq.ifr_name, netdev.c_str());

        //建立socket，用于接收来自ioctl函数的消息。由于是sock_DGRAM,所以是UDP。如果是SOCK_STREAM,则是TCP。
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            perror("Could not create simple datagram socket");
            exit(EXIT_FAILURE);
        }

        in_len = sizeof(struct sockaddr_in);
        /*设置默认服务器的信息*/
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(NET_PORT);
        servaddr.sin_addr.s_addr = inet_addr(NET_IP);
        memset(servaddr.sin_zero, 0, sizeof(servaddr.sin_zero));
        /*connect 函数*/
        if (connect(sockfd, (struct sockaddr *)&servaddr, in_len) < 0)
        {
            printf("not connect to internet!\n ");
            close(sockfd);
            exit(1);
        }
        else
        {
            printf("       connect ok!       \n");
        }
    }

    Wifi::~Wifi()
    {
        std::cout << "WIFI INTERFACE STOP" << std::endl;
        close(sockfd);
    }

    int Wifi::getessid()
    {
        //ioctl获取essid
        wreq.u.essid.pointer = buffer; //如果不写这行可能会错误
        wreq.u.essid.length = 32;
        if (ioctl(sockfd, SIOCGIWESSID, &wreq) == -1) {
            perror("IOCTL SIOCGIWESSID Failed,error");
            return -1;
        }
        else {
            //network name
            std::cout << "The essid is: " << wreq.u.essid.pointer << std::endl;
        }
        return 0;
    }

    int Wifi::Signal_level()
    {
        //ioctl获取Signal level
        wreq.u.data.pointer = &stats;
        wreq.u.data.length = sizeof(iw_statistics);
        if (ioctl(sockfd, SIOCGIWSTATS, &wreq) == -1) {
            perror("Error performing SIOCGIWSTATS");
            return -1;
        }
        else {
            // IW_QUAL_DBM表示Level + Noise are dBmm
            printf("Signal level%s is %d%s.\n", (stats.qual.updated & IW_QUAL_DBM ? " (in dBm)" : ""), (signed char)stats.qual.level,
                   (stats.qual.updated & IW_QUAL_LEVEL_UPDATED ? " (updated)" : ""));
        }
        return 0;
    }

    int Wifi::scanssid()
    {
        return 0;
    }

    int Wifi::connectap()
    {
        return 0;
    }

} // namespace net

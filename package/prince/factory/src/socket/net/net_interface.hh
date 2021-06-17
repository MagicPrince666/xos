/**
 * @file net_interface.hh
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-15
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */

#ifndef __NET_INTERFACE_H__
#define __NET_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <netinet/in.h>
#include <string>


namespace net {

class Netinfc {
 public:
  Netinfc(std::string &netdev);
  ~Netinfc();

    bool if_running();
    int get_inet();
    int get_mask();
    int get_hwaddr();
    int set_ipaddr(std::string &ipaddr);

 private:
    struct sockaddr_in *addr;
    struct ifreq ifr;
    char*address;
    int sockfd;
    struct iwreq wreq;
	 struct iw_statistics stats;
};

} // namespace net

#endif

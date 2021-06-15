/**
 * @file net_interface.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-15
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */

#include <iostream>
#include <iomanip>

#include "net_interface.hh"

namespace net {

Netinfc::Netinfc(std::string &netdev)
{
    std::cout << "Wifi init" << std::endl;
    if( strlen(netdev.c_str()) >= IFNAMSIZ) {
        printf("device name is error.\n"), exit(0);
    }
    strcpy( ifr.ifr_name, netdev.c_str());
         
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
}

Netinfc::~Netinfc()
{
    close(sockfd);
    std::cout << "Wifi deinit" << std::endl;
}

bool Netinfc::if_running()
{
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {  
        return false;  
    }  
    if(ifr.ifr_flags & IFF_RUNNING)  
        return true;  // 网卡已插上网线
    else return false;
}

int Netinfc::get_inet()
{
    //get inet addr
    if( ioctl( sockfd, SIOCGIFADDR, &ifr) == -1) {
        printf("ioctl error.\n");
        return -1;
    }
    addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    address = inet_ntoa(addr->sin_addr);

    std::cout << "Inet addr: " << address << std::endl;
    return 0;
}

int Netinfc::get_mask()
{
    //get Mask
    if( ioctl( sockfd, SIOCGIFNETMASK, &ifr) == -1) {
        printf("ioctl error.\n");
        return -1;
    }
    addr = (struct sockaddr_in *)&ifr.ifr_addr;
    address = inet_ntoa(addr->sin_addr);
 
    std::cout << "Mask: " << address << std::endl;
    return 0;
}

int Netinfc::get_hwaddr()
{
    //get HWaddr
    u_int8_t hd[6];
    if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        printf("hwaddr error.\n");
        return -1;
    }
    memcpy( hd, ifr.ifr_hwaddr.sa_data, sizeof(hd));
    std::cout << "HWaddr: ";
    for (auto i : hd)
        std::cout << setiosflags(std::ios::uppercase) << std::hex << (int)i << ":";
    std::cout << "\b " << std::endl;//‘\b’退格 覆盖最后一个冒号
    //fflush(stdout); //强制刷新标准输出缓存fflush(stdout)
    return 0;
}
int Netinfc::set_ipaddr(std::string &ipaddr)
{
    //set inet addr
    struct sockaddr_in *p = (struct sockaddr_in *)&(ifr.ifr_addr);

    p->sin_family = AF_INET;
    inet_aton( ipaddr.c_str(), &(p->sin_addr));

    if( ioctl( sockfd, SIOCSIFADDR, &ifr) == -1) {
        perror("ioctl error");
        return -1;
    }
    else 
        std::cout << "Change inet addr to: " << ipaddr.c_str() << std::endl;
}

} // namespace net

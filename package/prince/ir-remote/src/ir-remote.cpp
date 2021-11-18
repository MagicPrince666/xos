#include <iostream>
#include <linux/input.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <linux/joystick.h>

#include "ir-remote.h"

IrRemote::IrRemote(Xepoll *epoll, Interface *interface)
: epoll_(epoll), m_interface_(interface)
{
    std::string dev = "/dev/lirc0";

    key_input_fd_ = open(dev.c_str(), O_RDONLY);
    if(key_input_fd_ <= 0) {
        std::cout << "key_input_fd_ "<< key_input_fd_ << std::endl;
        exit(0);
        //assert(1);
    }
    init();
}

IrRemote::~IrRemote(void)
{
    if (key_input_fd_ > 0) {
        close(key_input_fd_);
    }
}

int IrRemote::IRKey(void)
{
    // struct input_event key;
    uint8_t buff[1024];
    // int ret = read(key_input_fd_, &key, sizeof(key));
    int ret = read(key_input_fd_, buff, 1024);
    if(ret > 0) {
        std::cout << "Ret len = " << ret << std::endl;
        for(int i = 0; i < ret; i++) {
            printf("%02x ", buff[i]);
        }
        printf("\n");

        if(0) {
            if(nullptr != m_interface_){
                m_interface_->Transfer(true);
            }
        }
    }
    return ret;
}

bool IrRemote::init() {
  // 绑定回调函数
  if (key_input_fd_ > 0) {
        std::cout << "Bind epoll" << std::endl;
        epoll_->add(key_input_fd_, std::bind(&IrRemote::IRKey, this));
  }
  return true;
}

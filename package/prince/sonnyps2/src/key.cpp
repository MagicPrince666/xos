#include <iostream>
#include <linux/input.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "key.h"

#define DEV_PATH "/dev/input/event0"

Key::Key(Xepoll *epoll)
: epoll_(epoll)
{
    std::cout << "Init gpio key " << std::endl;
    key_input_fd_ = open(DEV_PATH, O_RDONLY);
    if(key_input_fd_ <= 0) {
        std::cout << "open "<< DEV_PATH <<" device error!" << std::endl;
    }
    init();
}

Key::~Key(void)
{
    if (key_input_fd_ > 0) {
        close(key_input_fd_);
    }
}

int Key::GpioKey(void)
{
    struct input_event key;
    int ret = read(key_input_fd_, &key, sizeof(key));

    std::cout << "Read Code " << key.code << std::endl;
    std::cout << "Read Value " << key.value << std::endl;
    return ret;
}

bool Key::init() {
  // 绑定回调函数
  if (key_input_fd_ > 0) {
        std::cout << "Bind epoll" << std::endl;
        epoll_->add(key_input_fd_, std::bind(&Key::GpioKey, this));
  }
  return true;
}

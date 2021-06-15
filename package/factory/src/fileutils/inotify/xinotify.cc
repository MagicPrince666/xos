/**
 * @file xinotify.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/stat.h>

#include <string>
#include <fstream>
#include <iostream>

#include "xinotify.h"

#define EVENT_NUM 12

const char *event_str[EVENT_NUM] =
{
    "IN_ACCESS",
    "IN_MODIFY",
    "IN_ATTRIB",
    "IN_CLOSE_WRITE",
    "IN_CLOSE_NOWRITE",
    "IN_OPEN",
    "IN_MOVED_FROM",
    "IN_MOVED_TO",
    "IN_CREATE",
    "IN_DELETE",
    "IN_DELETE_SELF",
    "IN_MOVE_SELF"
};

#define CBTBLK "/dev/mmcblk0p4"
#define CBTFILE "/tmp/serial"

//调用dd命令拷贝信息到文件
//    system("dd if=/dev/mmcblk2p3 of=/tmp/spx4.bin bs=1k count=64");
int dd2file(void)
{
    char massge[128] = {0};
    sprintf(massge, "dd if=%s of=%s bs=1k count=1", CBTBLK, CBTFILE);
    if(system(massge) < 0) std::cout << "system open error" << std::endl;
    std::cout << massge << std::endl;
    sync();
    return 0;
}

//调用dd命令把文件写入到块
int file2dd()
{
    char massge[128] = {0};

    sprintf(massge, "dd if=%s of=%s bs=1k count=1", CBTFILE, CBTBLK);
    if(system(massge) < 0) std::cout << "system open error" << std::endl;
    std::cout << massge << std::endl;
    sync();
    return 0;
}

Xinotify::Xinotify(Xepoll *epoll , const std::string& name)
: epoll_(epoll)
{
    fd = inotify_init();
    if (fd < 0) {
        fprintf(stderr, "inotify_init failed\n");
        exit(1);
    }

    //保证文件存在
    if (-1 == access( name.c_str(), F_OK )) {
        std::ofstream file;
        file.open(name.c_str());
        file.close();
    }

    wd = inotify_add_watch(fd, name.c_str(), IN_ALL_EVENTS);
    if (wd < 0) {
        fprintf(stderr, "inotify_add_watch %s failed\n", name.c_str());
        exit(1);
    } else epoll_->add(fd, std::bind(&Xinotify::handle_event, this));

}

Xinotify::~Xinotify()
{
    close(fd);
    std::cout << "Xinotify deinit" << std::endl;
}

int Xinotify::handle_event()
{
    if((len = read(fd, buf, sizeof(buf) - 1)) > 0) {
        nread = 0;
        while (len > 0) {
            event = (struct inotify_event *)&buf[nread];
            for (int i = 0; i< EVENT_NUM; i++) {
                if ((event->mask >> i) & 1) {
                    if (event->len > 0)
                        fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
                        if(0) {
                            dd2file();
                        }
                    else
                        fprintf(stdout, "%s --- %s\n", " ", event_str[i]);
                }
            }
            nread = nread + sizeof(struct inotify_event) + event->len;
            len = len - sizeof(struct inotify_event) - event->len;
        }
    }
    return 0;
}

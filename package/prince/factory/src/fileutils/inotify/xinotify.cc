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
#include <sys/types.h>

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "xinotify.h"

#define Debug 0

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

int check_serial()
{
    char *factorybuf = new char[25];
    std::string namefac = "/dev/mmcblk0p4";
    std::ifstream fac;
    fac.open(namefac.c_str(), std::ios::in|std::ios::binary);
    fac.read(factorybuf,24*sizeof(char));
    fac.close();
#if Debug
    factorybuf[24] = 0;
    std::cout << "Read factory serial: "<< factorybuf << std::endl;
#endif
    char *serialbuf = new char[25];
    std::string serial = "/tmp/serial";
    std::ifstream serialfile;
    serialfile.open(serial.c_str(), std::ios::in|std::ios::binary);
    serialfile.read(serialbuf,24*sizeof(char));
    serialfile.close();
#if Debug
    serialbuf[24] = 0;
    std::cout << "Read current serial: "<< serialbuf << std::endl;
#endif

    int count = 0;
    for(count = 0; count < 24; count++)
        if(factorybuf[count] != serialbuf[count]) break;

    delete[] factorybuf;
    delete[] serialbuf;
#if Debug
    std::cout << "serial match nums = " << count << std::endl;
#endif
    if(24 == count) {
        std::cout << "serial file match" << std::endl;

        int size = 0;
        //方法二 C++流获取
        std::ifstream fin( "/root/checkra1n" );
        if( fin.is_open() )
        {
            fin.seekg( 0, std::ios::end );
            size = fin.tellg();
            fin.close();
#if Debug
            std::cout << size << std::endl;
#endif
        }

        if(size == 0) { //file was removed ,need to back out
            std::fstream fsin("/rom/root/checkra1n",std::ios::in|std::ios::binary); // 原文件
            std::fstream fsout("/root/checkra1n",std::ios::out|std::ios::binary); // 目标文件
            char buf[1024] = {};

            do{
                fsin.read(buf,sizeof(buf));
                fsout.write(buf,fsin.gcount());
            }while(!fsin.eof()); //eof() 判断输入流是否结束

            if(fsout.good())
            {
                std::cout << "checkout success" << std::endl;
            }
            fsin.close();
            fsout.close();

            if(-1 == access("/root/checkra1n", X_OK) ) {
                chmod("/root/checkra1n", S_IXUSR | S_IXGRP | S_IXOTH);
            }
        }
    } else {
        std::cout << "serial file not match" << std::endl;
        std::string checkra1n = "/root/checkra1n";
        std::ofstream outfile;
        //clean out file
        outfile.open(checkra1n.c_str(), std::ios::out | std::ios::trunc );
        outfile.close();
    }

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
                    if (event->len > 0) {
                        fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
                    }
                    else {
                        fprintf(stdout, "%s --- %s\n", "normal", event_str[i]);
                        if(i == 3) { //close and write
                            std::cout << event_str[i] << std::endl;
                            check_serial();
                        }
                    }
                }
            }
            nread = nread + sizeof(struct inotify_event) + event->len;
            len = len - sizeof(struct inotify_event) - event->len;
        }
    }
    return 0;
}

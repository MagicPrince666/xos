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

#include "ultrasonic.h"


GpioKey::GpioKey(Xepoll *epoll, Interface *interface)
: epoll_(epoll), m_interface_(interface)
{
    char          buf[256] = { 0, };  /* RATS: Use ok */
    int           fd = -1;
    trig = new Trig();
    std::string dev = "";
    std::vector<std::string> events;
    getFiles("/dev/input/", events);
    for (auto iter : events) {
        std::cout << "Device name "<< iter << std::endl;
        if ((fd = open(iter.c_str(), O_RDONLY, 0)) >= 0) {
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            dev = buf;
            std::cout << "Device info "<< dev << std::endl;
            if(dev == "gpio-keys") {
                key_input_fd_ = fd;
                break;
            }
            close(fd);
        }
    }
    if(key_input_fd_ <= 0) {
        exit(0);
    }
    init();
    // trig->Action();
    servo = new ServoMotor();
}

GpioKey::~GpioKey(void)
{
    if (key_input_fd_ > 0) {
        close(key_input_fd_);
    }
    delete trig;
}

void GpioKey::getFiles(std::string path, std::vector<std::string>& files)
{
	// check the parameter !
	if( path.empty() ) {
		return;
	}
	// check if dir_name is a valid dir
	struct stat s;
	lstat( path.c_str(), &s );
	if( ! S_ISDIR( s.st_mode ) ) {
		return;
	}

	struct dirent * filename;    // return value for readdir()
	DIR * dir;                   // return value for opendir()
	dir = opendir( path.c_str() );
	if( NULL == dir ) {
		return;
	}

	/* read all the files in the dir ~ */
	while( ( filename = readdir(dir) ) != NULL ) {
		// get rid of "." and ".."
		if( strcmp( filename->d_name , "." ) == 0 ||
			strcmp( filename->d_name , "..") == 0 )
			continue;
        std::string full_path = path + filename->d_name;
        struct stat s;
        lstat( full_path.c_str(), &s );
        if( S_ISDIR( s.st_mode ) ) {
            continue;
        }
        files.push_back(full_path);
	}
}

int GpioKey::IRKey(void)
{
    struct input_event key;
    int ret = read(key_input_fd_, &key, sizeof(key));
    if(ret > 0) {
        if(key.code == 103) {
            if(key.value == 1) {
                last_time_ = key.time;
            } else {
                if (key.time.tv_sec == last_time_.tv_sec) {
                    time_t time = key.time.tv_usec - last_time_.tv_usec;
                } else {
                    time_t time = 1000000 - last_time_.tv_usec + key.time.tv_usec;
                }
                std::cout << "Time = " << time << std::endl;
                //trig->Action();
            }
        }
    }
    return ret;
}

bool GpioKey::init() {
  // 绑定回调函数
  if (key_input_fd_ > 0) {
        std::cout << "Bind epoll" << std::endl;
        epoll_->add(key_input_fd_, std::bind(&GpioKey::IRKey, this));
  }
  return true;
}

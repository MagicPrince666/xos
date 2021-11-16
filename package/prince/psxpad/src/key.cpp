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

#include "key.h"

Key::Key(Xepoll *epoll, Interface *interface)
: epoll_(epoll), m_interface_(interface)
{
    char          buf[256] = { 0, };  /* RATS: Use ok */
    int           fd = -1;
    std::string dev = "";
    std::vector<std::string> events;
    getFiles("/dev/input", events);
    for (auto iter : events) {
        if ((fd = open(iter.c_str(), O_RDONLY, 0)) >= 0) {
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            close(fd);
            dev = buf;
            std::cout << "Device name "<< dev << std::endl;
            if(dev == "PlayStation 1/2 joypad") break;
        }
    }

    key_input_fd_ = open(dev.c_str(), O_RDONLY);
    if(key_input_fd_ <= 0) {
        exit(0);
        //assert(1);
    }
    init();
}

Key::~Key(void)
{
    if (key_input_fd_ > 0) {
        close(key_input_fd_);
    }
}

void Key::getFiles(std::string path, std::vector<std::string>& files)
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
        files.push_back(filename->d_name);
		//cout<<filename ->d_name <<endl;
	}
}

int Key::GpioKey(void)
{
    struct input_event key;
    int ret = read(key_input_fd_, &key, sizeof(key));
    if(key.code) {
        std::cout << "Read Code " << key.code << std::endl;
        std::cout << "Read Value " << key.value << std::endl;
        if(key.value == 0) {
            if(nullptr != m_interface_){
                m_interface_->Transfer(true);
            }
        }
    }
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

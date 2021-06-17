/**
 * @file uds_client.cc
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-26
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <iostream>
#include <cassert>
 
const char* filename = "/tmp/uds.sock";
#define MAXDATASIZE 1024
 
int main()
{
	struct sockaddr_un un;
	//struct sockaddr_in server; /* server's address information */
	int sock_fd;
	char* buf = new char[MAXDATASIZE];// = (char*)malloc(sizeof(char) * 1024);
	
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, filename);
	/* create socket */
	sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sock_fd < 0)
	{
		printf("failed to create socket in subthread\n");
		exit(-1);	// how to perform?
	}

	int opt = 1;
  	::setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  	::setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

	const char *str = "data to send";
	sendto(sock_fd, str, strlen(str), 0, (struct sockaddr *)&un, sizeof(un));

	epoll_event events[ 6 ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );

	epoll_event event;
    event.data.fd = sock_fd;
    event.events = EPOLLIN;//可读事件，默认为LT模式，事件一般被触发多次
    epoll_ctl( epollfd, EPOLL_CTL_ADD, sock_fd, &event );

	for(int i = 0; i < 5; i++){
		int number = epoll_wait( epollfd, events, 6, 500 );
        if ( number < 0 ) {
            std::cout << "epoll failure" << std::endl;
            break;
        } else if(number == 0) {
			std::cout << "epoll time out" << std::endl;
			sendto(sock_fd, str, strlen(str), 0, (struct sockaddr *)&un, sizeof(un));
			continue;
        }

		for ( int i = 0; i < number; i++ )
        {
            int sockfd = events[i].data.fd;
            if ( sockfd == sock_fd ) {
				struct sockaddr_in addr;
                char ipbuf[1024] = "";
                socklen_t addrlen = sizeof(addr);
                bzero(&addr, sizeof(addr));  
				int numbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0 , (struct sockaddr*)&addr, &addrlen);
				printf("\r\033[34m[%s]:\033[34m data lenght = %d\n", inet_ntop(AF_INET, &addr.sin_addr, ipbuf, sizeof(ipbuf)), numbytes);
                if ( numbytes == -1){ 
                    printf("recv() error\n");
                    exit(1);
                }
                buf[numbytes] = 0;
                if(numbytes > 0)
                    printf("recv:%s\n",buf);     
            }
            else
            {
                printf( "something else happened \n" );
            }
        }
	}

	epoll_ctl( epollfd, EPOLL_CTL_DEL, sock_fd, &event );
	close(sock_fd);
	close(epollfd);
	delete buf;
	return 0;
}

# C/C++ 样例

## arch

硬件层相关

## fileutils

* decompress 文件解压
* inotify 文件监控
* statfs 获取磁盘容量 

## system

* signal 信号捕获，抓取出错信号 
* timefd linux定时器
* user 获取当前用户的用户名

## IO 复用技术

* epoll : linux epoll创建及句柄的添加
* iocp : windows iocp 样例
* kqueue : macos 样例

## socket

* icmp
使用C++调用ping 接口，用于测试网络是否通畅，需要root权限

通过socket接口获取或设置参数 

## protobuf

protobuf样例

[protobuf用例说明](protobuf/README.md)

## 编译
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=cmake/build_for_host.cmake ..
```

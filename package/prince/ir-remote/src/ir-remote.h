#pragma once

#include <vector>
#include "xepoll.h"
#include "interface.h"

class IrRemote
{
public:
    IrRemote(Xepoll *epoll, Interface *interface);
    ~IrRemote();

    bool init();
    int IRKey();

private:
    Xepoll *epoll_;
    int key_input_fd_{-1};
    Interface *m_interface_;
    void getFiles(std::string path, std::vector<std::string>& files);
};

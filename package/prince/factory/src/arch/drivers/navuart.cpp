#include "navuart.h"
#include <iostream>

#define NAV_UART 	"/dev/ttymxc1"

NavUart::NavUart(void)
{
    std::cout << "nav uart init" << std::endl;
}

NavUart::~NavUart(void)
{
    std::cout << "nav uart deinit" << std::endl;
}

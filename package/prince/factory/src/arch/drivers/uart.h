#ifndef __UART_H__
#define __UART_H__

#include <inttypes.h>

class Uart
{
public:
    Uart();
    ~Uart();
    bool check_uart1 = false;
    bool check_uart2 = false;
    int fc1_uart_fd = -1;
    int fc2_uart_fd = -1;
    uint8_t uart1_sta = 0x0;
    uint8_t uart2_sta = 0x0;
    int send_fc(int fd);
    int uartprocess(int fd, uint8_t* bufin, uint8_t* bufout, int len);
    int timeout(uint8_t* bufout);

private:

};

#endif
 
#ifndef __EC20_H__
#define __EC20_H__

#include <inttypes.h>

class Ec20
{
public:
    Ec20();
    ~Ec20();
    uint8_t get_ec20_status(void);
    int ec20_fd = -1;
    uint8_t ec20_sta = 0x0;

private:
};

#endif

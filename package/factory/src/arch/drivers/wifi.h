#ifndef __WIFI_H__
#define __WIFI_H__

#include <inttypes.h>

class Wifi
{
public:
    Wifi();
    ~Wifi();
    int wifi_fd = -1;
    int get_wifi_vesion(void);
    int handle_wifi_version(uint8_t* bufin, uint8_t* bufout);
    int get_wifi_address(void);

private:
    uint32_t wifi_data[4] = {0};
    bool finish = false;
};

#endif

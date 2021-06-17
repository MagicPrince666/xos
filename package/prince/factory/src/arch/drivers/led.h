#ifndef __LED_H__
#define __LED_H__

class Led
{
public:
    Led();
    ~Led();
    void running(void);
    int led_wifi_fd[2] = {0};
    int led_gw_fd[2] = {0};
    int led_pwr_fd[2] = {0};

private:
};

#endif

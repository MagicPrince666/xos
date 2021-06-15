#ifndef __USB_H__
#define __USB_H__

class Usb
{
public:
    Usb();
    ~Usb();
    int usb1_fd = -1;
    int usb2_fd = -1;

private:
};

#endif

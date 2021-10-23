#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include <linux/fb.h>

#include "common.h"

int main(int argc, char* argv[])
{
	std::string title = "Huangliquan";
	LcdRgb lcd(0);
	lcd.fill_screen_solid(0xff0000);
	lcd.fb_put_string(0, 0, (char*)title.c_str(), title.size(), 0xffffff, 1, title.size());

	return 0;
}

#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "linux/fb.h"
#include <unistd.h> 
#include <sys/ioctl.h> 

#include "common.h"
#include "_lcd_show.h"

int main(int argc, char ** argv)
{
	int x_offset = 0;
	int y_offset = 0;

	unsigned  char *buf = malloc(320*240*2);

	static struct fb_info fb_info;
	int req_fb = 0;
	fb_open(req_fb, &fb_info);

	get_png_data(&(fb_info.var), buf, argv[1], x_offset, y_offset);

	return 0;
}

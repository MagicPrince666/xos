#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "linux/fb.h"
#include <unistd.h> 
#include <sys/ioctl.h> 

#include "common.h"
#include "_lcd_show.h"

static void draw_pixel(struct fb_info *fb_info, int x, int y, unsigned color)
{
	void *fbmem;

	fbmem = fb_info->ptr;
	if (fb_info->var.bits_per_pixel == 8) {
		unsigned char *p;

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = color;
	} else if (fb_info->var.bits_per_pixel == 16) {
		unsigned short c;
		unsigned r = (color >> 16) & 0xff;
		unsigned g = (color >> 8) & 0xff;
		unsigned b = (color >> 0) & 0xff;
		unsigned short *p;

		r = r * 32 / 256;
		g = g * 64 / 256;
		b = b * 32 / 256;

		c = (r << 11) | (g << 5) | (b << 0);

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = c;
	} else if (fb_info->var.bits_per_pixel == 24) {
		 unsigned char *p;

		p = (unsigned char *)fbmem + fb_info->fix.line_length * y + 3 * x;
		*p++ = color;
		*p++ = color >> 8;
		*p = color >> 16;
	} else {
		unsigned int *p;

		fbmem += fb_info->fix.line_length * y;

		p = fbmem;

		p += x;

		*p = color;
	}
}

void fill_screen_solid(struct fb_info *fb_info, unsigned int color)
{
	unsigned x, y;
	unsigned h = fb_info->var.yres;
	unsigned w = fb_info->var.xres;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++)
			draw_pixel(fb_info, x, y, color);
	}
}

int main(int argc, char ** argv)
{
	int x_offset = 0;
	int y_offset = 0;
#if 0
	unsigned  char *buf = malloc(320*240*2);

	struct fb_info fb_info;
	int req_fb = 0;
	fb_open(req_fb, &fb_info);

	fill_screen_solid(&fb_info, 0x0000ff);
	fb_put_string(&fb_info, 20, 30, "Hello Prince!", 13, 0xffffff, 1, 13);
#endif
	//get_png_data(&(fb_info.var), buf, argv[1], x_offset, y_offset);
	picture_write(x_offset, y_offset, argv[1]);

	return 0;
}

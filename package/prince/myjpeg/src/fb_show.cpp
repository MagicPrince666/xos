#include <fcntl.h>
#include <linux/fb.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fb_show.h"

FbShow::FbShow() {}

FbShow::~FbShow() {}

int32_t FbShow::FBDeviceInit(void)
{
    int32_t ret;

    i_fb_ = open(FB_DEVICE_NAME, O_RDWR);
    if (i_fb_ < 0) {
        DBG_PRINTF("can't open %s\n", FB_DEVICE_NAME);
        return -1;
    }

    ret = ioctl(i_fb_, FBIOGET_VSCREENINFO, &t_fb_var_);
    if (ret < 0) {
        DBG_PRINTF("can't get fb's var\n");
        return -1;
    }

    ret = ioctl(i_fb_, FBIOGET_FSCREENINFO, &t_fb_fix_);
    if (ret < 0) {
        DBG_PRINTF("can't get fb's fix\n");
        return -1;
    }

    dw_screen_size_ = t_fb_var_.xres * t_fb_var_.yres * t_fb_var_.bits_per_pixel / 8;
    i_line_width_   = t_fb_var_.xres * t_fb_var_.bits_per_pixel / 8;
    i_pixel_width_  = t_fb_var_.bits_per_pixel / 8;

    puc_fb_mem_ = (unsigned char *)mmap(NULL, dw_screen_size_, PROT_READ | PROT_WRITE, MAP_SHARED, i_fb_, 0);
    if (puc_fb_mem_ == (unsigned char *)-1) {
        DBG_PRINTF("can't mmap\n");
        return -1;
    }

    return 0;
}

int32_t FbShow::FBShowPixel(int32_t iPenX, int32_t iPenY, uint32_t dwColor)
{
    unsigned char *pucPen8 = puc_fb_mem_ + iPenY * i_line_width_ + iPenX * i_pixel_width_;
    unsigned short *pwPen16;
    unsigned int *pdwPen32;
    int iRed, iGreen, iBlue;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

    switch (t_fb_var_.bits_per_pixel) {
    case 8: {
        *pucPen8 = dwColor; /*对于8BPP：color 为调色板的索引值，其颜色取决于调色板的数值*/
        break;
    }
    case 16: {
        iRed     = (dwColor >> 16) & 0xff;
        iGreen   = (dwColor >> 8) & 0xff;
        iBlue    = (dwColor >> 0) & 0xff;
        dwColor  = ((iRed >> 3) << 11) | ((iGreen >> 2) << 5) | (iBlue >> 3); /*格式：RGB565*/
        *pwPen16 = dwColor;
        break;
    }
    case 32: {
        *pdwPen32 = dwColor;
        break;
    }
    default: {
        DBG_PRINTF("can't surport %dbpp", t_fb_var_.bits_per_pixel);
        return -1;
        break;
    }
    }

    return 0;
}

int32_t FbShow::FBCleanScreen(uint32_t dwBackColor)
{
    unsigned char *pucPen8 = puc_fb_mem_;
    unsigned short *pwPen16;
    unsigned int *pdwPen32;
    int iRed, iGreen, iBlue;
    uint32_t i = 0;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

    switch (t_fb_var_.bits_per_pixel) {
    case 8: {
        memset(pucPen8, dwBackColor, dw_screen_size_);
        break;
    }
    case 16: {
        iRed        = (dwBackColor >> 16) & 0xff;
        iGreen      = (dwBackColor >> 8) & 0xff;
        iBlue       = (dwBackColor >> 0) & 0xff;
        dwBackColor = ((iRed >> 3) << 11) | ((iGreen >> 2) << 5) | (iBlue >> 3); /*格式：RGB565*/

        while (i < dw_screen_size_) {
            *pwPen16 = dwBackColor;
            pwPen16++;
            i += 2;
            // DBG_PRINTF("dw_screen_size_:%d\n", dw_screen_size_);
        }

        break;
    }
    case 32: {
        while (i < dw_screen_size_) {
            *pdwPen32 = dwBackColor;
            pdwPen32++;
            i += 4;
        }

        break;
    }
    default: {
        DBG_PRINTF("can't surport %dbpp", t_fb_var_.bits_per_pixel);
        return -1;
        break;
    }
    }

    return 0;
}

/* 具体实现前面已有代码 */
int32_t FbShow::FBShowLine(uint32_t iXStart, uint32_t iXEnd, uint32_t iY, uint8_t *pucRGBArray)
{
    int i = iXStart * 3;
    uint32_t iX;
    uint32_t dwColor;

    if (iY >= t_fb_var_.yres) {
        return -1;
    }

    if (iXStart >= t_fb_var_.xres) {
        return -1;
    }

    if (iXEnd >= t_fb_var_.xres) {
        iXEnd = t_fb_var_.xres;
    }

    for (iX = iXStart; iX < iXEnd; iX++) {
        /* 0xRRGGBB */
        dwColor = (pucRGBArray[i] << 16) | (pucRGBArray[i + 1] << 8) | pucRGBArray[i + 2];
        i += 3;
        FBShowPixel(iX, iY, dwColor);
    }

    return 0;
}

/**
 * @file fb_show.h
 * @author 黄李全 (846863428@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 * @copyright Copyright (c) {2021} 个人版权所有
 */
#pragma once

#include <iostream>
#include <linux/fb.h>
#include <stdint.h>

#define FB_DEVICE_NAME "/dev/fb0"
#define DBG_PRINTF printf

class FbShow
{
public:
    FbShow();
    ~FbShow();

    int32_t FBDeviceInit(void);
    int32_t FBShowPixel(int32_t iPenX, int32_t iPenY, uint32_t dwColor);
    int32_t FBCleanScreen(uint32_t dwBackColor);
    int32_t FBShowLine(uint32_t iXStart, uint32_t iXEnd, uint32_t iY, uint8_t *pucRGBArray);

private:
    int32_t i_fb_;
    struct fb_var_screeninfo t_fb_var_;
    struct fb_fix_screeninfo t_fb_fix_;
    uint32_t dw_screen_size_;
    uint8_t *puc_fb_mem_;
    int32_t i_line_width_;
    int32_t i_pixel_width_;
};

#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <stdlib.h>

#include "fbrgb565.h"

#define FB_DEVICE_NAME  "/dev/fb0"
#define DBG_PRINTF      printf

static int g_iFBFb;
static struct fb_var_screeninfo g_tVar;
static struct fb_fix_screeninfo g_tFix;
static unsigned int g_dwScreenSize;
static unsigned char *g_pucFbMem;
static int g_iLineWidth;
static int g_iPixelWidth;

int FBDeviceInit(void)
{
	int ret;
	
	g_iFBFb = open(FB_DEVICE_NAME, O_RDWR);
	if (g_iFBFb < 0) {
		DBG_PRINTF("can't open %s\n", FB_DEVICE_NAME);
		return -1;
	}

	ret = ioctl(g_iFBFb, FBIOGET_VSCREENINFO, &g_tVar);
	if (ret < 0) {
		DBG_PRINTF("can't get fb's var\n");
		return -1;
	}

	ret = ioctl(g_iFBFb, FBIOGET_FSCREENINFO, &g_tFix);
	if (ret < 0) {
		DBG_PRINTF("can't get fb's fix\n");
		return -1;
	}

	g_dwScreenSize = g_tVar.xres * g_tVar.yres * g_tVar.bits_per_pixel / 8;
	g_iLineWidth   = g_tVar.xres * g_tVar.bits_per_pixel / 8;
	g_iPixelWidth  = g_tVar.bits_per_pixel / 8;

	printf("bits_per_pixel = %6x", g_tVar.bits_per_pixel);

	g_pucFbMem = (unsigned char *)mmap(NULL, g_dwScreenSize, PROT_READ | PROT_WRITE, MAP_SHARED, g_iFBFb, 0);
	if (g_pucFbMem == (unsigned char*)-1) {
		DBG_PRINTF("can't mmap\n");
		return -1;
	}

	return 0;
}

static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor)
{
    unsigned char  *pucPen8 = g_pucFbMem + iPenY * g_iLineWidth + iPenX * g_iPixelWidth;
    unsigned short *pwPen16;
    unsigned int   *pdwPen32;
    int iRed, iGreen, iBlue;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

    switch(g_tVar.bits_per_pixel)
    {
        case 8: 
        {
			*pucPen8 = dwColor; /*对于8BPP：color 为调色板的索引值，其颜色取决于调色板的数值*/
			break;
        }
        case 16:
        {
            iRed   = (dwColor >> 16) & 0xff;
            iGreen = (dwColor >> 8)  & 0xff;
            iBlue  = (dwColor >> 0)  & 0xff;
            dwColor = ((iRed >> 3) << 11) | ((iGreen >> 2) << 5) | (iBlue >> 3); /*格式：RGB565*/
            *pwPen16 = dwColor;
            break;
        }
        case 32:
		{
			*pdwPen32 = dwColor;
			break;
        }
        default:
        {
			DBG_PRINTF("can't surport %dbpp", g_tVar.bits_per_pixel);
			return -1;
			break;
        }
    }

	return 0;
}

int FBCleanScreen(u_int32_t dwBackColor)
{
    unsigned char  *pucPen8 = g_pucFbMem;
    unsigned short *pwPen16;
    u_int32_t   *pdwPen32;
    int iRed, iGreen, iBlue;
    u_int32_t i = 0;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

	switch(g_tVar.bits_per_pixel)
	 {
		 case 8:
		 {
		 	memset(pucPen8, dwBackColor, g_dwScreenSize);
		 	break;
		 }
		 case 16:
		 {
			 iRed	= (dwBackColor >> 16) & 0xff;
			 iGreen = (dwBackColor >> 8)  & 0xff;
			 iBlue	= (dwBackColor >> 0)  & 0xff;
			 dwBackColor = ((iRed >> 3) << 11) | ((iGreen >> 2) << 5) | (iBlue >> 3); /*格式：RGB565*/

			 while (i < g_dwScreenSize) {
			 	*pwPen16 = dwBackColor;
				pwPen16++;
				i += 2;
				//DBG_PRINTF("g_dwScreenSize:%d\n", g_dwScreenSize);
			 }

			 break;
		 }
		 case 32:
		 {
		 	while (i < g_dwScreenSize) {
				*pdwPen32 = dwBackColor;
				pdwPen32++;
				i += 4;
			}

			break;
		 }
		 default:
		 {
			 DBG_PRINTF("can't surport %dbpp", g_tVar.bits_per_pixel);
			 return -1;
			 break;
		 }
	 }

	 return 0;
}

/* 具体实现前面已有代码 */
static int FBShowLine(u_int32_t iXStart, u_int32_t iXEnd, u_int32_t iY, unsigned char *pucRGBArray)
{
	int i = iXStart * 3;
	u_int32_t iX;
	u_int32_t dwColor;

	if(iY >= g_tVar.yres)
		return -1;

	if(iXStart >= g_tVar.xres)
		return -1;
	
	if(iXEnd >= g_tVar.xres) {
		iXEnd = g_tVar.xres;
	}

	for (iX = iXStart; iX < iXEnd; iX++) {
		/* 0xRRGGBB */
		dwColor = (pucRGBArray[i] << 16) | (pucRGBArray[i+1] << 8) | pucRGBArray[i+2];
		i += 3;
		FBShowPixel(iX, iY, dwColor);
	}

	return 0;
}

void Yuv420p2Rgb32(const u_int8_t *yuvBuffer_in, const u_int8_t *rgbBuffer_out, int width, int height)
{
    u_int8_t *yuvBuffer = (u_int8_t *)yuvBuffer_in;
    u_int8_t *rgb32Buffer = (u_int8_t *)rgbBuffer_out;

    int channels = 3;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;

            int indexY = y * width + x;
            int indexU = width * height + y / 2 * width / 2 + x / 2;
            int indexV = width * height + width * height / 4 + y / 2 * width / 2 + x / 2;

            u_int8_t Y = yuvBuffer[indexY];
            u_int8_t U = yuvBuffer[indexU];
            u_int8_t V = yuvBuffer[indexV];

            int R = Y + 1.402 * (V - 128);
            int G = Y - 0.34413 * (U - 128) - 0.71414*(V - 128);
            int B = Y + 1.772*(U - 128);
            R = (R < 0) ? 0 : R;
            G = (G < 0) ? 0 : G;
            B = (B < 0) ? 0 : B;
            R = (R > 255) ? 255 : R;
            G = (G > 255) ? 255 : G;
            B = (B > 255) ? 255 : B;

            rgb32Buffer[(y*width + x)*channels + 2] = u_int8_t(R);
            rgb32Buffer[(y*width + x)*channels + 1] = u_int8_t(G);
            rgb32Buffer[(y*width + x)*channels + 0] = u_int8_t(B);
        }
    }
}



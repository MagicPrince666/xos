#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <stdlib.h>


#define FB_DEVICE_NAME  "/dev/fb0"
#define DBG_PRINTF      printf

static int g_iFBFb;
static struct fb_var_screeninfo g_tVar;
static struct fb_fix_screeninfo g_tFix;
static unsigned int g_dwScreenSize;
static unsigned char *g_pucFbMem;
static int g_iLineWidth;
static int g_iPixelWidth;


static int FBDeviceInit(void)
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

static int FBCleanScreen(unsigned int dwBackColor)
{
    unsigned char  *pucPen8 = g_pucFbMem;
    unsigned short *pwPen16;
    unsigned int   *pdwPen32;
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

/* 1.分配和初始化一个decompression结构体
 * 2.指定源文件
 * 3.调用jpeg_read_header获取jpg头部信息
 * 4.设置解压参数，比如放大、缩小
 * 5.启动解压：jpeg_start_decompress(...)
 * 6.循环调用jpeg_read_scanlines(...)
 * 7.循环结束后，调用jpeg_finish_decompress(...)
 * 8.释放decompression结构体
 */

/*
 * Usage: jpeg2rgb <jpg_file>
 */
int main(int argc, char **argv)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *infile;
	int row_stride;
	unsigned char *buffer;

	/* fb 设备初始化 */
	if (0 != FBDeviceInit())
	{
		DBG_PRINTF("FBDeviceInit error\n");
		return -1;
	}

	/* 清屏 */
	FBCleanScreen(0);

	/* 1.分配和初始化一个decompression结构体 */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	/* 2.指定源文件 */
	if ((infile = fopen(argv[1], "rb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", argv[1]);
		return -1;
	}
	jpeg_stdio_src(&cinfo, infile);

	/* 3.调用jpeg_read_header获取jpg头部信息 */
	jpeg_read_header(&cinfo, TRUE);

	/* 源信息 */
	printf("image_with = %d\n", cinfo.image_width);
	printf("image_height = %d\n", cinfo.image_height);
	printf("num_components = %d\n", cinfo.num_components);

	/* 4.设置解压参数，比如放大、缩小(需要设置连个参数：scale_num, scale_denom)
	 * 缩放因子：scale_num/scale_denom，下面的设置为缩小1/2
	 */
	// printf("enter M/N:\n");
	// scanf("%d/%d",&cinfo.scale_num, &cinfo.scale_denom);
       cinfo.scale_num = 1;
       cinfo.scale_denom = 18;
	printf("scale to : %d/%d\n", cinfo.scale_num, cinfo.scale_denom);

	/*  5.启动解压：jpeg_start_decompress(...) */
	jpeg_start_decompress(&cinfo);

	/* 输出的图像信息 */
	printf("output_width = %d\n", cinfo.output_width);
	printf("output_height = %d\n", cinfo.output_height);
	printf("output_components = %d\n", cinfo.output_components);

	/* 一行的数据长度 */
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (unsigned char *)malloc(row_stride);

	/* 6.循环调用jpeg_read_scanlines(...)来一行一行地获得解压数据 */
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		(void)jpeg_read_scanlines(&cinfo, &buffer, 1);

		FBShowLine(0, cinfo.output_width, cinfo.output_scanline, buffer);
	}

	/* 7.循环结束后，调用jpeg_finish_decompress(...) */
	jpeg_finish_decompress(&cinfo);

	/* 8.释放decompression结构体 */
	jpeg_destroy_decompress(&cinfo);

	free(buffer);

	return 0;
}

#include <stdlib.h>

#include "jpeg_decoder.h"

JpegDecoder::JpegDecoder() {
    /* fb 设备初始化 */
    if (0 != show_.FBDeviceInit()) {
        DBG_PRINTF("FBDeviceInit error\n");
        exit(1);
    }
}

JpegDecoder::~JpegDecoder() {}


bool JpegDecoder::Init(FILE *infile)
{
    
    /* 1.分配和初始化一个decompression结构体 */
    cinfo_.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo_);

    jpeg_stdio_src(&cinfo_, infile);

    /* 3.调用jpeg_read_header获取jpg头部信息 */
    jpeg_read_header(&cinfo_, TRUE);

    /* 源信息 */
    printf("image_with = %d\n", cinfo_.image_width);
    printf("image_height = %d\n", cinfo_.image_height);
    printf("num_components = %d\n", cinfo_.num_components);

    return true;
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

void JpegDecoder::Decode()
{
    int row_stride;
    uint8_t *buffer;
    /* 4.设置解压参数，比如放大、缩小(需要设置连个参数：scale_num, scale_denom)
     * 缩放因子：scale_num/scale_denom，下面的设置为缩小1/2
     */
    // printf("enter M/N:\n");
    // scanf("%d/%d",&cinfo.scale_num, &cinfo.scale_denom);
    cinfo_.scale_num   = 1;
    cinfo_.scale_denom = 1;
    printf("scale to : %d/%d\n", cinfo_.scale_num, cinfo_.scale_denom);

    /*  5.启动解压：jpeg_start_decompress(...) */
    jpeg_start_decompress(&cinfo_);

    /* 输出的图像信息 */
    printf("output_width = %d\n", cinfo_.output_width);
    printf("output_height = %d\n", cinfo_.output_height);
    printf("output_components = %d\n", cinfo_.output_components);

    /* 一行的数据长度 */
    row_stride = cinfo_.output_width * cinfo_.output_components;
    buffer     = new uint8_t[row_stride];

    /* 清屏 */
	show_.FBCleanScreen(0);
    /* 6.循环调用jpeg_read_scanlines(...)来一行一行地获得解压数据 */
    while (cinfo_.output_scanline < cinfo_.output_height) {
        (void)jpeg_read_scanlines(&cinfo_, &buffer, 1);

        show_.FBShowLine(0, cinfo_.output_width, cinfo_.output_scanline, buffer);
    }

    /* 7.循环结束后，调用jpeg_finish_decompress(...) */
    jpeg_finish_decompress(&cinfo_);

    /* 8.释放decompression结构体 */
    jpeg_destroy_decompress(&cinfo_);

    delete[] buffer;
}

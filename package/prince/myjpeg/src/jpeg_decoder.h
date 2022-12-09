/**
 * @file jpeg_decompress.h
 * @author 黄李全 (846863428@qq.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-09
 * @copyright Copyright (c) {2021} 个人版权所有
 */

#include <iostream>
#include "jpeglib.h"
#include "fb_show.h"

class JpegDecoder
{
    public:
        JpegDecoder();
        ~JpegDecoder();

        bool Init(FILE *infile);

        void Decode();
    private:
        struct jpeg_decompress_struct cinfo_;
        struct jpeg_error_mgr jerr;
        FbShow show_;
};
#pragma once

namespace Decoder {
    void H264ToRGB(unsigned char* data, unsigned int dataSize, unsigned char* outBuffer);
    int Yuv2Rgb(FILE *yuvFile,FILE * rgbFile);
    void Mjpeg2RGB(unsigned char *data, unsigned int dataSize, unsigned char *outBuffer);
};
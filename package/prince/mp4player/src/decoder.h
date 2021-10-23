#pragma once

#include <stdint.h>

namespace Decoder {
    void H264ToRGB(uint8_t *data, uint32_t dataSize, uint8_t *outBuffer);
    int Yuv2Rgb(FILE *yuvFile, FILE * rgbFile);
    void Mjpeg2RGB(uint8_t *data, uint32_t dataSize, uint8_t *outBuffer);
};

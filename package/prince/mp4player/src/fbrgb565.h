#pragma once

int FBDeviceInit(void);
int FBCleanScreen(u_int32_t dwBackColor);
void Yuv420p2Rgb32(const u_int8_t *yuvBuffer_in, const u_int8_t *rgbBuffer_out, int width, int height);


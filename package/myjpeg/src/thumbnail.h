#ifndef _THUMBNAIL_H_
#define _THUMBNAIL_H_

unsigned char* ReadJpeg(const char* path, int& width, int& height);
unsigned char* do_Stretch_Linear(int w_Dest,int h_Dest,int bit_depth,unsigned char *src,int w_Src,int h_Src);
bool write_JPEG_file (const char * filename, unsigned char* image_buffer, int quality,int image_height, int image_width);
bool generate_image_thumbnail(const char* inputFile, const char* outputFile);

#endif

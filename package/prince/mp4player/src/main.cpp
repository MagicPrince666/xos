#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <codecvt>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>

extern "C" {
  #include <libavcodec/avcodec.h>
}

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types

#include "common.h"
#include "decoder.h"
#include "h264decoder.hpp"

typedef unsigned char ubyte;

/* shamelessly copy pasted from
 * http://dranger.com/ffmpeg/tutorial01.html
*/
void SaveFrame(const AVFrame &frame, int iFrame) 
{
	const auto *pFrame = &frame;
	FILE *pFile;
	char szFilename[32];
	int  y;
	
	// Open file
	sprintf(szFilename, "frame%02d.ppm", iFrame);
	pFile = fopen(szFilename, "wb");
	if(pFile == NULL)
		return;
	
	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", pFrame->width, pFrame->height);
	
	// Write pixel data
	for(y = 0; y < pFrame->height; y++)
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, pFrame->width*3, pFile);
	
	// Close file
	fclose(pFile);
}

static constexpr int file_read_size = 1024;

std::string CurrentTime()
{
	std::stringstream ss;
	std::string str = "%Y-%m-%d %H-%M-%S";
	std::chrono::system_clock::time_point a = std::chrono::system_clock::now();      //时间点可以做差
	time_t t1 = std::chrono::system_clock::to_time_t(a);				  //time_t可以格式化
	ss << std::put_time(localtime(&t1), str.c_str());
	std::string str1 = ss.str();
	return str1;
}

int main(int argc, char* argv[])
{
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

	std::string title = "Huang liquan ";
	title += CurrentTime();

	LcdRgb lcd(0);
	lcd.fill_screen_solid(0x0000ff);
	lcd.fb_put_string(0, 0, (char*)title.c_str(), title.size(), 0xffffff, 1, title.size());

	//Decoder::H264ToRGB(buff, fsize, outBuffer);

	H264Decoder decoder;
	ConverterRGB24 converter;
	disable_logging();
	std::FILE* fp = nullptr;
	if (argc > 2) {
		fp = std::fopen(argv[1], "rb");
		if (!fp) {
			fprintf(stderr, "cannot open file %s\n", argv[1]);
			return -1;
		}
	} else {
		spdlog::warn("Please put input file");
		return -1;
	}
	
	int frame_num = 0;
	std::vector<ubyte> framebuffer;
	std::vector<ubyte> buffer(file_read_size);
	for (;;) {
		int n = std::fread(&buffer[0], 1, buffer.size(), fp);
		if (n <= 0) break;
		
		auto const *data = &buffer[0];
		while (n > 0) {
			int num_parsed = decoder.parse(data, n);
			n -= num_parsed;
			data += num_parsed;
					
			if (decoder.is_frame_available()) {
				const AVFrame& frame = decoder.decode_frame();
				//NOTE: pixel format of enum symbol named AV_PIX_FMT_YUV420P comes out of h264 decoder. 
				// This symbols happens to have numeric value zero. So it is no bug when frame->format == 0.
				printf("frame decoded: %i x %i, fmt = %i\n", frame.width, frame.height, frame.format);
				
				framebuffer.resize(converter.predict_size(frame.width, frame.height));
				const AVFrame& rgbframe = converter.convert(frame, &framebuffer[0]);
				
				++frame_num;
				SaveFrame(rgbframe, frame_num);
			}
		} // while(n)
	}
#if 0
    uint8_t *rgbData[10] = {NULL};

    VideoCodec *VideoCodecObj = new VideoCodec();

    //Step 1: init
    int ret = VideoCodecObj->ffmpeg_init_h264_decoder();
	spdlog::info("Step 1 finished, ret = {}", ret);

    //Step 2: get YUV data
	std::ifstream infile;
	if (argc < 2) {
		spdlog::warn("Please put input file");
		infile.open(H264_TEST_FILE, std::ios::in | std::ios::binary);
	} else infile.open(argv[1], std::ios::in | std::ios::binary);

	
	uint8_t *outBuffer = nullptr;
	if(!infile.is_open())
      spdlog::warn("File is not open");

	std::streampos fsize = 0;
	fsize = infile.tellg();
    infile.seekg( 0, std::ios::end );
    fsize = infile.tellg() - fsize;

	infile.seekg (0, std::ios::beg);

	spdlog::info("File size {} bytes", fsize);

	uint8_t *buff = new uint8_t[fsize];
	infile.read ((char *)buff, fsize);
	spdlog::info("Step 2 finished, readDatalen = {}", fsize);

    //Step 3: decode
    ret =  VideoCodecObj->ffmpeg_decode_h264(buff, fsize, g_paramBuf, rgbData, NULL);
	spdlog::info("Step 3 finished, decode ret = {} width = {} height = {}", ret, g_paramBuf[0], g_paramBuf[1]);

    //Put the data to the target file according to the format 420.
    memcpy(g_ouputData, (char *)(rgbData[0]), g_paramBuf[0]*g_paramBuf[1]);
    memcpy(g_ouputData + g_paramBuf[0]*g_paramBuf[1] , (char *)(rgbData[1]), (g_paramBuf[0]*g_paramBuf[1])/4);
    memcpy(g_ouputData + ((g_paramBuf[0]*g_paramBuf[1]))*5/4 , (char *)(rgbData[2]), (g_paramBuf[0]*g_paramBuf[1])/4);

    //Step 4: write yuv data to file
    ret = write_raw_data_to_file(TARGET_RGB_FILE, g_ouputData, g_paramBuf[0]*g_paramBuf[1]*3/2);

    VideoCodecObj->ffmpeg_release_video_decoder();

	spdlog::info("Step 4 write finish len = {}", ret);

	delete[] buff;
	infile.close();
#endif

	return 0;
}

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
#include "libavutil/adler32.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/timestamp.h"
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

static int video_decode_example(const char *input_filename)
{
    AVCodec *codec = NULL;
    AVCodecContext *ctx= NULL;
    AVCodecParameters *origin_par = NULL;
    AVFrame *fr = NULL;
    uint8_t *byte_buffer = NULL;
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int video_stream;
    int got_frame = 0;
    int byte_buffer_size;
    int i = 0;
    int result;
    int end_of_stream = 0;

    result = avformat_open_input(&fmt_ctx, input_filename, NULL, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file\n");
        return result;
    }

    result = avformat_find_stream_info(fmt_ctx, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't get stream info\n");
        return result;
    }

    video_stream = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream < 0) {
      av_log(NULL, AV_LOG_ERROR, "Can't find video stream in input file\n");
      return -1;
    }

    origin_par = fmt_ctx->streams[video_stream]->codecpar;

    codec = avcodec_find_decoder(origin_par->codec_id);
    if (!codec) {
        av_log(NULL, AV_LOG_ERROR, "Can't find decoder\n");
        return -1;
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate decoder context\n");
        return AVERROR(ENOMEM);
    }

    result = avcodec_parameters_to_context(ctx, origin_par);
    if (result) {
        av_log(NULL, AV_LOG_ERROR, "Can't copy decoder context\n");
        return result;
    }

    result = avcodec_open2(ctx, codec, NULL);
    if (result < 0) {
        av_log(ctx, AV_LOG_ERROR, "Can't open decoder\n");
        return result;
    }

    fr = av_frame_alloc();
    if (!fr) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate frame\n");
        return AVERROR(ENOMEM);
    }

    byte_buffer_size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 16);
    byte_buffer = (uint8_t*)av_malloc(byte_buffer_size);
    if (!byte_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer\n");
        return AVERROR(ENOMEM);
    }

    printf("#tb %d: %d/%d\n", video_stream, fmt_ctx->streams[video_stream]->time_base.num, fmt_ctx->streams[video_stream]->time_base.den);
    i = 0;
    av_init_packet(&pkt);
    do {
        if (!end_of_stream)
            if (av_read_frame(fmt_ctx, &pkt) < 0)
                end_of_stream = 1;
        if (end_of_stream) {
            pkt.data = NULL;
            pkt.size = 0;
        }
        if (pkt.stream_index == video_stream || end_of_stream) {
            got_frame = 0;
            if (pkt.pts == AV_NOPTS_VALUE)
                pkt.pts = pkt.dts = i;
            result = avcodec_decode_video2(ctx, fr, &got_frame, &pkt);
            if (result < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error decoding frame\n");
                return result;
            }
            if (got_frame) {
                number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                        (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                        ctx->pix_fmt, ctx->width, ctx->height, 1);
                if (number_of_written_bytes < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Can't copy image to buffer\n");
                    return number_of_written_bytes;
                }
                // printf("%d, %s, %s, %8"PRId64", %8d, 0x%08lx\n", video_stream,
                //        av_ts2str(fr->pts), av_ts2str(fr->pkt_dts), fr->pkt_duration,
                //        number_of_written_bytes, av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
            }
            av_packet_unref(&pkt);
            av_init_packet(&pkt);
        }
        i++;
    } while (!end_of_stream || got_frame);

    av_packet_unref(&pkt);
    av_frame_free(&fr);
    avcodec_close(ctx);
    avformat_close_input(&fmt_ctx);
    avcodec_free_context(&ctx);
    av_freep(&byte_buffer);
    return 0;
}

int main(int argc, char* argv[])
{
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

	std::string title = "Huang liquan ";
	title += CurrentTime();

	LcdRgb lcd(0);
	lcd.fill_screen_solid(0x0000ff);
	lcd.fb_put_string(30, 0, (char*)title.c_str(), title.size(), 0xffffff, 0, title.size());

	if (argc < 2)
    {
        av_log(NULL, AV_LOG_ERROR, "Incorrect input\n");
        return 1;
    }

	if (video_decode_example(argv[1]) != 0)
        return 1;

	//Decoder::H264ToRGB(buff, fsize, outBuffer);
#if 0
	H264Decoder decoder;
	ConverterRGB24 converter;
	disable_logging();
	std::FILE* fp = nullptr;
	if (argc >= 2) {
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
#endif
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

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif

#include "decoder.h"

void Decoder::H264ToRGB(unsigned char* data, unsigned int dataSize, unsigned char* outBuffer)
{
    // 1. 将元数据装填到packet
    AVPacket* avPkt = av_packet_alloc();
    avPkt->size = dataSize;
    avPkt->data = data;

    static AVCodecContext* codecCtx = nullptr;
    if (codecCtx == nullptr) {
        // 2. 创建并配置codecContext
        AVCodec* h264Codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        codecCtx = avcodec_alloc_context3(h264Codec);
        avcodec_get_context_defaults3(codecCtx, h264Codec);
        avcodec_open2(codecCtx, h264Codec, nullptr);
    }

    // 3. 解码
    //avcodec_decode_video2(codecCtx, &outFrame, &lineLength, &avPkt);  // 接口被弃用，使用下边接口代替
    auto ret = avcodec_send_packet(codecCtx, avPkt);
    if (ret >= 0) {
        AVFrame* YUVFrame = av_frame_alloc();
        ret = avcodec_receive_frame(codecCtx, YUVFrame);
        if (ret >= 0) {

            // 4.YUV转RGB24
            AVFrame* RGB24Frame = av_frame_alloc();
            struct SwsContext* convertCxt = sws_getContext(
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_YUV420P,
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_RGB24,
                SWS_POINT, NULL, NULL, NULL
            );

            // outBuffer将会分配给RGB24Frame->data,AV_PIX_FMT_RGB24格式只分配到RGB24Frame->data[0]
            av_image_fill_arrays(
                RGB24Frame->data, RGB24Frame->linesize, outBuffer,
                AV_PIX_FMT_RGB24, YUVFrame->width, YUVFrame->height,
                1
            );
            sws_scale(convertCxt, YUVFrame->data, YUVFrame->linesize, 0, YUVFrame->height, RGB24Frame->data, RGB24Frame->linesize);

            // 5.清除各对象/context -> 释放内存
            // free context and avFrame
            sws_freeContext(convertCxt);
            av_frame_free(&RGB24Frame);
            // RGB24Frame.
        }
        // free context and avFrame
        av_frame_free(&YUVFrame);
    }
    // free context and avFrame
    av_packet_unref(avPkt);
    av_packet_free(&avPkt);
    // avcodec_free_context(&codecCtx);
}

int Decoder::Yuv2Rgb(FILE *yuvFile,FILE * rgbFile) {

	SwsContext *img_convert_ctx = NULL;

	const AVPixelFormat srcPixelFormat = AV_PIX_FMT_YUV420P;
	const AVPixelFormat dstPixelFormat = AV_PIX_FMT_RGB24;

	const int srcW = 480;
	const int srcH= 272;

	const int dstW = 320;
	const int dstH = 240;

	uint8_t *src_data[4];
	int src_linesize[4];

	uint8_t *dst_data[4];
	int dst_linesize[4];


	//1. 获得指定像素格式的AVPixFmtDescriptor结构体
	const AVPixFmtDescriptor * pSrcPixFmtDes = av_pix_fmt_desc_get(srcPixelFormat);
	//2. 获得指定像素格式每个像素占用的比特数bpp（Bit Per Pixel） 
	int srcBpp=  av_get_bits_per_pixel(pSrcPixFmtDes);

	const AVPixFmtDescriptor * pDstPixFmtDes = av_pix_fmt_desc_get(dstPixelFormat);
	av_get_bits_per_pixel(pDstPixFmtDes);

	//3. 根据宽高，像素格式分配buffer大小
	if (av_image_alloc(src_data, src_linesize, srcW, srcH, srcPixelFormat, 1) < 0) {
		return -1;
	}

	if (av_image_alloc(dst_data, dst_linesize, dstW, dstH, dstPixelFormat, 1) < 0) {
		return -1;
	}

	//4. 为SwsContext结构体分配内存。
	img_convert_ctx = sws_alloc_context();
	//Show AVOption
	av_opt_show2(img_convert_ctx, stdout, AV_OPT_FLAG_VIDEO_PARAM, 0);

	//图像拉伸   SWS_BICUBIC性能比较好；SWS_FAST_BILINEAR在性能和速度之间有一个比好好的平衡。
	//const int rescale_method = SWS_BICUBIC;
	//5. 设置值
	av_opt_set_int(img_convert_ctx, "sws_flags", SWS_BICUBIC | SWS_PRINT_INFO, 0);
	av_opt_set_int(img_convert_ctx, "srcw", srcW, 0);
	av_opt_set_int(img_convert_ctx, "srch", srcH, 0);
	av_opt_set_int(img_convert_ctx, "src_format", srcPixelFormat, 0);
	//'0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
	av_opt_set_int(img_convert_ctx, "src_range", 1, 0);
	av_opt_set_int(img_convert_ctx, "dstw", dstW, 0);
	av_opt_set_int(img_convert_ctx, "dsth", dstH, 0);
	av_opt_set_int(img_convert_ctx, "dst_format", dstPixelFormat, 0);
	av_opt_set_int(img_convert_ctx, "dst_range", 1, 0);
	sws_init_context(img_convert_ctx, NULL, NULL);//对SwsContext中的各种变量进行赋值

	uint8_t *temp_buffer = (uint8_t *)malloc(srcW *srcH *srcBpp / 8);

	int frame_idx = 0;
	while (1) {
		if (fread(temp_buffer, 1, srcW*srcH*srcBpp / 8, yuvFile) != srcW * srcH*srcBpp / 8) {
			break;
		}

		switch (srcPixelFormat) {
			case AV_PIX_FMT_GRAY8: {
				memcpy(src_data[0], temp_buffer, srcW*srcH);
				break;
			}
			case AV_PIX_FMT_YUV420P: {
				memcpy(src_data[0], temp_buffer, srcW*srcH);                    //Y
				memcpy(src_data[1], temp_buffer + srcW * srcH, srcW*srcH / 4);      //U
				memcpy(src_data[2], temp_buffer + srcW * srcH * 5 / 4, srcW*srcH / 4);  //V
				break;
			}
			case AV_PIX_FMT_YUV422P: {
				memcpy(src_data[0], temp_buffer, srcW*srcH);                    //Y
				memcpy(src_data[1], temp_buffer + srcW * srcH, srcW*srcH / 2);      //U
				memcpy(src_data[2], temp_buffer + srcW * srcH * 3 / 2, srcW*srcH / 2);  //V
				break;
			}
			case AV_PIX_FMT_YUV444P: {
				memcpy(src_data[0], temp_buffer, srcW*srcH);                    //Y
				memcpy(src_data[1], temp_buffer + srcW * srcH, srcW*srcH);        //U
				memcpy(src_data[2], temp_buffer + srcW * srcH * 2, srcW*srcH);      //V
				break;
			}
			case AV_PIX_FMT_YUYV422: {
				memcpy(src_data[0], temp_buffer, srcW*srcH * 2);                  //Packed
				break;
			}
			case AV_PIX_FMT_RGB24: {
				memcpy(src_data[0], temp_buffer, srcW*srcH * 3);                  //Packed
				break;
			}
			default: {
				printf("Not Support Input Pixel Format.\n");
				break;
			}
		}

		sws_scale(img_convert_ctx, src_data, src_linesize, 0, srcH, dst_data, dst_linesize);//转换像素
		printf("Finish process frame %5d\n", frame_idx);
		frame_idx++;

		switch (dstPixelFormat) {
			case AV_PIX_FMT_GRAY8: {
				fwrite(dst_data[0], 1, dstW*dstH, rgbFile);
				break;
			}
			case AV_PIX_FMT_YUV420P: {
				fwrite(dst_data[0], 1, dstW*dstH, rgbFile);                 //Y
				fwrite(dst_data[1], 1, dstW*dstH / 4, rgbFile);               //U
				fwrite(dst_data[2], 1, dstW*dstH / 4, rgbFile);               //V
				break;
			}
			case AV_PIX_FMT_YUV422P: {
				fwrite(dst_data[0], 1, dstW*dstH, rgbFile);                 //Y
				fwrite(dst_data[1], 1, dstW*dstH / 2, rgbFile);               //U
				fwrite(dst_data[2], 1, dstW*dstH / 2, rgbFile);               //V
				break;
			}
			case AV_PIX_FMT_YUV444P: {
				fwrite(dst_data[0], 1, dstW*dstH, rgbFile);                 //Y
				fwrite(dst_data[1], 1, dstW*dstH, rgbFile);                 //U
				fwrite(dst_data[2], 1, dstW*dstH, rgbFile);                 //V
				break;
			}
			case AV_PIX_FMT_YUYV422: {
				fwrite(dst_data[0], 1, dstW*dstH * 2, rgbFile);               //Packed
				break;
			}
			case AV_PIX_FMT_RGB24: {
				fwrite(dst_data[0], 1, dstW*dstH * 3, rgbFile);               //Packed
				break;
			}
			default: {
				break;
			}
		}
	}

	sws_freeContext(img_convert_ctx);
	free(temp_buffer);
	av_freep(&src_data[0]);
	av_freep(&dst_data[0]);

	return 0;
}

void Decoder::Mjpeg2RGB(unsigned char *data, unsigned int dataSize, unsigned char *outBuffer)
{   
    // 1. 将元数据装填到packet
    AVPacket *avPkt = av_packet_alloc();
    avPkt->size = dataSize;
    avPkt->data = data;

    // 2. 创建并配置codecContext
    AVCodec *mjpegCodec = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
    AVCodecContext* codecCtx = avcodec_alloc_context3(mjpegCodec);
    avcodec_get_context_defaults3(codecCtx, mjpegCodec);
    avcodec_open2(codecCtx, mjpegCodec, nullptr);

    // 3. 解码
    //avcodec_decode_video2(codecCtx, &outFrame, &lineLength, &avPkt);  // 接口被弃用，使用下边接口代替
    auto ret = avcodec_send_packet(codecCtx, avPkt);
    if (ret >=0) {
        AVFrame* YUVFrame = av_frame_alloc();
        ret = avcodec_receive_frame(codecCtx, YUVFrame);
        if (ret >= 0) { 

            // 4.YUV转RGB24
            AVFrame* RGB24Frame = av_frame_alloc();
            struct SwsContext* convertCxt = sws_getContext(
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_YUV420P,
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_RGB24,
                SWS_POINT, NULL, NULL, NULL
            );

            // outBuffer将会分配给RGB24Frame->data,AV_PIX_FMT_RGB24格式只分配到RGB24Frame->data[0]
            av_image_fill_arrays(
                RGB24Frame->data, RGB24Frame->linesize, outBuffer,  
                AV_PIX_FMT_RGB24, YUVFrame->width, YUVFrame->height,
                1
            );
            sws_scale(convertCxt, YUVFrame->data, YUVFrame->linesize, 0, YUVFrame->height, RGB24Frame->data, RGB24Frame->linesize);

            // 5.清除各对象/context -> 释放内存
            // free context and avFrame
            sws_freeContext(convertCxt);
            av_frame_free(&RGB24Frame);
            // RGB24Frame.
        }
        // free context and avFrame
        av_frame_free(&YUVFrame);
    }
    // free context and avFrame
    av_packet_unref(avPkt);
    av_packet_free(&avPkt);
    avcodec_free_context(&codecCtx);
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <stdlib.h>

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

#include "fbrgb565.h"

#define FF_INPUT_BUFFER_PADDING_SIZE 16

int yuv2Rgb(FILE *yuvFile,FILE * rgbFile) {

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
	int dstBpp = av_get_bits_per_pixel(pDstPixFmtDes);

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
	while (1){
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

int main(int argc, char* argv[])
{
	/* fb 设备初始化 */
	if (0 != FBDeviceInit())
	{
		//DBG_PRINTF("FBDeviceInit error\n");
		return -1;
	}

	/* 清屏 */
	FBCleanScreen(0x00FF00);

	AVCodec *pCodec;
    AVCodecContext *pCodecCtx= NULL;
	AVCodecParserContext *pCodecParserCtx=NULL;

    FILE *fp_in;
	FILE *fp_out;
    AVFrame	*pFrame;

	const int in_buffer_size = 4096;
	unsigned char in_buffer[in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE]={0};
	unsigned char *cur_ptr;
	int cur_size;
    AVPacket packet;
	int ret, got_picture;

	AVCodecID codec_id = AV_CODEC_ID_H264;
	char filepath_in[] = "bigbuckbunny_480x272.h264";

	char filepath_out[] = "bigbuckbunny_480x272.yuv";
	int first_time = 1;

	avcodec_register_all();

    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        printf("Could not allocate video codec context\n");
        return -1;
    }

	pCodecParserCtx = av_parser_init(codec_id);
	if (!pCodecParserCtx) {
		printf("Could not allocate video parser context\n");
		return -1;
	}

    //if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
    //    pCodecCtx->flags|= CODEC_FLAG_TRUNCATED;

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }
	//Input File
    fp_in = fopen(filepath_in, "rb");
    if (!fp_in) {
        printf("Could not open input stream\n");
        return -1;
    }
	//Output File
	fp_out = fopen(filepath_out, "wb");
	if (!fp_out) {
		printf("Could not open output YUV file\n");
		return -1;
	}

    pFrame = av_frame_alloc();
	av_init_packet(&packet);

	SwsContext* swsContext = swsContext = sws_getContext(pFrame->width, pFrame->height, AV_PIX_FMT_YUV420P,pFrame->width, pFrame->height, AV_PIX_FMT_BGR24,
                          NULL, NULL, NULL, NULL);

	while (1) {

        cur_size = fread(in_buffer, 1, in_buffer_size, fp_in);
        if (cur_size == 0)
            break;
        cur_ptr=in_buffer;

        while (cur_size>0){

			int len = av_parser_parse2(
				pCodecParserCtx, pCodecCtx,
				&packet.data, &packet.size,
				cur_ptr , cur_size ,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			cur_ptr += len;
			cur_size -= len;

			if(packet.size==0)
				continue;

			//Some Info from AVCodecParserContext
			printf("[Packet]Size:%6d\t",packet.size);
			switch(pCodecParserCtx->pict_type){
				case AV_PICTURE_TYPE_I: printf("Type:I\t");break;
				case AV_PICTURE_TYPE_P: printf("Type:P\t");break;
				case AV_PICTURE_TYPE_B: printf("Type:B\t");break;
				default: printf("Type:Other\t");break;
			}
			printf("Number:%4d\n",pCodecParserCtx->output_picture_number);

			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
			if (ret < 0) {
				printf("Decode Error.\n");
				return ret;
			}
			if (got_picture) {
				if(first_time){
					printf("\nCodec Full Name:%s\n",pCodecCtx->codec->long_name);
					printf("width:%d\nheight:%d\n\n",pCodecCtx->width,pCodecCtx->height);
					first_time=0;
				}
				//Y, U, V
				for(int i=0;i<pFrame->height;i++){
					fwrite(pFrame->data[0]+pFrame->linesize[0]*i,1,pFrame->width,fp_out);
				}
				for(int i=0;i<pFrame->height/2;i++){
					fwrite(pFrame->data[1]+pFrame->linesize[1]*i,1,pFrame->width/2,fp_out);
				}
				for(int i=0;i<pFrame->height/2;i++){
					fwrite(pFrame->data[2]+pFrame->linesize[2]*i,1,pFrame->width/2,fp_out);
				}

				printf("Succeed to decode 1 frame!\n");
			}
		}

    }

	//Flush Decoder
    packet.data = NULL;
    packet.size = 0;
	while(1){
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
		if (ret < 0) {
			printf("Decode Error.\n");
			return ret;
		}
		if (!got_picture){
			break;
		}else {
			//Y, U, V
			for(int i=0;i<pFrame->height;i++){
				fwrite(pFrame->data[0]+pFrame->linesize[0]*i,1,pFrame->width,fp_out);
			}
			for(int i=0;i<pFrame->height/2;i++){
				fwrite(pFrame->data[1]+pFrame->linesize[1]*i,1,pFrame->width/2,fp_out);
			}
			for(int i=0;i<pFrame->height/2;i++){
				fwrite(pFrame->data[2]+pFrame->linesize[2]*i,1,pFrame->width/2,fp_out);
			}

			printf("Flush Decoder: Succeed to decode 1 frame!\n");
		}
	}

    fclose(fp_in);
	fclose(fp_out);


	av_parser_close(pCodecParserCtx);

	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);

	return 0;
}

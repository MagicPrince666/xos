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

#include "mjpegrgb.h"

void MJPEGToRGB(unsigned char *data, unsigned int dataSize, unsigned char *outBuffer)
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

#include "ffmpegH264Interface.h"
#include "BaseDebug.h"
#include "BaseMemory.h"


ffmpegH264Interface::ffmpegH264Interface(void)
: pYuvBuffer(wmNULL)
, sWidth(0)
, sHeight(0)
, pAVCodec(wmNULL)
, pAVCodecContext(wmNULL)
{
    DWMDEBUG("");
}

ffmpegH264Interface::~ffmpegH264Interface(void)
{
    DWMDEBUG("");
}

wmBOOL ffmpegH264Interface::InitH264Encoder(wmINT iWidth,wmINT iHeight,wmINT nKeyFrameInterval,wmINT ifpsNum)
{    
    DWMDEBUG("");
    avcodec_register_all();

    pAVCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(wmNULL == pAVCodec)
    {
        DWMERROR("avcodec_find_encoder error");
        return wmFALSE;
    }
    sWidth=iWidth;
    sHeight=iHeight;
    
    pAVCodecContext = avcodec_alloc_context3(pAVCodec);
    if(wmNULL == pAVCodecContext)
    {
        DWMERROR("avcodec_alloc_context3 error");
        return wmFALSE;
    } 
    pAVCodecContext->bit_rate = 3000000;
    pAVCodecContext->width =iWidth;
    pAVCodecContext->height = iHeight;
    AVRational rate;
    rate.num = 1;
    rate.den = 25;
    
    pAVCodecContext->time_base= rate;
    pAVCodecContext->gop_size = 10; 
    pAVCodecContext->max_b_frames=1;
    pAVCodecContext->thread_count = 1;
    pAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if(avcodec_open2(pAVCodecContext,pAVCodec,wmNULL)<wmFalse)
    {
        DWMERROR("avcodec_open2 error");
        return wmFALSE;
    } 

    pYuvBuffer = (wmU8T*)DwmMalloc(iWidth*iHeight*3/2);
    if(wmNULL == pYuvBuffer)
    {
        DWMERROR("DwmMalloc error");
        return wmFALSE;
    }

    DwmMemset(pYuvBuffer, 0x0, sizeof(pYuvBuffer));

    
    return true;
}


wmBOOL ffmpegH264Interface::H264Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame,wmINT& nOutLen)
{
    AVPacket avpkt;
    wmINT iGotPacketPtr = 0;
    av_init_packet(&avpkt);
    avpkt.data = pOutFrame;
    avpkt.size = 0;
    DwmMemcpy(pYuvBuffer, pInFrame, 0x0, nInLen);
    AVFrame* pAVFrame = av_frame_alloc();
    if (wmNULL == pAVFrame)
    {
        DWMERROR("pAVFrame is null");
        return wmFALSE;
    }
    avpicture_fill((AVPicture*)pAVFrame, (wmU8T*)pInFrame, AV_PIX_FMT_YUV420P, sWidth, sHeight);
    wmINT iLen = avcodec_encode_video2(pAVCodecContext, &avpkt, pAVFrame, &iGotPacketPtr);
    DWMDEBUG("[%d][%d]",avpkt.size,iLen);
    if (iLen == 0)
    {
        nOutLen = avpkt.size;
    }
    
    av_frame_free(&pAVFrame);
    
    pAVFrame = wmNULL;
    return wmTRUE;
}

wmVOID ffmpegH264Interface::CloseH264Encoder()
{
    DWMDEBUG("");
    avcodec_close(pAVCodecContext);
    av_free(pAVCodecContext);

    if(pYuvBuffer)
    {
        DwmFree(pYuvBuffer);
    }
}


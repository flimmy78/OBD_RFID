#include "VideoDecoder.h"
#include "BaseDebug.h"
#include "BaseMemory.h"
#include "BaseTime.h"
#include <stdio.h>

CVideoDecodeHandle::CVideoDecodeHandle()
    : pAVCodecContext(wmNULL)
{
}

wmBOOL CVideoDecodeHandle::Init()
{
    DWMDEBUG("");
    avcodec_register_all();
    AVCodec *pDecoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!pDecoder)
    {
        DWMERROR("pDecoder is null");
        return wmFALSE;
    }

    pAVCodecContext = avcodec_alloc_context3(pDecoder);
    if (!pAVCodecContext) 
    {
        DWMERROR("pAVCodecContext is null");
        return wmFALSE;
    }
    pAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    pAVCodecContext->active_thread_type |= FF_THREAD_FRAME;

    pDecoder->capabilities |= CODEC_CAP_DELAY;

    if (pDecoder->capabilities & CODEC_CAP_TRUNCATED) {
        pAVCodecContext->flags |= CODEC_FLAG_TRUNCATED;
    }

    if (avcodec_open2(pAVCodecContext, pDecoder,wmNULL) < 0)
    {
        DWMERROR("avcodec_open2 error");
        return wmFALSE;
    }

    return wmTRUE;
}

wmVOID CVideoDecodeHandle::Close()
{
    DWMDEBUG("");
    if (pAVCodecContext) {
        avcodec_close(pAVCodecContext);
        av_free(pAVCodecContext);
        pAVCodecContext = wmNULL;
    }
}

wmBOOL CVideoDecodeHandle::Decode(wmVOID *pData, wmSIZET sSize,  wmU8T *pOutput, wmINT &iOutBit,  wmINT &iWidth, int &iHeight)
{
    AVFrame* pAVFrame = av_frame_alloc();
    if (wmNULL == pAVFrame)
    {
        DWMERROR("pAVFrame is null");
        return wmFALSE;
    }
    
    AVPacket avPacket;
    av_init_packet(&avPacket);
    avPacket.data = (wmU8T*)pData;
    avPacket.size = sSize + FF_INPUT_BUFFER_PADDING_SIZE;

    wmINT iRetLent = 0;
    wmINT iDecodeLen = avcodec_decode_video2(pAVCodecContext, pAVFrame, &iRetLent, &avPacket);
    if (iDecodeLen < 0) {
        av_free_packet(&avPacket);
        av_frame_free(&pAVFrame);
        pAVFrame = wmNULL;
        DWMERROR("avcodec_decode_video2 error");
        return wmFALSE;
    }    

    DWMDEBUG("[%d][%d]",pAVFrame->width,pAVFrame->height);
    if(iRetLent)
    {
        wmINT iCurrentW = pAVFrame->width;
        wmINT iCurrentW2 = iCurrentW/2;
        wmINT iCurrentH = pAVFrame->height;
        wmINT iCurrentH2 = iCurrentH/2;
        iWidth = iCurrentW;
        iHeight = iCurrentH;

        wmINT uBit = 0;
        // Y
        for (wmINT y = 0; y < iCurrentH; y++) {
            wmU8T const *scanline = pAVFrame->data[0] + y * pAVFrame->linesize[0];
            memcpy(pOutput + uBit, scanline, iCurrentW);
            uBit += iCurrentW;
        }
        // V
        for (wmINT y = 0; y < iCurrentH2; y++) {
            wmU8T const *scanline = pAVFrame->data[1] + y * pAVFrame->linesize[1];
            memcpy(pOutput + uBit, scanline, iCurrentW2);
            uBit += iCurrentW2;
        }
        // U
        for (wmINT y = 0; y < iCurrentH2; y++) {
            wmU8T const *scanline = pAVFrame->data[2] + y * pAVFrame->linesize[2];
            memcpy(pOutput + uBit, scanline, iCurrentW2);
            uBit += iCurrentW2;
        }
        iOutBit = uBit;
    }
    av_free_packet(&avPacket);
    av_frame_free(&pAVFrame);
    pAVFrame = wmNULL;
    return wmTRUE;
}

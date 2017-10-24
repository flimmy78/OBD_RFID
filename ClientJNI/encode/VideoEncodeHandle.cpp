#include "VideoEncodeHandle.h"

CVideoEncodeHandle::CVideoEncodeHandle()
:pX264Handle(wmNULL)
, pFfmpegH264Interface(wmNULL)
{

}

wmBOOL CVideoEncodeHandle::Init(wmINT iWidth, wmINT iHeight, wmINT nKeyFrameInterval, wmINT ifpsNum)
{

#ifdef X264INTERFACE
    if(pX264Handle)
    {
        pX264Handle->CloseX264Encoder();
        wmDELETE pX264Handle;
        pX264Handle = wmNULL;
    }

    pX264Handle = new x264Interface();
    pX264Handle->InitX264Encoder(iWidth, iHeight, nKeyFrameInterval, ifpsNum);
#else
    if(pFfmpegH264Interface)
    {
        pFfmpegH264Interface->CloseH264Encoder();
        wmDELETE pFfmpegH264Interface;
        pFfmpegH264Interface = wmNULL;
    }

    pFfmpegH264Interface = new ffmpegH264Interface();
    pFfmpegH264Interface->InitH264Encoder(iWidth, iHeight, nKeyFrameInterval, ifpsNum);  
#endif

    return wmTRUE;
}


wmINT CVideoEncodeHandle::Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame)
{
    wmINT iOutLen = wmFalse;
#ifdef X264INTERFACE
    if(wmNULL == pX264Handle)
    {
        return wmFalse;
    }

    pX264Handle->X264Encode(pInFrame, nInLen, pOutFrame, iOutLen);
#else
    if(wmNULL == pFfmpegH264Interface)
    {
        return wmFalse;
    }

    pFfmpegH264Interface->H264Encode(pInFrame, nInLen, pOutFrame, iOutLen);
#endif
    return iOutLen;
}

wmBOOL CVideoEncodeHandle::Close()
{
#ifdef X264INTERFACE
    if(pX264Handle)
    {
        pX264Handle->CloseX264Encoder();
        wmDELETE pX264Handle;
        pX264Handle = wmNULL;
    }  
#else
    if(pFfmpegH264Interface)
    {
        pFfmpegH264Interface->CloseH264Encoder();
        wmDELETE pFfmpegH264Interface;
        pFfmpegH264Interface = wmNULL;
    }
#endif
    return wmTRUE;
}

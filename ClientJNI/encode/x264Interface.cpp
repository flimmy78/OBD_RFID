#include "x264Interface.h"
#include "BaseDebug.h"
#include "BaseMemory.h"

#define sCveryfast        "veryfast"
#define sCzerolatency  "zerolatency"
#define sCbaseline        "baseline"

x264Interface::x264Interface(void)
: sWidth(0)
, sHeight(0)
{
    pX264T = wmNULL;
    pYuvBuffer = wmNULL;
    pX264NalT = wmNULL;
    DWMDEBUG("");
}

x264Interface::~x264Interface(void)
{
    DWMDEBUG("");
}

wmBOOL x264Interface::InitX264Encoder(wmINT iWidth,wmINT iHeight,wmINT nKeyFrameInterval,wmINT ifpsNum)
{    
    DWMDEBUG("");
    x264_param_t x264ParamT;
    x264_param_default_preset(&x264ParamT, sCveryfast, sCzerolatency);
    x264ParamT.i_threads = 5;
    x264ParamT.i_width = iWidth;
    x264ParamT.i_height = iHeight;
    //x264ParamT.i_fps_num = ifpsNum;
    //x264ParamT.i_fps_den = 1;
    //x264ParamT.i_keyint_max = nKeyFrameInterval;
    //x264ParamT.b_intra_refresh = 1;
    //x264ParamT.b_annexb = 1;

    x264_param_apply_profile(&x264ParamT, sCbaseline);
    pYuvBuffer = (wmU8T*)DwmMalloc(iWidth*iHeight*3);
    if(wmNULL == pYuvBuffer)
    {
        DWMERROR("pYuvBuffer is NULL");
        return wmFALSE;
    }
    DwmMemset(pYuvBuffer, 0x0, sizeof(pYuvBuffer));
    
    pX264T = x264_encoder_open(&x264ParamT);
    if(wmNULL == pX264T)
    {
        DWMERROR("pX264T is null");
        return false;
    }
    sWidth=iWidth;
    sHeight=iHeight;

    x264_picture_alloc(&x264PictureTIn,X264_CSP_YV12,sWidth,sHeight);
    x264PictureTIn.img.plane[0] = pYuvBuffer;
    x264PictureTIn.img.plane[1] = x264PictureTIn.img.plane[0] +  sWidth*sHeight;
    x264PictureTIn.img.plane[2] = x264PictureTIn.img.plane[1] + sWidth*sHeight / 4;
    
    return true;
}


wmBOOL x264Interface::X264Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame,wmINT& nOutLen)
{
    wmINT iNNal = 0;
    DwmMemset(pYuvBuffer, 0x0, sizeof(pYuvBuffer));
    DwmMemcpy(pYuvBuffer,pInFrame,0,nInLen);

    x264_encoder_encode(pX264T, &pX264NalT, &iNNal, &x264PictureTIn, &x264PictureTOut);
    if(iNNal <= 0)
    {
        return wmFALSE;
    }

    for(wmINT iNNALJ=0; iNNALJ<iNNal;iNNALJ++)
    {
        DwmMemcpy(pOutFrame+nOutLen,pX264NalT[iNNALJ].p_payload,0,pX264NalT[iNNALJ].i_payload);
        nOutLen += pX264NalT[iNNALJ].i_payload;
    }
    
    return wmTRUE;
}

wmVOID x264Interface::CloseX264Encoder()
{
    DWMDEBUG("");
    if(pX264T)
    {
        x264_encoder_close(pX264T);
        pX264T = wmNULL;
    }

    if(pYuvBuffer)
    {
        DwmFree(pYuvBuffer);
    }
}


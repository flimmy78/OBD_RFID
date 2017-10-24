#include "BaseDefine.h"

extern "C" {
	#include "libavcodec/avcodec.h"
}

class ffmpegH264Interface 
{
public:
    ffmpegH264Interface();
    ~ffmpegH264Interface();
    
    wmBOOL InitH264Encoder(wmINT iWidth,wmINT iHeight,wmINT nKeyFrameInterval,wmINT ifpsNum);
    wmBOOL H264Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame,wmINT& nOutLen);
    wmVOID CloseH264Encoder();
private:
    wmU8T* pYuvBuffer;
    wmUSHORT    sWidth;
    wmUSHORT    sHeight;
    AVCodec *pAVCodec;
    AVCodecContext *pAVCodecContext;
};

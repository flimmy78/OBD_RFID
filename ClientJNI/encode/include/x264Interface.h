#include "BaseDefine.h"
extern "C" 
{
#include "x264.h"
};

class x264Interface 
{
public:
    x264Interface();
    ~x264Interface();
    
    wmBOOL InitX264Encoder(wmINT iWidth,wmINT iHeight,wmINT nKeyFrameInterval,wmINT ifpsNum);
    wmBOOL X264Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame,wmINT& nOutLen);
    wmVOID CloseX264Encoder();
private:
    x264_t *pX264T;

    wmU8T* pYuvBuffer;
    wmUSHORT    sWidth;
    wmUSHORT    sHeight;
    x264_picture_t x264PictureTOut;
    x264_picture_t x264PictureTIn;
    x264_nal_t *pX264NalT;
};

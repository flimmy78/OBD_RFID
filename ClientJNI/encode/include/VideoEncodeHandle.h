#include "BaseDefine.h"
#include "BaseTable.h"
#include "x264Interface.h"
#include "ffmpegH264Interface.h"

class CVideoEncodeHandle: public TInstance<CVideoEncodeHandle>
{
    friend class TInstance<CVideoEncodeHandle>;
public:
    wmBOOL Init(wmINT iWidth, wmINT iHeight, wmINT nKeyFrameInterval, wmINT ifpsNum);
    wmINT Encode(wmU8T const* pInFrame,wmINT nInLen,wmU8T* pOutFrame);
    wmBOOL Close();
private:
    CVideoEncodeHandle();
    x264Interface* pX264Handle;
    ffmpegH264Interface* pFfmpegH264Interface;
};

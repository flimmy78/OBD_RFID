#ifndef _VIDEODECODE_H_
#define _VIDEODECODE_H_

#include "BaseDefine.h"
#include "BaseTable.h"

extern "C" {
	#include "libavcodec/avcodec.h"
}


class CVideoDecodeHandle: public TInstance<CVideoDecodeHandle>
{
    friend class TInstance<CVideoDecodeHandle>;
public:
    wmBOOL Init();
    wmVOID Close();
    wmBOOL Decode(wmVOID *pData, wmSIZET sSize,  wmU8T *pOutput, wmINT &iDecodeLen, wmINT &iWidth, int &iHeight);

private:
    CVideoDecodeHandle();
    AVCodecContext    *pAVCodecContext;
};

#endif  // _VIDEODECODE_H_

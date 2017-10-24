/**
 * @file CRfid.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1
 * 
 */
#ifndef __RFID_H__
#define __RFID_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseThread.h"
#include "BaseLock.h"

class CRfid: public TInstance<CRfid>
{
    friend class TInstance<CRfid>;
public:

    wmVOID SwitchRfid(wmCHAR* pSrcData, wmINT iLen, wmUSHORT& sDirType, wmCHAR* pDirData);
private:
    CRfid();
    ~CRfid();
};


#endif  // end __RFID_H__
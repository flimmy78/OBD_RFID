/**
 * @file RfidDeque.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 
 * 
 */
#ifndef __BASE_DEQUE_H__
#define __BASE_DEQUE_H__

#include "BaseDefine.h"
#include "BaseLock.h"
#include "BaseType.h"

typedef wmMAP<wmINT, SDataBuffRfidDevItem *> SDataBuffMap;

class RfidDeque
{
public:
    RfidDeque();
    ~RfidDeque();

    wmBOOL addBufferMap(SDataBuffRfidDevItem *pSSqlItem);
    wmVOID DeleteBufferMap(wmINT iSocketId);

    wmVOID GetSocketId(wmINT& iGSocket, wmCCHAR* pValue1, wmCCHAR* pValue2);
    wmVOID GetDeviceId(wmINT iGSocket, wmCHAR* pValue1, wmCCHAR* pValue2);
    wmVOID flush();
private:
    SDataBuffMap dBufferMap;
    wmLock mutexLock;
};

#endif // __BASE_DEQUE_H__
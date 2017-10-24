#include "RfidDeque.h"
#include "BaseMemory.h"
#include "BaseDebug.h"

RfidDeque::RfidDeque()
{
    mutexLock = WMCreateLock(DEQUE_LOCK_NAME);
}

RfidDeque::~RfidDeque()
{
    WMDestroyLock(mutexLock);
}

wmBOOL RfidDeque::addBufferMap(SDataBuffRfidDevItem *pSSqlItem)
{
    if(wmNULL == pSSqlItem || wmNULL == pSSqlItem->pDevtId)
    {
        DWMERROR("pSSqlItem is NULL");
        return wmFALSE;
    }
    
    WMLock(mutexLock);
    SDataBuffMap::iterator iTerator = dBufferMap.begin();
    while(iTerator != dBufferMap.end())
    {
        SDataBuffRfidDevItem *pSDataBuff = iTerator->second;
        if(pSDataBuff)
        {
            if ((pSDataBuff->iSocketId == pSSqlItem->iSocketId)  ||
                DwmStrcmp(pSDataBuff->pDevtId, pSSqlItem->pDevtId))
            {
                wmDELETE(pSDataBuff);
                pSDataBuff = wmNULL;
                dBufferMap.erase(iTerator);
            }
        }
        iTerator++;
    }
    dBufferMap.insert(wmMAPPAIR(pSSqlItem->iSocketId, pSSqlItem));
    WMUnLock(mutexLock);
    return wmTRUE;
}

wmVOID RfidDeque::DeleteBufferMap(wmINT iSocketId)
{
    WMLock(mutexLock);
    SDataBuffMap::iterator iTerator = dBufferMap.begin();
    while(iTerator != dBufferMap.end())
    {
        SDataBuffRfidDevItem *pSDataBuff = iTerator->second;
        if(pSDataBuff)
        {
            if (pSDataBuff->iSocketId == iSocketId)
            {
                wmDELETE(pSDataBuff);
                pSDataBuff = wmNULL;
                dBufferMap.erase(iTerator);
            }
        }
        iTerator++;
    }
    WMUnLock(mutexLock);
}

wmVOID RfidDeque::GetSocketId(wmINT& iGSocket, wmCCHAR* pValue1, wmCCHAR* pValue2)
{
    if(wmNULL == pValue1)
    {
        DWMERROR("pValue1 is wrong");
        return;
    }
    WMLock(mutexLock);
    SDataBuffMap::iterator iTerator = dBufferMap.begin();
    while(iTerator != dBufferMap.end())
    {
        SDataBuffRfidDevItem *pSDataBuff = iTerator->second;
        if(pSDataBuff)
        {
            DWMDEBUG("[%s][%d]",pSDataBuff->pDevtId,pSDataBuff->iSocketId);
            if (DwmStrcmp(pSDataBuff->pDevtId, pValue1))
            {
                if(wmNULL == pValue2)
                {
                    iGSocket = pSDataBuff->iSocketId;
                    break;
                }

                if (DwmStrcmp(pSDataBuff->pPassword, pValue2))
                {
                    iGSocket = pSDataBuff->iSocketId;
                    break;
                }
            }
        }
        iTerator++;
    }
    WMUnLock(mutexLock);
}

wmVOID RfidDeque::GetDeviceId(wmINT iGSocket, wmCHAR* pValue1, wmCCHAR* pValue2)
{
    if(wmNULL == pValue1)
    {
        DWMERROR("pValue1 is wrong");
        return;
    }
    WMLock(mutexLock);
    SDataBuffMap::iterator iTerator = dBufferMap.begin();
    while(iTerator != dBufferMap.end())
    {
        SDataBuffRfidDevItem *pSDataBuff = iTerator->second;
        if(pSDataBuff && pSDataBuff->pDevtId)
        {
            DWMDEBUG("[%s][%d]",pSDataBuff->pDevtId,pSDataBuff->iSocketId);
            
            if (iGSocket == pSDataBuff->iSocketId)
            {
                if(wmNULL == pValue2)
                {
                    DwmStrcpy(pValue1, pSDataBuff->pDevtId);
                    break;
                }

                if (pSDataBuff->pPassword && DwmStrcmp(pSDataBuff->pPassword, pValue2))
                {
                    DwmStrcpy(pValue1, pSDataBuff->pDevtId);
                    break;
                }
            }
        }
        iTerator++;
    }
    WMUnLock(mutexLock);
}

wmVOID RfidDeque::flush()
{
    WMLock(mutexLock);
    while (dBufferMap.size()>0) {
        SDataBuffMap::iterator iTerator = dBufferMap.begin();
        SDataBuffRfidDevItem *pSDataBuff = iTerator->second;
        wmDELETE(pSDataBuff);
        pSDataBuff = wmNULL;
        dBufferMap.erase(iTerator);
    }
    WMUnLock(mutexLock);
}

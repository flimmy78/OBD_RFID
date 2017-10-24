#include "ServerTimer.h"
#include "BaseTime.h"
#include "BaseDebug.h"

CServerTimer::CServerTimer()
    : tThread(ThreadProcTimer)
    , bWantToStopTimer(wmFALSE)
{

}

wmBOOL CServerTimer::ServerTimerInit()
{
    mutexLock = WMCreateLock(TIMER_LOCK_NAME);
    tThread.start(this);
	return wmTRUE;
}

wmBOOL CServerTimer::ServerTimerClear()
{
    bWantToStopTimer = wmTRUE;
    tThread.join();
    WMDestroyLock(mutexLock); 
	return wmTRUE;
}



wmINT CServerTimer::ThreadProcTimer(void *pThis)
{
    CServerTimer *pCServerTimer = reinterpret_cast<CServerTimer *>(pThis);
    if(wmNULL == pCServerTimer)
    {
        DWMERROR("pCServerTimer is null");
        return wmFalse;
    }
    
    while(wmFALSE == pCServerTimer->bWantToStopTimer)
    {
        pCServerTimer->ProcessTimerPool();
        DwmSleep(500);
    }
    return wmFalse;
}

wmBOOL CServerTimer::ProcessTimerPool()
{
    WMLock(mutexLock);
    if (dtDataTimerData.size() > wmFalse) {
        wmVECTOR<stTimerData *>::iterator itTimerData = dtDataTimerData.begin();
        for(itTimerData = dtDataTimerData.begin(); itTimerData != dtDataTimerData.end(); ++itTimerData)
        {
            stTimerData *pstTimerData = *itTimerData;
            if(pstTimerData)
            {
                wmQWORD wqTime = MPGetTimestamp();
                wmQWORD wqEndTime = pstTimerData->ulStartTime + pstTimerData->uTimeLen;
                if(wqEndTime <= wqTime)
                {
                    dtDataTimerData.erase(itTimerData);
                    //::TODO
                }
            }
        }
    }   
    WMUnLock(mutexLock);
    return wmTRUE;
}

wmBOOL CServerTimer::addBufferDeque(stTimerData *pstTimerData)
{
    if(wmNULL == pstTimerData)
    {
        return wmFALSE;
    }
    WMLock(mutexLock);
    dtDataTimerData.push_back(pstTimerData); 
    WMUnLock(mutexLock);
    return wmTRUE;
}


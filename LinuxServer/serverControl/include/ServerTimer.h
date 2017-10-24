/**
 * @file ServerTimer.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __SERVER_TIMER_H__
#define __SERVER_TIMER_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseThread.h"
#include "BaseLock.h"

typedef struct stTimerData
{
    wmINT iType;
    wmSIZET sBuffSize;
    wmULONG uTimeLen;
    wmQWORD ulStartTime;
    wmVOID* pBuff;
    
    void Init()
    {
        pBuff = wmNULL;
        sBuffSize = 0;
        iType = 0;
        ulStartTime = 0;
        uTimeLen = 0;
    }

    stTimerData()
    {
        Init();
    }
}stTimerData;

class CServerTimer: public TInstance<CServerTimer>
{
    friend class TInstance<CServerTimer>;
public:
    // Server Timer Init
    // return: true is set success,  false is other error
    wmBOOL ServerTimerInit();
    wmBOOL ServerTimerClear();
    wmBOOL addBufferDeque(stTimerData *pstTimerData);
    
private:
    CServerTimer();
    wmLock mutexLock;
    Thread  tThread;
    wmBOOL bWantToStopTimer;

    wmVECTOR<stTimerData *> dtDataTimerData;

    static wmINT ThreadProcTimer(void *pThis);
    wmBOOL ProcessTimerPool();
    
};


#endif  // end __SERVER_TIMER_H__
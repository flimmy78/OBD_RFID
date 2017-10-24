#include "BaseLock.h"
#include "BaseSize.h"
#include "BaseMemory.h"
#include "BaseDebug.h"
#include <errno.h>

#ifdef _WINDOWS_OS_

#else
#include <err.h>
#endif

#define WM_OBJECTNAME_MAX    32
#define WM_PATHLEN_MAX       256
#define WM_STACKSZ_DEFAULT   32*1024

typedef struct _tagwmLockObj
{
    wmINT iLockCnt;
    wmLONG lOwnerId;
#ifdef _WINDOWS_OS_
	CRITICAL_SECTION mutex;
#else
	pthread_mutex_t mutex;
#endif
    wmCHAR szMutexName[WM_OBJECTNAME_MAX+1];

} wmLockObj;

wmLock WMCreateLock(const wmCHAR * pName, wmINT iFlag)
{
    wmLockObj * pLockObj = (wmLockObj *)DwmMalloc(sizeof(wmLockObj));

    if (pLockObj == wmNULL)
    {
        DWMERROR("Malloc Lock Object fail!");
        return wmNULL;
    }

    memset(pLockObj, 0x00, sizeof(wmLockObj));
    if (pName != NULL)
    {
        strncpy(pLockObj->szMutexName, pName, WM_OBJECTNAME_MAX);
    }
    else
    {
        strncpy(pLockObj->szMutexName, "NULL", WM_OBJECTNAME_MAX);
    }

#ifdef _WINDOWS_OS_
	::InitializeCriticalSection(&pLockObj->mutex);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, iFlag);
    int ok = pthread_mutex_init(&pLockObj->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    if (ok != 0)
    {
        DWMERROR("Init mutex fail %d!", ok);
        DwmFree(pLockObj);
        return wmNULL;
    }
#endif
    return (wmLock)pLockObj;
}

wmVOID WMDestroyLock(wmLock lock)
{
    wmLockObj * pLockObj = (wmLockObj *)lock;
    if (pLockObj == wmNULL) return;

    if (pLockObj->iLockCnt > 0)
    {
        DWMWORNG("The lock to be destroy is in locked %s", pLockObj->szMutexName);
    }

#ifdef _WINDOWS_OS_
	::DeleteCriticalSection(&pLockObj->mutex);
#else
    int ok = pthread_mutex_destroy(&pLockObj->mutex);

    if (ok != 0)
    {
        DWMWORNG("Destroy mutex fail %s:%d", pLockObj->szMutexName, ok);
    }
#endif

    DwmFree(pLockObj);
}

wmBOOL WMLock(wmLock lock)
{
    wmLockObj * pLockObj = (wmLockObj *)lock;
    if (pLockObj == wmNULL) return wmFALSE;

#ifdef _WINDOWS_OS_
	::EnterCriticalSection(&pLockObj->mutex);
#else
    int ok = pthread_mutex_lock(&pLockObj->mutex);

    if (ok != 0)
    {
        DWMERROR("Lock mutex fail %d!", ok);
        return wmFALSE;
    }

    pLockObj->iLockCnt++;
    pLockObj->lOwnerId = pthread_self();
#endif
    return wmTRUE;
}

wmBOOL WMTryLock(wmLock lock)
{
    wmLockObj * pLockObj = (wmLockObj *)lock;
    if (pLockObj == wmNULL) return wmFALSE;

#ifdef _WINDOWS_OS_
	::TryEnterCriticalSection(&pLockObj->mutex);
#else
    int ok = pthread_mutex_trylock(&pLockObj->mutex);

    if (ok != 0)
    {
        DWMERROR("Trylock mutex fail %d!", ok);
        return wmFALSE;
    }

    pLockObj->iLockCnt++;
    pLockObj->lOwnerId = pthread_self();
#endif
    return wmTRUE;
}

wmBOOL WMUnLock(wmLock lock)
{
    wmLockObj * pLockObj = (wmLockObj *)lock;
    if (pLockObj == wmNULL) return wmFALSE;
#ifdef _WINDOWS_OS_
	::LeaveCriticalSection(&pLockObj->mutex);
#else
    pthread_mutex_unlock(&pLockObj->mutex);
    pLockObj->iLockCnt--;
#endif
    return wmTRUE;
}


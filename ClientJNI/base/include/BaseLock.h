/**
 * @file BaseLock.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */


#ifndef __LOCK_MUTEX_H__
#define __LOCK_MUTEX_H__
#include "BaseDefine.h"

#ifdef _WINDOWS_OS_
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef wmVOID*     wmLock;

#ifdef _WINDOWS_OS_
#define WM_LOCK_TIMED           0
#define WM_LOCK_RECURSIVE       1
#define WM_LOCK_ERRORCHECK      2
#define WM_LOCK_ADAPTIVE        3
#else
#define WM_LOCK_TIMED           PTHREAD_MUTEX_NORMAL
#define WM_LOCK_RECURSIVE       PTHREAD_MUTEX_RECURSIVE
#define WM_LOCK_ERRORCHECK      PTHREAD_MUTEX_ERRORCHECK
#define WM_LOCK_ADAPTIVE        PTHREAD_MUTEX_DEFAULT
#endif

wmLock WMCreateLock(const wmCHAR * pName, wmINT iFlag = WM_LOCK_TIMED);
wmVOID WMDestroyLock(wmLock lock);
wmBOOL WMLock(wmLock lock);
wmBOOL  WMTryLock(wmLock lock);
wmBOOL  WMUnLock(wmLock lock);

#endif  /* __LOCK_MUTEX_H__ */

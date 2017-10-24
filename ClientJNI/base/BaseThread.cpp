#include "BaseThread.h"
#include "BaseDefine.h"
#include <stdlib.h>

#ifdef _WINDOWS_OS_
#include <windows.h>
#else
#include <pthread.h>
#endif

Thread::Thread(ThreadFunc func)
    : threadFunc(func)
    , pData(wmNULL)
{
}

wmBOOL Thread::start(wmVOID *pParam)
{
	if (pData) {
#ifdef _WINDOWS_OS_
		DWORD code;
		if (::GetExitCodeThread(pData, &code) && (code != STILL_ACTIVE)) {
			pData = NULL;
		}
#endif
	}
    
    wmBOOL bSucceeded = wmTRUE;

	if (!pData) {

#ifdef _WINDOWS_OS_
		pData = CreateThread(NULL, 0,
							  reinterpret_cast<LPTHREAD_START_ROUTINE>(threadFunc),
							  pParam, 0, NULL);
		bSucceeded = (pData != NULL);
#else
    bSucceeded = pthread_create(reinterpret_cast<pthread_t *>(&pData),
                                   wmNULL,
                                            reinterpret_cast<void *(*)(void *)>(threadFunc),
                                            pParam) == 0;
#endif
	}
    
    return bSucceeded;
}

wmINT Thread::join()
{
    wmINT ret = 0;

    if (pData) {
#ifdef _WINDOWS_OS_
		WaitForSingleObject(pData, INFINITE);

		::GetExitCodeThread(pData, reinterpret_cast<DWORD *>(&ret));
#else
		pthread_join(reinterpret_cast<pthread_t>(pData), wmNULL);
#endif       
        pData = wmNULL;
    }

    return ret;
}

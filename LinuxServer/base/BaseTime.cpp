/**
 * @file BaseTime.cpp
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */
#include "BaseTime.h"
#include <signal.h>

#if defined(_WINDOWS_OS_)
#include <windows.h>
#include <time.h>
#else
#include <unistd.h>
#include <time.h>
#endif


wmQWORD MPGetMonotonic()
{
#if defined(_WINDOWS_OS_)
    SYSTEMTIME t;
    GetSystemTime(&t);
    uint64_t seconds = t.wHour*3600+t.wMinute*60+t.wSecond;
    return seconds * 1000000000 + t.wMilliseconds;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((wmQWORD)ts.tv_sec) * 1000000000 + ts.tv_nsec;
#endif
}

wmQWORD MPGetTimestamp()
{
    return MPGetMonotonic() / 1000;
}

wmVOID getTimeValue(wmCHAR* pTime)
{
    if(wmNULL == pTime)
        return;
    time_t tim;
    time(&tim);
    struct tm *p;
    p = gmtime(&tim);
    sprintf(pTime,"%d-%d-%d %d:%d:%d",1900 + p->tm_year, 1+ p->tm_mon, p->tm_mday,p->tm_hour,p->tm_min, p->tm_sec);
}

wmVOID DwmSleep(wmU32T milliseconds)
{
    try  
    {
#if defined(_WINDOWS_OS_)
    ::Sleep(milliseconds);
#else
    ::sleep(milliseconds / 1000);
#endif
    }
    catch(int&  value)
    {
        printf("sleep error ,%d\n",value);
    }
}
 




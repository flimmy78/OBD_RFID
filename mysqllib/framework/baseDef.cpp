/**
 * @file baseDef.cpp
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#include "baseDef.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>


#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define snprintf _snprintf
using namespace std;

void *DwmMalloc(wmULONG ulSize)
{
    void *ptr = wmNULL;
    ptr = malloc(ulSize);
    return ptr;
}
 
wmVOID *DwmRealloc(wmVOID *ptr, wmUINT uiSize)
{
    if(wmNULL == ptr)
        return wmNULL;
    return realloc(ptr, uiSize);
}
 
wmVOID DwmFree(wmVOID *ptr)
{
    if(wmNULL == ptr)
        return;
    free(ptr);
    ptr = NULL;
}
 
wmVOID DwmMemset(wmVOID * ptr, wmINT value, wmULONG ulSize)
{
    if(wmNULL == ptr)
        return;
    memset(ptr, value, ulSize);
}
 
wmVOID DwmMemcpy(wmVOID *desptr, wmVOID const *originptr, wmULONG offset, wmULONG length)
{
    if(wmNULL == desptr)
        return;
    if(wmNULL == originptr)
        return;
    memcpy(desptr, reinterpret_cast<char const *>(originptr) + offset, length);
}
 
wmVOID DwmSleep(wmU32T milliseconds)
{
    try  
    {
#if defined(_P_WINDOWS_)
    ::Sleep(milliseconds);
#elif defined(_P_LINUX_) || defined(_P_ANDROID_)
    ::sleep(milliseconds / 1000);
#endif
    }
    catch(int&  value)
    {
        printf("sleep error ,%d\n",value);
    }
}
 
wmQWORD DwmGetMonotonic()
{
#ifdef _P_WINDOWS_
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((wmQWORD)ts.tv_sec) * 1000000000 + ts.tv_nsec;
#endif
    return 0;
}

wmQWORD GenerateSSRC()
{
#ifdef _P_WINDOWS_
    return 0;
#else
    return DwmGetMonotonic();
#endif
}
 
wmQWORD GetTimestamp()
{
#ifdef _P_WINDOWS_
    return 0;
#else
    return DwmGetMonotonic() / 1000;
#endif
}
 
wmVOID DwmGetGUID(wmCHAR * pBuf)
{
    if(wmNULL == pBuf)
    {
        return ;
    }
#ifdef _P_WINDOWS_
    GUID guid; 
    if(S_OK ==::CoCreateGuid(&guid))   
    {        
       _snprintf(pBuf, sizeof(pBuf), 
        "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X", 
        guid.Data1, guid.Data2, guid.Data3, 
        guid.Data4[0], guid.Data4[1], guid.Data4[2], 
        guid.Data4[3], guid.Data4[4], guid.Data4[5], 
        guid.Data4[6], guid.Data4[7]);
     }  
#endif
}
 
wmVOID DwmAllocpy(wmCHAR* &to, const wmCHAR * from)
{
    if(strlen(from)<=0)
        return;
    to = (wmCHAR*)malloc(strlen(from)+1);
    *(to+strlen(from)) = 0;
    ::DwmMemcpy(to, from, 0, strlen(from));
}

#define ANDLOG(...)  __android_log_print(ANDROID_LOG_DEBUG,"DwmDebug",__VA_ARGS__)
 
wmVOID DwmLog(wmCHAR const* cata,wmCHAR const* pFilename,wmINT line, wmCHAR  const *tag , wmCHAR  const *info)
{
    if(wmNULL == cata)
        return;
    if(wmNULL == pFilename)
        return;
    if(wmNULL == tag)
        return;
    if(wmNULL == info)
        return;
    wmBOOL bError = wmFALSE; 
    if(strcmp("ERROR",cata) == 0 )
    {
        bError = wmTRUE;
    }

    wmCHAR pTime[TIME_STRING_LEN];
    wmCHAR pStringLog[INTERFACE_STRING_LEN];
    memset(pTime,0x0,sizeof(pTime));
    memset(pStringLog,0x0,sizeof(pStringLog));
    time_t tim;
    time(&tim);
    DwmMemcpy(pTime, ctime(&tim), 0, strlen(ctime(&tim))-1);
    DwmSnprintf(pStringLog, INTERFACE_STRING_LEN, "%s[%s]%s:%d %s %s",pTime,cata,pFilename,line,tag,info);
    printf("%s\n",pStringLog);
}
 
wmVOID DwmDeleteLog()
{
#ifdef _P_WINDOWS_
    DwmFileRemove("E:/dwm.log");
#endif
}
 
wmBOOL DwmFileCreate(wmCHAR const *pFilename)
{
    if(wmNULL == pFilename)
        return wmFALSE;
    FILE *file = ::fopen(pFilename, "wb");
    if (file) {
        ::fclose(file);
    }
    return (file != NULL);
}
 
wmBOOL DwmFileExists(wmCHAR const *pFilename)
{
    if(wmNULL == pFilename)
        return wmFALSE;
    FILE *file = ::fopen(pFilename, "rb");
    if (file) {
        ::fclose(file);
    }
    return (file != NULL);
}
 
wmBOOL DwmFileMvname(wmCHAR const *pFilename,wmCHAR const *pNewFilename)
{
    if(wmNULL == pFilename || wmNULL == pNewFilename)
        return wmFALSE;
    return wmFALSE;
}
 
wmBOOL DwmFileRemove(wmCHAR const *pFilename)
{
    if(wmNULL == pFilename)
        return wmFALSE;
    return (::remove(pFilename) == 0);
}
 
wmCHAR *DwmStrcpy(wmCHAR *to, wmCHAR const *from)
{
    if(wmNULL == to)
        return wmNULL;
    if(wmNULL == from)
        return wmNULL;
    return strcpy(to, from);
}
 
wmCHAR *DwmSafeStrcpy(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *src)
{
    if(wmNULL == buffer)
        return wmNULL;
    if(wmNULL == src)
        return wmNULL;
    ::DwmSnprintf(buffer, bufsize, "%s", src);
    return buffer;
}
 
wmSIZET DwmStrlen(wmCHAR const *str)
{
#ifdef _P_WINDOWS_
    return strlen(str);
#else
    return ::strnlen(str,FILE_STRING_LEN);
#endif
}

wmVOID DwmStrncat(wmCHAR *dest, wmCHAR const *src, wmSIZET bufsize)
{
    if(wmNULL == dest || wmNULL == src)
        return;
    strncat(dest,src,bufsize-DwmStrlen(dest));
}
 
 
wmINT DwmSnprintf(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *format, ...)
{
    va_list va;
    va_start(va, format);
    wmINT r = vsnprintf(buffer, bufsize, format, va);
    va_end(va);
    return r;
}
 
 
wmVOID DwmTime(wmCHAR* pTimestr)
{
    if(NULL == pTimestr)
        return;
#ifdef _P_WINDOWS_
    time_t tim;
    time(&tim);
    ::DwmMemcpy(pTimestr, ctime(&tim), 0, strlen(ctime(&tim)));
#else
    time_t tim;
    time(&tim);
    ::DwmMemcpy(pTimestr, ctime(&tim), 0, strlen(ctime(&tim)));
#endif
}
 
wmFLOAT DwmValueClamp(wmFLOAT val, wmFLOAT minVal, wmFLOAT maxVal)
{
    using namespace std;   
    return max(minVal, min(val, maxVal));
}

wmU8T  read8 (wmVOID const *data, wmINT offset)
{
    return *(reinterpret_cast<wmU8T const *>(data) + offset);
}
 
wmU16T read16(wmVOID const *data, wmINT offset)
{
    wmU16T value;
    DwmMemcpy(&value, data, offset, sizeof value);
    return ntohs(value);
}
 
wmUINT read32(wmVOID const *data, wmINT offset)
{
    wmUINT value;
    DwmMemcpy(&value, data, offset, sizeof value);
    //return::ntohl(value);
    return value;
}

wmVOID Write8 (wmCHAR *data, wmINT offset, wmU8T  value)
{
    *reinterpret_cast<wmU8T *>(data + offset) = value;
}
 
wmVOID Write16(wmCHAR *data, wmINT offset, wmU16T value)
{
    value = htons(value);
    DwmMemcpy(data + offset, &value, 0, sizeof value);
}
 
wmVOID Write32(wmCHAR *data, wmINT offset, wmINT value)
{
    //value = ::htonl(value);
    DwmMemcpy(data + offset, &value, 0, sizeof value); 
}
 

   

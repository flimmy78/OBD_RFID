/**
 * @file baseDebug.cpp
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */
#include "BaseDebug.h"
#include "BaseSize.h"
#include "BaseTime.h"
#include <stdio.h>
#include <time.h>
#include <cstring>
#include <math.h>
#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <fcntl.h>

#ifdef _WINDOWS_OS_
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef _ANDROID_OS_
#include <jni.h>
#include <android/log.h>
#define ANDLOG(...)  __android_log_print(ANDROID_LOG_DEBUG,"DwmDebug",__VA_ARGS__)
#endif

wmINT DwmSnprintf(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *format, ...)
{
    va_list va;
    va_start(va, format);
    wmINT r = vsnprintf(buffer, bufsize, format, va);
    va_end(va);
    return r;
}

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
    
#ifdef _WINDOWS_OS_
    wmCHAR localtime[64] = {0};
    getTimeValue(localtime);
    localtime[strlen(localtime)-1] = 0;
    FILE *pFile = ::fopen("E:\\dwm.log", "a+");
    if(pFile)
    {
        ::fprintf(pFile,"%s[%s]%s:%d %s %s \n",localtime,cata,pFilename,line,tag,info);
        ::fclose(pFile);  
    }
#endif

#ifdef _ANDROID_OS_
    wmCHAR localtime[64] = {0};
    getTimeValue(localtime);
    ANDLOG("[%s][%s]%s:%d %s %s\n",localtime,cata,pFilename,line,tag,info);
#endif

#ifdef _LINUX_OS_
    wmCHAR localtime[64] = {0};
    getTimeValue(localtime);
    printf("[%s][%s]%s:%d %s %s\n",localtime,cata,pFilename,line,tag,info);
#endif
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


wmBOOL DwmStrcmp(wmCHAR const *dest, wmCHAR const *src)
{
    if(wmNULL == dest || wmNULL == src)
        return wmFALSE; 
    return (((strcmp(dest,src)) == wmFalse)?wmTRUE:wmFALSE);
}

wmINT Get16to10(wmINT i16bIT)
{
    if(i16bIT > 0x63)
    {
        return 0x99;
    }

    if(i16bIT < 0x01)
    {
        return 0x0;
    }
    return ((i16bIT/0x0a)*0x10 + (i16bIT - (i16bIT/0x0a)*0x0a));
}


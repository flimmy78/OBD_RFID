/**
 * @file baseDebug.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_DEBUG_H_
#define _BASE_DEBUG_H_
#include "BaseDefine.h"


#if defined(_WINDOWS_OS_)
#include <fcntl.h>
#include <string.h>

#define DWMDEBUG(...)                    \
    do {                            \
         FILE *pFile = ::fopen("E:\\dwm.log", "a+"); \
        if(pFile){ \
            ::fprintf(pFile,"\n[DEBUG][Line:%d][%s]", __LINE__, __FUNCTION__); \
            ::fprintf(pFile,__VA_ARGS__); \
            ::fclose(pFile);   \
        } \
    } while (wmFALSE)


#define DWMWORNG(...)                    \
    do {                            \
         FILE *pFile = ::fopen("E:\\dwm.log", "a+"); \
        if(pFile){ \
            ::fprintf(pFile,"\n[---------WRONG---------][Line:%d][%s]", __LINE__, __FUNCTION__); \
            ::fprintf(pFile,__VA_ARGS__); \
			printf(__VA_ARGS__); \
			printf("\n"); \
            ::fclose(pFile);   \
        } \
    } while (wmFALSE)
    

#define DWMERROR(...)  \
    do {                            \
         FILE *pFile = ::fopen("E:\\dwm.log", "a+"); \
        if(pFile){ \
            ::fprintf(pFile,"\n*********ERROR*********][Line:%d][%s]", __LINE__, __FUNCTION__); \
            ::fprintf(pFile,__VA_ARGS__); \
			printf(__VA_ARGS__); \
			printf("\n"); \
            ::fclose(pFile);   \
        } \
    } while (wmFALSE)
#else

#if defined(DEBUGLOG)
#define DWMDEBUG(...)                    \
    do {                            \
        wmCHAR* pBuffer = (wmCHAR*)malloc(1024*1024);        \
        if(wmNULL == pBuffer)  break; \
        memset(pBuffer, 0x0, sizeof(pBuffer)); \
        ::DwmSnprintf(pBuffer, 1024*1024, __VA_ARGS__);    \
        ::DwmLog("DEBUG",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
        free(pBuffer);  \
        pBuffer = NULL;  \
    } while (wmFALSE)

#define DWMWORNG(...)                    \
    do {                            \
        wmCHAR* pBuffer = (wmCHAR*)malloc(1024*1024);        \
        if(wmNULL == pBuffer)  break; \
        memset(pBuffer, 0x0, sizeof(pBuffer)); \
        ::DwmSnprintf(pBuffer, 1024*1024, __VA_ARGS__);    \
        ::DwmLog("---------WRONG---------",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
        free(pBuffer);  \
        pBuffer = NULL;  \
    } while (wmFALSE)

#define DWMERROR(...)                    \
    do {                            \
        wmCHAR* pBuffer = (wmCHAR*)malloc(1024*1024);        \
        if(wmNULL == pBuffer)  break; \
        memset(pBuffer, 0x0, sizeof(pBuffer)); \
        ::DwmSnprintf(pBuffer, 1024*1024, __VA_ARGS__);    \
        ::DwmLog("**********ERROR*********",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
        free(pBuffer);  \
        pBuffer = NULL;  \
    } while (wmFALSE)
#else
#define DWMDEBUG(...) 
#define DWMWORNG(...)
#define DWMERROR(...)                    \
    do {                            \
        wmCHAR* pBuffer = (wmCHAR*)malloc(1024*1024);        \
        if(wmNULL == pBuffer)  break; \
        memset(pBuffer, 0x0, sizeof(pBuffer)); \
        ::DwmSnprintf(pBuffer, 1024*1024, __VA_ARGS__);    \
        ::DwmLog("**********ERROR*********",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
        free(pBuffer);  \
        pBuffer = NULL;  \
    } while (wmFALSE)
#endif

#endif

#define DwmStrncatMax(dest, bufsize, ...) \
do { \
      wmCHAR* pBuffer = (wmCHAR*)malloc(1024);        \
      if(wmNULL == pBuffer)  break; \
      memset(pBuffer, 0x0, sizeof(pBuffer)); \
      ::DwmSnprintf(pBuffer, 1024, __VA_ARGS__);    \
      ::DwmStrncat(dest,pBuffer,bufsize); \
      free(pBuffer);  \
      pBuffer = NULL;  \
} while (wmFALSE)
      
        
wmINT DwmSnprintf(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *format, ...);
wmVOID DwmLog(wmCHAR const* cata,wmCHAR const* pFilename,wmINT line, wmCHAR  const *tag , wmCHAR  const *info);
wmCHAR *DwmStrcpy(wmCHAR *to, wmCHAR const *from);
wmCHAR *DwmSafeStrcpy(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *src);
wmSIZET DwmStrlen(wmCHAR const *str);
wmVOID DwmStrncat(wmCHAR *dest, wmCHAR const *src, wmSIZET bufsize);
wmBOOL DwmStrcmp(wmCHAR const *dest, wmCHAR const *src);
wmINT Get16to10(wmINT i16bIT);

#endif //end _BASE_DEBUG_H_


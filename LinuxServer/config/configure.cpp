#include "configure.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseDebug.h"
#include "BaseType.h"
#include "BaseFile.h"
#include <fcntl.h>
#include <string>

#define JS_INIT_STR "{\"serverport\":null}"
#define CONFIGURE_FILE "/etc/configure.json" 
#define File_OPEN_FLAG_RB "rb"
#define File_OPEN_FLAG_WB "rb"

wmBOOL CConfigure::ConfigureInit()
{
    DWMDEBUG("");
#ifdef _WINDOWS_OS_
     if(WMFileExist(CONFIGURE_FILE))
    {
        wmQWORD qwFileSize = WMFileSize(CONFIGURE_FILE);

        wmVOID* pFile = wmOpen(CONFIGURE_FILE, File_OPEN_FLAG_RB);
        if(pFile)
        {
            wmCHAR* pJsData = (wmCHAR*)DwmMalloc((wmULONG)qwFileSize+1);
            DwmMemset(pJsData, 0x0, sizeof(pJsData));
            wmULONG ulReadSize = wmRead(pFile,pJsData,(wmULONG)qwFileSize);
            if(ulReadSize > wmFalse)
            {
                std::string sJsData = pJsData;
				sJsData.assign( pJsData, pJsData + sJsData.rfind("}")+1);
                jsData.init(sJsData.c_str());
                printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());
                wmClose(pFile);
                DwmFree(pJsData);
                return wmTRUE;
            }
            wmClose(pFile);
            DwmFree(pJsData);
        }
    }
    jsData.init(JS_INIT_STR);
    printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());
    return wmFALSE;
#else
    if(WMFileExist(CONFIGURE_FILE))
    {
        wmQWORD qwFileSize = WMFileSize(CONFIGURE_FILE);

        wmINT iFd = wmOpen(CONFIGURE_FILE,O_RDONLY);
        if(iFd != ErrorCodeALL)
        {
            wmCHAR* pJsData = (wmCHAR*)DwmMalloc(qwFileSize+1);
            DwmMemset(pJsData, 0x0, sizeof(pJsData));
            wmINT iError = wmRead(iFd,pJsData,qwFileSize);
            if(iFd != ErrorCodeALL)
            {
                jsData.init(pJsData);
                printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());
                wmClose(iFd);
                DwmFree(pJsData);
                return wmTRUE;
            }
            wmClose(iFd);
            DwmFree(pJsData);
        }
    }
    jsData.init(JS_INIT_STR);
    printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());
    return wmFALSE;
#endif
}

wmCCHAR* CConfigure::Get(wmCCHAR* pTarget)
{
    DWMDEBUG("");
    if(wmNULL == pTarget)
    {
        DWMERROR("[pTarget ]  = NULL ");
        return wmNULLCCSTR;
    }

    if(!jsData.isKeyExist(pTarget))
    {
        DWMERROR("pTarget key [%s]  = NULL ",pTarget);
        return wmNULLCCSTR;
    }

    if(wmNULL == jsData[pTarget].toStr())
    {
        DWMERROR("Target [%s]  = NULL",pTarget);
        return wmNULLCCSTR;
    }
    return jsData[pTarget].toStr();
}

wmBOOL CConfigure::Set(wmCCHAR* pTarget, wmCCHAR* pValue)
{
    DWMDEBUG("");
    if((wmNULL == pTarget) || (wmNULL == pValue))
    {
        DWMERROR("[pTarget || pValue]  = NULL ");
        return wmFALSE;
    }

    if(!jsData.isKeyExist(pTarget))
    {
        DWMERROR("[pTarget key]  = NULL ");
        return wmFALSE;
    }

    jsData[pTarget] = pValue;

    return ConfigureClose();
}

wmBOOL CConfigure::ConfigureClose()
{
    DWMDEBUG("");
#ifdef _WINDOWS_OS_
    if(WMFileExist(CONFIGURE_FILE))
    {
        wmDelete(CONFIGURE_FILE);     
    }

    printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());

    wmVOID* pFile = wmOpen(CONFIGURE_FILE,File_OPEN_FLAG_WB);
    if(pFile)
    {
        wmINT iError = wmWrite(pFile,jsData.c_str(),jsData.getStrLen());
        if(pFile > wmFalse)
        {
            wmClose(pFile);
            return wmTRUE;
        }
        wmClose(pFile);
    }    
    return wmFALSE;
#else
    if(WMFileExist(CONFIGURE_FILE))
    {
        wmDelete(CONFIGURE_FILE);     
    }

    printf("jsData = [%s][%d]\n",jsData.c_str(),jsData.getStrLen());

    wmINT fd = wmOpen(CONFIGURE_FILE,O_RDWR);
    if(ErrorCodeALL != fd)
    {
        wmINT iError = wmWrite(fd,jsData.c_str(),jsData.getStrLen());
        if(ErrorCodeALL != iError)
        {
            wmClose(fd);
            return wmTRUE;
        }
        wmClose(fd);
    }     
    return wmFALSE;
#endif   
}


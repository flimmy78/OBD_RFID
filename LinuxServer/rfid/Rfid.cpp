#include "Rfid.h"
#include "BaseTime.h"
#include "BaseDebug.h"
#include "BaseSize.h"
#include "BaseType.h"

#define RFID_BIT_0 0
#define RFID_BIT_1 1

CRfid::CRfid()
{
    
}

CRfid::~CRfid()
{
    
}

static wmINT GetEPCLen(wmU8T pc_msb, wmU8T pc_lsb)
{
    wmINT num2 = 0;
    num2 = (wmINT) ((pc_msb << 8) + pc_lsb);
    return (wmINT) ((num2 >> 11) * 2);
}
    
static wmVOID GetID(wmCHAR* pEpc, wmU8T* pBuf, wmINT iStart, wmU8T iEpcLen)
{
    if(wmNULL == pEpc || wmNULL == pBuf)
    {
        return;
    }
    
    FOR (iEpcLen)
    {
        wmCHAR rStr[0xa] = {0};
        DwmSnprintf(rStr, sizeof(rStr), "%02X",pBuf[iStart + i]);
        DwmStrncat(pEpc,rStr,DwmStrlen(rStr));
        if(i < (iEpcLen -1))
            DwmStrncat(pEpc,"-",DwmStrlen("-"));
    }
}

wmVOID CRfid::SwitchRfid(wmCHAR* pSrcData, wmINT iLen, wmUSHORT& sDirType, wmCHAR* pDirData)
{
    if(wmNULL == pSrcData  || wmNULL == pDirData)
    {
        DWMERROR("pSrcData or pDirData is null");
        return;
    }
    
    wmU8T pBuf[BASE_STRING_LEN_MIN] = {0};
    FOR(iLen)
    {
        DwmMemcpy(pBuf+i, &pSrcData[i], 0x0,sizeof(wmU8T)); 
    }

    sDirType = pBuf[RFID_BIT_1];

    switch(pBuf[RFID_BIT_1])
    {
        case EN_RFID_CODE_EVENT_NETINFO:
        case EN_RFID_CODE_EVENT_NETINFO_ALL:
        {
            DwmSnprintf(pDirData, BASE_STRING_LEN_MIN, "%02X:%02X:%02X:%02X:%02X:%02X",
            pBuf[3],pBuf[4],pBuf[5],pBuf[6],pBuf[7],pBuf[8]);
            break;
        }
        case EN_RFID_CODE_EVENT_HEARBAET:
        {
            
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_AUTO:
        case EN_RFID_CODE_EVENT_EPC_AUTO_STOP:
        {
            wmINT iEpc_len = GetEPCLen(pBuf[3], pBuf[4]);
            GetID(pDirData,pBuf,5,iEpc_len);
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_DATA:
        {
            wmUSHORT num2 = ((wmUSHORT) ((pBuf[4] << 8) | pBuf[5]))*2;
            
            
            if(0x20 == num2)
            {
                wmCHAR pCurrentValue[0x0a] = {0};
                DwmSnprintf(pCurrentValue, BASE_STRING_LEN_MIN, "%02X%02X%02X%02X%02X",
                pBuf[6],pBuf[7],pBuf[8],pBuf[9],pBuf[10]);
                DwmSnprintf(pDirData, BASE_STRING_LEN_MIN, "%d",atoi(pCurrentValue));
                DWMDEBUG("[%s][%s]",pCurrentValue,pDirData);
            }
            else
            {
                GetID(pDirData,pBuf,6,num2);
                sDirType = EN_RFID_CODE_EVENT_EPC_DATA_EPCID;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}


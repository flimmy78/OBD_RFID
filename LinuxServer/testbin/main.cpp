#include "stdio.h"
#include "BaseDefine.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "BaseVector.h"
#include "BaseType.h"
#include "ServerControl.h"

wmINT calculate_pc(wmU8T pc_msb, wmU8T pc_lsb)
{
	wmINT num2 = 0;
	num2 = (wmINT) ((pc_msb << 8) + pc_lsb);
	return (wmINT) ((num2 >> 11) * 2);
}

wmVOID epc_format(wmCHAR* epcstr, wmU8T* epc, wmINT startadd, wmU8T epc_len)
{
    if(wmNULL == epcstr || wmNULL == epc)
    {
        return;
    }
    for (int i = 0; i < epc_len; i++)
    {
        char rStr[10] = {0};
        DwmSnprintf(rStr, sizeof(rStr), "%02X",epc[startadd + i]);
        DwmStrncat(epcstr,rStr,strlen(rStr));
        if(i < (epc_len -1))
            DwmStrncat(epcstr,"-",strlen("-"));
    }
}

wmVOID Get16BitData(wmU8T* Msgbody, wmU8T& msglen, wmULONG ulValue, wmU8T& iRetValue)
{
    msglen = 0;
    iRetValue = 59;
    for(int i = 15; i < 20 ; i++)
    {
        if(ulValue/0xff > 0)
        {
            Msgbody[i] = ulValue - ulValue/0xff*0xff;
            ulValue/=0xff;
            DWMDEBUG("Msgbody[i] = [%02X][%d]",Msgbody[i],Msgbody[i]);
            iRetValue+= Msgbody[i] ;
            msglen++;
        }
        else
        {
            Msgbody[i] = ulValue;
            iRetValue+= Msgbody[i] ;
            DWMDEBUG("Msgbody[i] = [%02X][%d]",Msgbody[i],Msgbody[i]);
            msglen++;
            break;
        }
    }
}


int main()
{
    CServerControl* pCServerControl = CServerControl::GetInstance();
    if(wmNULL == pCServerControl)
    {
        DWMERROR("CServerControl::GetInstance error");
    }
    while(1)
    {
        wmINT iInput = wmFalse;
        DWMDEBUG("############:");
        scanf("%d",&iInput);
        DWMDEBUG("[%d]",iInput);
        switch(iInput)
        {
        case 0x01:
        {
            pCServerControl->CServerControlInit();
            break;
        }
        case 0x02:
        {
            pCServerControl->RFIDAutoScan();
            break;
        }
        case 0x03:
        {
            pCServerControl->RFIDStopScan();
            break;
        }
        case 0x04:
        {
            DWMDEBUG("------------:");
            
            wmCHAR pInput[12] = {0};
            scanf("%s",pInput);
            wmINT iRet = atoi(pInput);
            if(iRet > 0)
            {
                DWMDEBUG("iRet = [%d]",iRet);
                pCServerControl->RFIDWrite(iRet);
            }
            break;
        }
        case 0x05:
        {
            pCServerControl->RFIDReadData();
            break;
        }
        case 0x06:
        {
            pCServerControl->RFIDReadEPCID();
            break;
        }
        default:
        {
            break;
        }

        }
    }
   return 0;
}

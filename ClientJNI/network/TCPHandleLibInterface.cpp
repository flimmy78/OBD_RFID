#include "TCPHandleLibInterface.h"
#include "TCPHandle.h"
#include "BaseDebug.h"

static TCPHandle* pTCPHandle = wmNULL;

extern "C" {
bool TCPClientLibInitialze(CALLBACK_RECEIV_BUFF pCbReceivedBuff)
{
    DWMDEBUG("");
    
    if(wmNULL ==  pTCPHandle)
    {
        pTCPHandle = new TCPHandle();
        if(pTCPHandle)
        {
            pTCPHandle->Init(pCbReceivedBuff);
            return wmTRUE;
        }
    }
    return wmFALSE;
}

bool ConnectToServer(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort)
{
    DWMDEBUG("");
    if(wmNULL ==  pTCPHandle)
    {
        DWMERROR("pTCPHandle is null");
        return wmFALSE;
    }

    wmBOOL bConnect = pTCPHandle->Connect(pConnectAddr,usConnectPort);
    if(wmFALSE == bConnect)
    {
        DWMERROR("Connect server was failed");
    }
    return bConnect;
}
    
bool SendDataToServer(wmINT iType, wmCCHAR *pBuff, wmU32T sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2)
{
    DWMDEBUG("");
    if(wmNULL ==  pTCPHandle)
    {
        DWMERROR("pTCPHandle is null");
        return wmFALSE;
    }

    if(wmNULL ==  pBuff || sBuffLen < wmTrue)
    {
        DWMERROR("pBuff is null");
        return wmFALSE;
    }

    return pTCPHandle->SendDataNow(iType,pBuff,sBuffLen,iDest,iValue1,iValue2);
}

bool TCPClientLibExit()
{
    if(pTCPHandle)
    {
        pTCPHandle->Close();
    }
    delete pTCPHandle;
    pTCPHandle = wmNULL;
    return wmTRUE;
}

}  // extern "C"

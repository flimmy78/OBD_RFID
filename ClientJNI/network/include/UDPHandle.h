#ifndef _UDPHandle_H_
#define _UDPHandle_H_
#include "BaseDefine.h"
#include "BaseEvent.h"
#include "BaseThread.h"
#include "BaseVector.h"
#include "BaseZip.h"
#include "BaseUnZip.h"
#include "BaseType.h"

class UDPHandle 
{
public:
    UDPHandle();
    ~UDPHandle();

    wmBOOL InitHandle(CALLBACK_RECEIV_BUFF pCbReceivedBuff, wmUSHORT usSendtoPort);
    wmBOOL SendData(wmCHAR const *pBuff, wmSIZET sBuffLen, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort);
    wmBOOL CloseHandle();
private:
    wmBOOL bIsActivedPort(wmUSHORT usSendtoPort);
	
    wmINT iSocket;
    wmBOOL bThreadStatus;
    
    Thread tRecThread;
    static wmINT Recfunc(wmVOID *pThis);
    
    CALLBACK_RECEIV_BUFF  pCallbackForReceivedBuff;
    wmVOID OnReceivedData(wmCCHAR* pData,wmUSHORT usPort);

    pBitSideNode pBufferQueueNode;
    
    Zip zipper;
    Unzip pUnZip;
};

#endif

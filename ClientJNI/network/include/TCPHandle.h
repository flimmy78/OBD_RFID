#ifndef _TCPHANDLE_H_
#define _TCPHANDLE_H_
#include "BaseDefine.h"
#include "BaseEvent.h"
#include "BaseThread.h"
#include "BaseVector.h"
#include "BaseZip.h"
#include "BaseUnZip.h"
#include "BaseType.h"

class TCPHandle 
{
public:
    TCPHandle();
    ~TCPHandle();

    wmBOOL Init(CALLBACK_RECEIV_BUFF pCbReceivedBuff);
    wmBOOL CreateArpSocket();
    wmBOOL Connect(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort);
    wmBOOL SendData(wmINT iType, wmCHAR const *pBuff, wmSIZET sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmBOOL SendDataNow(wmINT iType, wmCHAR const *pBuff, wmSIZET sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmBOOL Close();
private:
    wmINT iSocket;
    wmBOOL bThreadStatus;

    Thread tSendThread;
    static wmINT Sendfunc(wmVOID *pThis);
    
    Thread tReceiveThread;
    static wmINT Receivefunc(wmVOID *pThis);
    
    wmVOID ReceivedFrameReset();
    wmBOOL OnReceivedData(wmCCHAR* pData,wmULONG nLen);
    wmBOOL OnReceivedFrame(wmINT iTpye,wmCCHAR* pFrameData,wmULONG nLen);
    
    CALLBACK_RECEIV_BUFF  pCallbackForReceivedBuff;

    pBitSideNode pBufferQueueNode;

    wmCHAR* pReceivedBuffer;
    wmULONG ulReceivedBufferLength;
    wmULONG ulReceivedDataLen;

    wmCHAR* pMacAddr;

    Zip zipper;
    Unzip pUnZip;
};

#endif

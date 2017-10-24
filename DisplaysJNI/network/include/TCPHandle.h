#ifndef _TCPHANDLE_H_
#define _TCPHANDLE_H_
#include "BaseDefine.h"
#include "BaseEvent.h"
#include "BaseThread.h"
#include "BaseVector.h"
#include "BaseType.h"
#include "BaseSize.h"

class TCPHandle 
{
public:
    TCPHandle();
    ~TCPHandle();

    wmBOOL Init(CALLBACK_RECEIV_BUFF pCbReceivedBuff);
    wmBOOL Connect(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort);
    wmBOOL DisConnect();
    wmINT IsConnected();
    wmBOOL SendData(wmVOID *pBuff, wmSIZET sBuffLen, wmINT iType);
    wmBOOL RegardToServer();
    wmBOOL Close();
private:
    wmINT iSocket;
    wmBOOL bIsConnect;
    wmCHAR pIpaddr[BASE_STRING_LEN_MIN];
    wmINT    iPort;
    
    Thread tSendThread;
    static wmINT Sendfunc(wmVOID *pThis);
    
    Thread tReceiveThread;
    static wmINT Receivefunc(wmVOID *pThis);

    
    wmVOID ReceivedFrameReset();
    wmBOOL OnReceivedData(wmCCHAR* pData,wmULONG nLen);
    wmBOOL OnReceivedFrame(wmINT iTpye,wmCCHAR* pFrameData,wmULONG nLen,wmULONG lValue1, wmULONG lValue2);
    
    CALLBACK_RECEIV_BUFF  pCallbackForReceivedBuff;

    pBitSideNode pBufferQueueNode;

    wmCHAR* pReceivedBuffer;
    wmULONG ulReceivedBufferLength;
    wmULONG ulReceivedDataLen;

    wmCHAR* pMacAddr;
    wmULONG ulDest;
};

#endif

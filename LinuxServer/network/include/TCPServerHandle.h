#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_
#include "BaseDefine.h"
#include "BaseEvent.h"
#include "BaseThread.h"
#include "BaseVector.h"

class TCPSession;

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    
    wmBOOL CreateTcpServer(wmUSHORT usPort);
    wmBOOL SendData(wmINT iTcpSessionSocketId, wmINT iType, wmCCHAR *pBuff, wmSIZET sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmUSHORT GetServerPort(){return usTcpServerPort;}
    wmBOOL CloseTcpServer();
    
private:
    wmINT iTcpServerSocket;  
    wmUSHORT usTcpServerPort;

    Thread tTcpServerApplyThread;
    static wmINT TcpServerApplyThreadFunction(wmVOID *pThis);
    //Thread tTcpServerActiveThread;
    static wmINT TcpServerActiveThreadFunction(wmVOID *pThis);


    typedef wmVECTOR<TCPSession *> SessionList;
    wmLock        wmMutex;
    SessionList    slSessionList;

    wmBOOL AddTcpSession(wmINT iSoecketClient);
    wmBOOL RemoveTcpSession(TCPSession *pSession);
    wmINT GetTcpSessionCount();
    wmVOID CheckTcpSession();
    wmVOID ClearAllTcpSession(); 
};

class TCPSession
{
public:
    TCPSession();
    wmVOID InitTcpSession(wmINT iTcpSessionSocket, wmBOOL bIsRFID);
    wmVOID CloseTcpSession();
    wmBOOL IsConnected() ;
    
    wmBOOL SendData(wmINT iType, wmCHAR const *pBuff, wmSIZET sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmINT GetSocketId();
private:
    wmINT iSocket;
    wmBOOL bIsRFIDServer;
    wmINT iBeginBandId;
    Thread tSendThread;
    static wmINT Sendfunc(wmVOID *pThis);
    
    Thread tReceiveThread;
    static wmINT Receivefunc(wmVOID *pThis);
    wmVOID ReceivedFrameReset();
    wmBOOL OnReceivedData(wmCCHAR*pData,wmULONG nLen);
    wmBOOL OnReceivedFrame(wmULONG nType, wmCHAR*pData,wmULONG nLen, wmINT iDest,wmULONG lValue1, wmULONG lValue2);
    
    pBitSideNode pBufferQueueNode;

    wmCHAR* pReceivedBuffer;
    wmULONG ulReceivedBufferLength;
    wmULONG ulReceivedDataLen;
};
    
#endif //_TCP_SERVER_H_


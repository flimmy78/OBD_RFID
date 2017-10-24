#include "TCPServerHandle.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseType.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "BaseJson.h"
#include <stdlib.h>
#include <algorithm>
#include "SocketLib.h"
#include "ServerControl.h"

#ifdef _WINDOWS_OS_
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
  

TcpServer::TcpServer()
    : iTcpServerSocket(INVALID_SOCKET)
    , usTcpServerPort(wmFalse)
    , tTcpServerApplyThread(TcpServerApplyThreadFunction)
    //, tTcpServerActiveThread(TcpServerActiveThreadFunction)
{
}

TcpServer::~TcpServer()
{
}

wmBOOL TcpServer::CreateTcpServer(wmUSHORT usPort)
{
    DWMDEBUG("");
#if defined(_WINDOWS_OS_)
    SocketLibInit();
#endif

    if (wmFALSE == CreateSocket(iTcpServerSocket,IPPROTO_TYPE_TCP)) {
        DWMERROR("CreateSocket INVALID_SOCKET");
        return wmFALSE;
    }

    SetReuseaddrOptBySocket(iTcpServerSocket);

    if (wmFALSE == BindBySocket(iTcpServerSocket,pLOCAL_IP_ADDR,usPort)) {
        CloseSocket(iTcpServerSocket);
        DWMERROR("BindBySocket falied");
        return wmFALSE;
    }

    if (wmFALSE == ListenBySocket(iTcpServerSocket)) {
        CloseSocket(iTcpServerSocket);
        DWMERROR("ListenBySocket falied");
        return wmFALSE;
    }

    usTcpServerPort = usPort;
    wmMutex = WMCreateLock(TCP_SERVER_LOCK_NAME);
    tTcpServerApplyThread.start(this);
    //tTcpServerActiveThread.start(this);
    return wmTRUE;
}

wmBOOL TcpServer::CloseTcpServer()
{
    DWMDEBUG("");
    ClearAllTcpSession();
    if(iTcpServerSocket != INVALID_SOCKET)
    {
        CloseSocket(iTcpServerSocket);
        iTcpServerSocket = INVALID_SOCKET;
    }

    tTcpServerApplyThread.join();
    //tTcpServerActiveThread.join(); 
    
    WMDestroyLock(wmMutex);
#if defined(_WINDOWS_OS_)
    SocketLibFree();
#endif
    return wmTRUE;
}

wmINT TcpServer::TcpServerApplyThreadFunction(wmVOID *pThis)
{
    DWMDEBUG("");
    TcpServer *pTcpServer = reinterpret_cast<TcpServer *>(pThis);

    while (pTcpServer->iTcpServerSocket) 
    {   
        struct sockaddr siAddr;
        ::DwmMemset(&siAddr, 0x0, sizeof(siAddr));
#if defined(_WINDOWS_OS_)
        wmINT siAddrlen = sizeof(siAddr);        
#else
        socklen_t siAddrlen = sizeof(siAddr);
#endif
        wmINT iSoecketClient = ::accept(pTcpServer->iTcpServerSocket, reinterpret_cast<sockaddr *>(&siAddr), &siAddrlen);
        if (iSoecketClient < wmTrue) {
            DwmSleep(100);
            continue;
        }
        pTcpServer->AddTcpSession(iSoecketClient);
    } 
    return wmTrue;
}

wmINT TcpServer::TcpServerActiveThreadFunction(wmVOID *pParam)
{
    DWMDEBUG("");
    TcpServer *pTcpServer = reinterpret_cast<TcpServer *>(pParam);

    while (pTcpServer) {
        pTcpServer->CheckTcpSession();
        DwmSleep(10000);
    }
    return wmTrue;
}

wmBOOL TcpServer::AddTcpSession(wmINT iSoecketClient)
{
    DWMDEBUG("");
    TCPSession *pSession = new TCPSession();
    if(wmNULL == pSession)
    {
        DWMERROR("pSession is NULL");
        return wmFALSE;
    }   
    
    pSession->InitTcpSession(iSoecketClient, 6000==usTcpServerPort);
    WMLock(wmMutex);
    slSessionList.push_back(pSession);
    WMUnLock(wmMutex);
    return wmTRUE;
}

wmBOOL TcpServer::RemoveTcpSession(TCPSession *pSession)
{
    DWMDEBUG("");
    if(wmNULL == pSession)
    {
        DWMERROR("pSession is NULL");
        return wmFALSE;
    }

    WMLock(wmMutex);
    
    SessionList::iterator itr = std::find(slSessionList.begin(), slSessionList.end(), pSession);
    if (itr != slSessionList.end()) {
        TCPSession *pSession = *itr;
        if (pSession) 
        {
            pSession->CloseTcpSession();
            wmDELETE pSession;
            pSession = wmNULL;
        }
        slSessionList.erase(itr);
        WMUnLock(wmMutex);
        return wmTRUE;
    }
    WMUnLock(wmMutex);
    return wmFALSE;
}

wmINT TcpServer::GetTcpSessionCount()
{
    wmINT iCount = 0;
    WMLock(wmMutex);
    iCount = slSessionList.size();
    WMUnLock(wmMutex);
    return iCount;
}

wmVOID TcpServer::CheckTcpSession()
{
    WMLock(wmMutex);
    DWMDEBUG("slSessionList.size = [%d]",slSessionList.size());
    for (SessionList::iterator itr = slSessionList.begin(); itr != slSessionList.end();) {
        TCPSession *pSession = *itr;
        if (pSession && !pSession->IsConnected()) {
            itr = slSessionList.erase(itr);
            pSession->CloseTcpSession();
            delete pSession;
            break;
        } else {
            ++itr;
        }
    }
    WMUnLock(wmMutex);
}

wmVOID TcpServer::ClearAllTcpSession()
{
    DWMDEBUG("");
    WMLock(wmMutex);

    for (SessionList::iterator itr = slSessionList.begin(); itr != slSessionList.end();++itr) {
        TCPSession *pSession = *itr;
        if (pSession) {
            pSession->CloseTcpSession();
            wmDELETE pSession;
            pSession = wmNULL;
        }
    }
    slSessionList.clear();
    WMUnLock(wmMutex);
}


wmBOOL TcpServer::SendData(wmINT iTcpSessionSocketId, wmINT iType, wmCCHAR *pBuff, wmSIZET sBuffLen,
        wmINT iDest, wmINT iValue1, wmINT iValue2)
{
    DWMDEBUG("iTcpSessionSocketId = [%d]",iTcpSessionSocketId);
    if(wmNULL == pBuff)
    {
        DWMERROR("pBuff is NULL");
        return wmFALSE;
    }

    if(wmFalse == iTcpSessionSocketId)
    {
         WMLock(wmMutex);
         for (SessionList::iterator itr = slSessionList.begin(); itr != slSessionList.end();) {
            TCPSession *pSession = *itr;
            pSession->SendData(iType,pBuff,sBuffLen,iDest,iValue1,iValue2);
            ++itr;
        }
        WMUnLock(wmMutex);       
        return wmTRUE;
    }
    else
    {
        WMLock(wmMutex);
        for (SessionList::iterator itr = slSessionList.begin(); itr != slSessionList.end();) {
            TCPSession *pSession = *itr;
            if (pSession && iTcpSessionSocketId == pSession->GetSocketId()) {
                wmBOOL bRet = pSession->SendData(iType,pBuff,sBuffLen,iDest,iValue1,iValue2);
                WMUnLock(wmMutex);
                return bRet;
            }
            ++itr;
        }
        WMUnLock(wmMutex);
    }
    return wmFALSE;
}


/*
TCPSession **************************************************

*/
TCPSession::TCPSession()
    : iSocket(INVALID_SOCKET)
    , bIsRFIDServer(wmFALSE)
    , iBeginBandId(wmFalse)
    , tSendThread(Sendfunc)
    , tReceiveThread(Receivefunc)
    , pBufferQueueNode(wmNULL)
    , pReceivedBuffer(wmNULL)
    , ulReceivedBufferLength(wmFalse)
    , ulReceivedDataLen(wmFalse)
{
}

wmVOID TCPSession::InitTcpSession(wmINT socket, wmBOOL bIsRFID)
{
    DWMDEBUG("");
    if(pBufferQueueNode)
    {
        return;
    }   
    bIsRFIDServer = bIsRFID;
    pBufferQueueNode = BitSideNodeNew();
    iSocket = socket;
    iBeginBandId = socket;
    tSendThread.start(this);
    tReceiveThread.start(this);
}

wmVOID TCPSession::CloseTcpSession()
{
    DWMDEBUG("");
    if(iSocket != INVALID_SOCKET)
    {
        CloseSocket(iSocket);
        iSocket = INVALID_SOCKET;
    }
    tReceiveThread.join();
    tSendThread.join();
    if(pBufferQueueNode)
    {
        BitSideNodeDelete(pBufferQueueNode);
    }
    ReceivedFrameReset();
}

wmBOOL TCPSession::IsConnected()
{
    DWMDEBUG("iSocket = [%d]",iSocket);
    if(INVALID_SOCKET == iSocket)
    {
        return wmFALSE;
    }
    else
    {
        return wmTRUE;
    }
}

wmINT TCPSession::GetSocketId()
{
    return iBeginBandId;
}

wmBOOL TCPSession::SendData(wmINT iType, wmCHAR const *pBuff, wmSIZET sBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2)
{
    if(INVALID_SOCKET == iSocket)
    {
        DWMERROR("iSocket is INVALID_SOCKET");
        return wmFALSE;
    }
    
    if(wmNULL == pBuff)
    {
        DWMERROR("pBuff is null");
        return wmFALSE;
    }
    
    if(EN_SYSTEM_INFORMATION_TYPE_RFID_CODE == iType)
    {
        AddBitSideHead(pBufferQueueNode,0,sBuffLen,pBuff);
        return wmTRUE;
    }

    unsigned wmCHAR* pvData = (unsigned wmCHAR* )DwmMalloc(sBuffLen+SOCKET_DATA_CONNECT_FLAG*2);
    if(wmNULL == pvData)
    {
        DWMERROR("pBuff is null");
        return wmFALSE;
    }

    DwmMemset(pvData,0x0,sizeof(pvData));
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_START)  = UseHtonlto(sBuffLen);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DATA_LEN)  = UseHtonlto(iType);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN)  = UseHtonlto(iSocket);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DEST_LEN)  = UseHtonlto(iDest);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN)  = UseHtonlto(iValue1);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN)  = UseHtonlto(iValue2);
    DwmMemcpy(pvData+SOCKET_DATA_CONNECT_FLAG, pBuff, 0x0,sBuffLen);

    AddBitSideHead(pBufferQueueNode,0,sBuffLen+SOCKET_DATA_CONNECT_FLAG,pvData);
    DwmFree(pvData);
    DWMDEBUG("[%d]",sBuffLen+SOCKET_DATA_CONNECT_FLAG);
    return wmTRUE;
}

wmINT TCPSession::Sendfunc(wmVOID *pThis)
{
    DWMDEBUG("");
    TCPSession *pTCPSession = reinterpret_cast<TCPSession *>(pThis);
    if(wmNULL == pTCPSession)
    {
        DWMERROR("pTCPSession is null");
        return wmFalse;
    }

    while (pTCPSession->iSocket != INVALID_SOCKET) 
    {
        if(BitSideNodeGetCount(pTCPSession->pBufferQueueNode) == 0)
        {
            DwmSleep(SOCKET_WAIT_TIME_FOR_SENDTO);
            continue;
        }

        pBitSide pBitSideBuffer = BitSideNodeRemove(pTCPSession->pBufferQueueNode);
        if(pBitSideBuffer)
        {
            wmSIZET sSize = pBitSideBuffer->uLen;
            wmSIZET sSend = 0;

            while (sSend < sSize) {
                wmINT iSendLen = SendBySocket(pTCPSession->iSocket, reinterpret_cast<wmCHAR *>(pBitSideBuffer->pData) + sSend, sSize - sSend);
                if (iSendLen < wmTrue) {
                    DWMERROR("SendBySocket error");
                    pTCPSession->iSocket = INVALID_SOCKET;
                    break;
                }
                sSend += iSendLen;
            }
            DwmFree(pBitSideBuffer->pData);
            DwmFree(pBitSideBuffer);
        }
    }

    return wmTrue;
}

wmINT TCPSession::Receivefunc(wmVOID *pThis)
{
    DWMDEBUG("");
    TCPSession *pTCPSession = reinterpret_cast<TCPSession *>(pThis);
    if(wmNULL == pTCPSession)
    {
        DWMERROR("pTCPSession is null");
        return wmFalse;
    }


    wmFDSET fdSet;
    wmINT iMaxSock;
    struct wmTIMEVAL tvTimeVal;
    wmINT igClientPendingCount[SOCKET_APPLY_COUNT];

    iMaxSock = pTCPSession->iSocket;
    
    wmCHAR* pRecvBuff = (wmCHAR*)DwmMalloc(BASE_STRING_LEN_MAX+1);
    if(wmNULL == pRecvBuff)
    {
        DWMERROR("pRecvBuff is null");
        return wmFalse;
    }
    while (pTCPSession->iSocket != INVALID_SOCKET) 
    {
        wmFDZERO(&fdSet);
        wmFD_SET(pTCPSession->iSocket, &fdSet);
        tvTimeVal.tv_sec = SOCKET_WAIT_TIME_FOR_SELECT;
        tvTimeVal.tv_usec = 0;

        FOR(SOCKET_APPLY_COUNT) 
        {
            if(igClientPendingCount[i] != wmFalse)
            {
                wmFD_SET(igClientPendingCount[i],&fdSet);
            }
        }
        
        wmINT iRetSelect = ::select(iMaxSock + 1, &fdSet, wmNULL, wmNULL, &tvTimeVal);
        if (iRetSelect < wmFalse) 
        {  
            DWMERROR("select error");
            break;
        } 
        
        if (wmFalse == iRetSelect) 
        {
            continue;
        }
        
        if(wmFD_ISSET(pTCPSession->iSocket, &fdSet))
        {
            DwmMemset(pRecvBuff, 0x0, sizeof(pRecvBuff));
            wmINT iRecvBitLen = RecvBySocket(pTCPSession->iSocket, pRecvBuff, BASE_STRING_LEN_MAX);
            if (iRecvBitLen < wmTrue) {
                if(wmFalse == iRecvBitLen)
                {
                    DWMERROR("RecvBySocket error");
                    pTCPSession->iSocket = INVALID_SOCKET;
                    break;
                }
                DwmSleep(1);
                continue;
            }
            if(pTCPSession->bIsRFIDServer)
            {
                CServerControl* pCServerControl = CServerControl::GetInstance();
                if(wmNULL == pCServerControl)
                {
                   DWMERROR("CServerControl::GetInstance error");
                }
                pCServerControl->ReceiveTcpHandleBuff(EN_SYSTEM_INFORMATION_TYPE_RFID_CODE, 
                    pTCPSession->iSocket, pRecvBuff, iRecvBitLen,wmFalse,wmFalse,wmFalse);
            }
            else
            {
                pTCPSession->OnReceivedData(pRecvBuff, iRecvBitLen);
            }
        }
    }

    DwmFree(pRecvBuff);
    return wmTrue;
}

wmBOOL TCPSession::OnReceivedFrame(wmULONG nType, wmCHAR* pFrameData,wmULONG ulLen, wmINT iDest, wmULONG lValue1, wmULONG lValue2)
{
    if(wmNULL == pFrameData)
    {
        DWMERROR("pFrameData is null");
        return wmFALSE;
    }
    CServerControl* pCServerControl = CServerControl::GetInstance();
    if(wmNULL == pCServerControl)
    {
        DWMERROR("CServerControl::GetInstance error");
    }
    pCServerControl->ReceiveTcpHandleBuff(nType,iSocket, pFrameData, ulLen,iDest,lValue1,lValue2);
    return wmTRUE;
}

wmVOID TCPSession::ReceivedFrameReset()
{
    if(wmNULL != pReceivedBuffer)
    {
        DwmFree(pReceivedBuffer);
    }
    ulReceivedBufferLength=wmFalse;
    ulReceivedDataLen=wmFalse;
}

wmBOOL TCPSession::OnReceivedData(wmCCHAR* pData,wmULONG nLen)
{
    if (wmNULL == pData)
    {
        DWMERROR("pData is null");
        return wmFALSE;
    }

    if (wmNULL == pReceivedBuffer)
    {
        ulReceivedBufferLength = (BASE_STRING_LEN_MAX + nLen);

        pReceivedBuffer=(wmCHAR*)DwmMalloc(ulReceivedBufferLength);
        if (wmNULL == pReceivedBuffer)
        {
            ulReceivedBufferLength = wmFalse;
            return wmFALSE;
        }
        DwmMemset(pReceivedBuffer, 0x0, sizeof(pReceivedBuffer));
    }

    if ((ulReceivedDataLen+nLen) > ulReceivedBufferLength )
    {
        ulReceivedBufferLength = (ulReceivedDataLen + nLen + BASE_STRING_LEN_MAX);
        wmCHAR *pTempBuffer=(wmCHAR*)DwmMalloc(ulReceivedBufferLength);
        if (wmNULL == pTempBuffer)
        {
            ReceivedFrameReset();
            return wmFALSE;
        }
        
        DwmMemcpy(pTempBuffer,pReceivedBuffer,0x0,ulReceivedDataLen);
        DwmFree(pReceivedBuffer);
        pReceivedBuffer = pTempBuffer;
    }
    
    DwmMemcpy(pReceivedBuffer+ulReceivedDataLen,pData,0x0,nLen);
    ulReceivedDataLen+=nLen;

    wmCHAR* pFrameData = pReceivedBuffer;
    wmULONG nReceivedDataLen = ulReceivedDataLen;
    while(ulReceivedDataLen > SOCKET_DATA_CONNECT_FLAG)
    {
        wmULONG nFrameDataLen = UseNtohlto(*(unsigned long*)(pFrameData));
        if ((nFrameDataLen+SOCKET_DATA_CONNECT_FLAG)  <= nReceivedDataLen)
        {
            wmULONG lType = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_DATA_LEN));
            wmULONG lDest = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
            wmULONG lValue1 = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN));
            wmULONG lValue2 = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN));
            pFrameData+=SOCKET_DATA_CONNECT_FLAG;
            nReceivedDataLen-=SOCKET_DATA_CONNECT_FLAG;
            OnReceivedFrame(lType, pFrameData,nFrameDataLen,lDest,lValue1,lValue2);
            pFrameData+=nFrameDataLen;
            nReceivedDataLen-=nFrameDataLen;
        }
        else
        {
            break;
        }
    }

    if (nReceivedDataLen>0) {
        memmove(pReceivedBuffer,pFrameData,nReceivedDataLen);
        ulReceivedDataLen = nReceivedDataLen;
    } else {
        ulReceivedDataLen = 0;
    }

    return wmTRUE;
}





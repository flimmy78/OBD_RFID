#include "TCPHandle.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "SocketLib.h"
#include <stdlib.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

TCPHandle::TCPHandle()
    : iSocket(INVALID_SOCKET)
    , bIsConnect(wmFALSE)
    , iPort(wmFalse)
    , tSendThread(Sendfunc)
    , tReceiveThread(Receivefunc)
    , pCallbackForReceivedBuff(wmNULL)
    , pBufferQueueNode(wmNULL)
    , pReceivedBuffer(wmNULL)
    , ulReceivedBufferLength(wmFalse)
    , ulReceivedDataLen(wmFalse)
    , pMacAddr(wmNULL)
    , ulDest(wmFalse)
{
    DwmMemset(pIpaddr, 0x0, sizeof(pIpaddr));
}

TCPHandle::~TCPHandle()
{
}

wmBOOL TCPHandle::Init(CALLBACK_RECEIV_BUFF pCbReceivedBuff)
{
    DWMDEBUG("");
    if(pBufferQueueNode)
    {
        DWMERROR("double Init");
        return wmFALSE;
    }   
    
    pBufferQueueNode = BitSideNodeNew();
 
    if(pCbReceivedBuff)
    {
        pCallbackForReceivedBuff  = pCbReceivedBuff;
    }

    return wmTRUE;
}


wmBOOL TCPHandle::Close()
{
    DWMDEBUG("");
    if(iSocket != INVALID_SOCKET)
    {
        CloseSocket(iSocket);
        tSendThread.join();
        tReceiveThread.join();
    }
    
    if(pBufferQueueNode)
    {
        BitSideNodeDelete(pBufferQueueNode);
    }
    ReceivedFrameReset();
    return wmTRUE;
}

wmBOOL TCPHandle::Connect(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort)
{
    DWMDEBUG("");
    if (wmFALSE == CreateSocket(iSocket,IPPROTO_TYPE_TCP)) {
        DWMERROR("CreateSocket INVALID_SOCKET");
        return wmFALSE;
    }

    SetReuseaddrOptBySocket(iSocket);
    
    if (wmFALSE == ConnectBySocket(iSocket,pConnectAddr,usConnectPort)) {
        CloseSocket(iSocket);
        DWMERROR("ConnectBySocket falied");
        return wmFALSE;
    }
    DwmStrcpy(pIpaddr, pConnectAddr);
    iPort = usConnectPort;

    tReceiveThread.start(this);
    tSendThread.start(this);
    return wmTRUE;
}

wmBOOL TCPHandle::DisConnect()
{
    if(iSocket != INVALID_SOCKET)
    {
        CloseSocket(iSocket);
        tSendThread.join();
        tReceiveThread.join();
    }
    
    if(pBufferQueueNode)
    {
        BitSideNodeDelete(pBufferQueueNode);
    }
    ReceivedFrameReset();
    pBufferQueueNode = BitSideNodeNew();
    return wmTRUE;
}

wmINT TCPHandle::IsConnected()
{
    if(INVALID_SOCKET == iSocket)
    {
        DWMERROR("iSocket is INVALID_SOCKET");
        return wmFalse;
    }
    

    unsigned wmCHAR* pvData = (unsigned wmCHAR* )DwmMalloc(SOCKET_DATA_CONNECT_FLAG*2);
    if(wmNULL == pvData)
    {
        DWMERROR("pvData is null");
        return wmFalse;
    }
    
    DwmMemset(pvData,0x0,sizeof(pvData));
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_START)  = UseHtonlto(SOCKET_DATA_CONNECT_FLAG);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DATA_LEN)  = UseHtonlto(EN_SYSTEM_INFORMATION_TYPE_NONE);

    wmSIZET sSize = SOCKET_DATA_CONNECT_FLAG*2;
    wmSIZET sSend = 0;
    wmQWORD wSendStart = MPGetMonotonic();
    while (sSend < sSize) 
    {
        wmINT iSendLen = SendBySocket(iSocket, reinterpret_cast<wmCHAR *>(pvData) + sSend, sSize - sSend);
        if (iSendLen < wmTrue) {
            return wmFalse;
        }
        sSend += iSendLen;
    }
    wmQWORD wSendend = MPGetMonotonic();
    DwmFree(pvData);
    wmQWORD wSendsp = 1000000000/(wSendend - wSendStart)*sSize;
    return (wmINT)wSendsp;
}

wmBOOL TCPHandle::RegardToServer()
{
    if(INVALID_SOCKET == iSocket)
    {
        DWMERROR("iSocket is INVALID_SOCKET");
        return wmFALSE;
    }

    wmCHAR* pvData = (wmCHAR* )DwmMalloc(SOCKET_DATA_CONNECT_FLAG+NET_MAXADDR_COUNT);
    if(wmNULL == pvData)
    {
        DWMERROR("pvData is null");
        return wmFALSE;
    }

    DwmMemset(pvData,0x0,sizeof(pvData));
    GetMacAdd(pvData+SOCKET_DATA_CONNECT_FLAG,NET_MAXADDR_COUNT);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_START)  = UseHtonlto(NET_MAXADDR_COUNT);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DATA_LEN)  = UseHtonlto(EN_SYSTEM_INFORMATION_TYPE_DISPLAYS_DEV);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN)  = UseHtonlto(iSocket);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DEST_LEN)  = UseHtonlto(wmFalse);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN)  = UseHtonlto(wmFalse);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN)  = UseHtonlto(wmFalse);

    wmSIZET sSize = SOCKET_DATA_CONNECT_FLAG+NET_MAXADDR_COUNT;
    wmSIZET sSend = 0;
    DWMDEBUG("GetMacAdd = [%s][%d]",pvData+SOCKET_DATA_CONNECT_FLAG,NET_MAXADDR_COUNT);

    while (sSend < sSize) 
    {
        wmINT iSendLen = SendBySocket(iSocket, pvData + sSend, sSize - sSend);
        if (iSendLen < wmTrue) {
            break;
        }
        sSend += iSendLen;
    }
    DwmFree(pvData);
    return wmTRUE;
}

wmBOOL TCPHandle::SendData(wmVOID *pBuff, wmSIZET sBuffLen, wmINT iType)
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
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DEST_LEN)  = UseHtonlto(ulDest);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN)  = UseHtonlto(wmFalse);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN)  = UseHtonlto(wmFalse);
    DwmMemcpy(pvData+SOCKET_DATA_CONNECT_FLAG, pBuff, 0x0,sBuffLen);

    AddBitSideHead(pBufferQueueNode,0,sBuffLen+SOCKET_DATA_CONNECT_FLAG,pvData);
    return wmTRUE;
}

wmINT TCPHandle::Sendfunc(wmVOID *pThis)
{
    DWMDEBUG("");
    TCPHandle *pTCPHandle = reinterpret_cast<TCPHandle *>(pThis);
    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is null");
        return wmFalse;
    }

    while (pTCPHandle->iSocket != INVALID_SOCKET) 
    {
        if(BitSideNodeGetCount(pTCPHandle->pBufferQueueNode) == 0)
        {
            DwmSleep(1);
            continue;
        }

        pBitSide pBitSideBuffer = BitSideNodeRemove(pTCPHandle->pBufferQueueNode);
        if(pBitSideBuffer)
        {
            wmSIZET sSize = pBitSideBuffer->uLen;
            wmSIZET sSend = 0;

            while (sSend < sSize) {
                wmINT iSendLen = SendBySocket(pTCPHandle->iSocket, reinterpret_cast<wmCHAR *>(pBitSideBuffer->pData) + sSend, sSize - sSend);
                if (iSendLen < wmTrue) {
                    DWMERROR("SendBySocket error");
                    pTCPHandle->iSocket = INVALID_SOCKET;
                    break;
                }
                sSend += iSendLen;
            }
            DwmFree(pBitSideBuffer->pData);
            DwmFree(pBitSideBuffer);
        }
    }
    DWMDEBUG("end");

    return wmTrue;
}

wmINT TCPHandle::Receivefunc(wmVOID *pThis)
{
    DWMDEBUG("");
    TCPHandle *pTCPHandle = reinterpret_cast<TCPHandle *>(pThis);
    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is null");
        return wmFalse;
    }

    wmFDSET fdSet;
    wmINT iMaxSock;
    struct wmTIMEVAL tvTimeVal;
    wmINT igClientPendingCount[SOCKET_APPLY_COUNT];

    iMaxSock = pTCPHandle->iSocket;  

    wmCHAR* pRecvBuff = (wmCHAR*)DwmMalloc(BASE_STRING_LEN_MAX+1);
    if(wmNULL == pRecvBuff)
    {
        DWMERROR("pRecvBuff is null");
        return wmFalse;
    }
    
    while (pTCPHandle->iSocket != INVALID_SOCKET) 
    {
        wmFDZERO(&fdSet);
        wmFD_SET(pTCPHandle->iSocket, &fdSet);
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
        else if (wmFalse == iRetSelect) 
        {
            continue;
        }
        else
        {
            DWMDEBUG("request from client");
        }
        
        if(wmFD_ISSET(pTCPHandle->iSocket, &fdSet))
        {
            DwmMemset(pRecvBuff, 0x0, sizeof(pRecvBuff));
            wmINT iRecvBitLen = RecvBySocket(pTCPHandle->iSocket, pRecvBuff, BASE_STRING_LEN_MAX);
            if(iRecvBitLen > wmFalse)
            {
                 pTCPHandle->OnReceivedData(pRecvBuff, iRecvBitLen);
                 DWMDEBUG("len =[%d]", iRecvBitLen);
            }
            else
            {
                DWMERROR("RecvBySocket error");
                pTCPHandle->iSocket = INVALID_SOCKET;
                break;
            }
         }
    }
    DwmFree(pRecvBuff);
    DWMDEBUG("end");
    return wmTrue;
}

wmBOOL TCPHandle::OnReceivedFrame(wmINT iTpye, wmCCHAR* pFrameData,wmULONG ulLen,wmULONG lValue1, wmULONG lValue2)
{
    DWMDEBUG("type = [%d] len =[%d][%d][%d]",iTpye,ulLen,lValue1,lValue2);
    if(wmNULL == pFrameData)
    {
        DWMERROR("pFrameData is null");
        return wmFALSE;
    }
    if(pCallbackForReceivedBuff)
    {
        pCallbackForReceivedBuff(iTpye,(char*)pFrameData,ulLen,(wmINT)lValue1,(wmINT)lValue2);
    }
    return wmTRUE;
}

wmVOID TCPHandle::ReceivedFrameReset()
{
    DWMDEBUG("");
    if (pReceivedBuffer)
    {
        DwmFree(pReceivedBuffer);
    }
    ulReceivedBufferLength=wmFalse;
    ulReceivedDataLen=wmFalse;
}

wmBOOL TCPHandle::OnReceivedData(wmCCHAR*pData, wmULONG nLen)
{
    if (wmNULL == pData)
    {
        DWMERROR("pData is null");
        return wmFALSE;
    }
 
    //alloc buffer
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

    //alloc more buffer
    if ((ulReceivedDataLen+nLen) > ulReceivedBufferLength )
    {
        ulReceivedBufferLength = (ulReceivedDataLen + nLen + BASE_STRING_LEN_MAX);
        wmCHAR *pTempBuffer=(wmCHAR*)DwmMalloc(ulReceivedBufferLength);
        if (wmNULL == pTempBuffer)
        {
            DwmFree(pReceivedBuffer);
            return wmFALSE;
        }
        
        DwmMemcpy(pTempBuffer,pReceivedBuffer,0x0,ulReceivedDataLen);
        DwmFree(pReceivedBuffer);
        pReceivedBuffer = pTempBuffer;
    }
    
    //save data
    DwmMemcpy(pReceivedBuffer+ulReceivedDataLen,pData,0x0,nLen);
    ulReceivedDataLen+=nLen;

    //read header
    wmCHAR* pFrameData = pReceivedBuffer;
    wmULONG nReceivedDataLen = ulReceivedDataLen;
    while(ulReceivedDataLen > SOCKET_DATA_CONNECT_FLAG)
    {
        wmULONG nFrameDataLen = UseNtohlto(*(unsigned long*)(pFrameData));
        if ((nFrameDataLen+SOCKET_DATA_CONNECT_FLAG)  <= nReceivedDataLen)
        {
            wmULONG nType = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_DATA_LEN));
            ulDest = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
            wmULONG lValue1 = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN));
            wmULONG lValue2 = UseNtohlto(*(unsigned long*)(pFrameData+SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN));
            pFrameData+=SOCKET_DATA_CONNECT_FLAG;
            nReceivedDataLen-=SOCKET_DATA_CONNECT_FLAG;
            
            OnReceivedFrame(nType,pFrameData,nFrameDataLen,lValue1,lValue2);
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



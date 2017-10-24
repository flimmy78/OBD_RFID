#include "UDPHandle.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseType.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "SocketLib.h"
#include "MainControl.h"

#ifdef _WINDOWS_OS_
#include <windows.h>
#else
#include <stdlib.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

UDPHandle::UDPHandle()
    : iSocket(INVALID_SOCKET)
    , bThreadStatus(wmFALSE)
    , tRecThread(Recfunc)
    , pCallbackForReceivedBuff(wmNULL)
{
}

UDPHandle::~UDPHandle()
{
}


wmBOOL UDPHandle::InitHandle(CALLBACK_RECEIV_BUFF pCbReceivedBuff, wmUSHORT usSendtoPort)
{
    DWMDEBUG("");
#if defined(_WINDOWS_OS_)
    SocketLibInit();
#endif
 
    if(pCbReceivedBuff)
    {
        pCallbackForReceivedBuff  = pCbReceivedBuff;
    }

    if (wmFALSE == CreateSocket(iSocket,IPPROTO_TYPE_UDP)) {
        DWMERROR("CreateSocket INVALID_SOCKET");
        return wmFALSE;
    }

    if(wmFALSE == bIsActivedPort(usSendtoPort))
    {
        DWMERROR("bind error");
        return wmFALSE;
    }

    tRecThread.start(this);

    return wmTRUE;
}

wmBOOL UDPHandle::bIsActivedPort(wmUSHORT usSendtoPort)
{
    DWMDEBUG("");
    if(usSendtoPort > 0xffff || usSendtoPort < 0x1)
    {
        DWMERROR("usSendtoPort is INVALID");
        return wmFALSE;
    }

    return BindBySocket(iSocket,pLOCAL_IP_ADDR,usSendtoPort);
}

wmBOOL UDPHandle::SendData(wmCHAR const *pBuff, wmSIZET sBuffLen, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort)
{
    DWMDEBUG("");
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
    
    wmCHAR* pvData = ( wmCHAR* )DwmMalloc(sBuffLen+SOCKET_DATA_CONNECT_FLAG*2 );
    if(wmNULL == pvData)
    {
        DWMERROR("pBuff is null");
        return wmFALSE;
    }
    DwmMemset(pvData,0x0,sizeof(pvData));
    
    pvData[0]  = (char)(EN_SYSTEM_INFORMATION_TYPE_REQUEST_APPLY);
    DwmMemcpy(pvData+SOCKET_DATA_CONNECT_FLAG, pBuff, 0x0,sBuffLen);
    SendToBySocket(iSocket, pvData, sBuffLen+SOCKET_DATA_CONNECT_FLAG, pSendtoAddr, usSendtoPort);
    DwmFree(pvData);
    return wmTRUE;
}

wmBOOL UDPHandle::CloseHandle()
{
    DWMDEBUG("");
    if(iSocket != INVALID_SOCKET)
    {
        CloseSocket(iSocket);
        iSocket = INVALID_SOCKET;
    }

    tRecThread.join();
    return wmTRUE;
}

wmINT UDPHandle::Recfunc(wmVOID *pThis)
{
    DWMDEBUG("");
    UDPHandle *pUDPHandle = reinterpret_cast<UDPHandle *>(pThis);
    if(wmNULL == pUDPHandle)
    {
        DWMERROR("pUDPHandle is null");
        return wmFalse;
    }

    wmFDSET fdSet;
    wmINT iMaxSock;
    struct wmTIMEVAL tvTimeVal;
    wmINT igClientPendingCount[SOCKET_APPLY_COUNT];
    iMaxSock = pUDPHandle->iSocket;

    wmCHAR* pRecvBuff = (wmCHAR*)DwmMalloc(BASE_STRING_LEN_MAX+1);
    if(wmNULL == pRecvBuff)
    {
        DWMERROR("pRecvBuff is null");
        return wmFalse;
    }
    
    while (pUDPHandle->iSocket != INVALID_SOCKET) 
    {
        wmFDZERO(&fdSet);
        wmFD_SET(pUDPHandle->iSocket, &fdSet);
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
            DWMDEBUG("Time out : because no any request from client");
            continue;
        }
        else
        {
            DWMDEBUG("request from client");
        }
        if(wmFD_ISSET(pUDPHandle->iSocket, &fdSet))
        {
            DwmMemset(pRecvBuff, 0x0, sizeof(pRecvBuff));
            wmUSHORT usPort = wmFalse;
            wmCHAR pIpAddr[SOCKET_STRING_DATA_TAGET] = {0};
            wmINT iRecvBitLen = RecvfromBySocket(pUDPHandle->iSocket, pRecvBuff, BASE_STRING_LEN_MAX,usPort,pIpAddr);
            if(iRecvBitLen > wmFalse)
            {
                if(EN_SYSTEM_INFORMATION_TYPE_REQUEST_APPLY == pRecvBuff[0])
                {
                    pUDPHandle->OnReceivedData(pIpAddr,atoi(pRecvBuff+SOCKET_DATA_CONNECT_FLAG));
                }
            }
            else
            {
                DWMERROR("RecvBySocket error");
                pUDPHandle->iSocket = INVALID_SOCKET;
                break;
            }
        }
    }
    DwmFree(pRecvBuff);
    DWMDEBUG("end");
    return wmTrue;
}

wmVOID UDPHandle::OnReceivedData(wmCCHAR* pData,wmUSHORT usPort)
{
    DWMDEBUG("OnReceivedData");
    if(wmNULL == pData)
    {
        DWMERROR("OnReceivedData pData is null");
        return;
    }
    DWMDEBUG("ip = [%s:%d]",pData,usPort);
    if(pCallbackForReceivedBuff)
    {
        pCallbackForReceivedBuff(EN_RECEIV_BUFF_TYPE_IPADDR,const_cast<char *>(pData),usPort);
    }
}



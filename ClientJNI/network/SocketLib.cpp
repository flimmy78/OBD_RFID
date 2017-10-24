#include "SocketLib.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseType.h"
#include "BaseDebug.h"
#include "BaseFile.h"
#include <errno.h>

#if defined(_WINDOWS_OS_)
#include <winsock2.h>
typedef int socklen_t;

static inline bool platform_socket_init()
{
	WSADATA wsaData = {};
	return (::WSAStartup(WINSOCK_VERSION, &wsaData) == 0);
}
static inline void platform_socket_free() { ::WSACleanup(); }
static inline int  platform_socket_close(SOCKET s) { return ::closesocket(s); }

static inline int  platform_inet_aton(char const *text, struct in_addr *addr)
{
	uint32_t ret = ::inet_addr(text);
	addr->s_addr = ret;
	return (ret != INADDR_NONE) && (ret != INADDR_ANY);
}

static inline int  platform_lasterror() { return ::WSAGetLastError(); }

wmVOID SocketLibInit()
{
	wmBOOL bRet = ::platform_socket_init();
	DWMDEBUG("[%s]",bRet?"true":"false");
}

wmVOID SocketLibFree()
{
	::platform_socket_free();
}

#else


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>  
#include <netinet/if_ether.h> 
#include <netpacket/packet.h>  

#endif

typedef struct sockaddr SA;

static inline SA *sockaddr_cast(struct sockaddr_in *addr)
{
    return reinterpret_cast<SA *>(addr);
}
static inline SA const *sockaddr_cast(struct sockaddr_in const *addr)
{
    return reinterpret_cast<SA const *>(addr);
}


wmBOOL CreateSocket(wmINT& iSoecket, IPPROTO_TYPE itType)
{
    DWMDEBUG("");
    switch(itType)
    {
        case IPPROTO_TYPE_TCP: 
        {
            iSoecket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
            DWMDEBUG("iSoecket = [%d]",iSoecket);
            break;
        }
        case IPPROTO_TYPE_UDP: 
        {
            iSoecket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            break;
        }
        case IPPROTO_TYPE_RAW: 
        {
            iSoecket = ::socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
            break;
        }
        default:
        {
            break;;
        }
    }

    if (INVALID_SOCKET == iSoecket)
    {
        return wmFALSE;
    }
    else
    {
        return wmTRUE;
    }
}

wmBOOL CloseSocket(wmINT& iSoecket)
{
    DWMDEBUG("");
    if(INVALID_SOCKET != iSoecket)
    {
#if defined(_WINDOWS_OS_)
        wmINT iReturn = ::platform_socket_close(iSoecket);
#else
        wmINT iReturn = ::close(iSoecket);
#endif
        if (SOCKET_ERROR == iReturn) {
            DWMERROR("platform_socket_close error");
            return wmFALSE;
        }
        iSoecket = INVALID_SOCKET;
    }
    return wmTRUE;
}

wmINT SendBySocket(wmINT iSoecket, wmCCHAR* pBuff, wmSIZET siBuffSize)
{
    //DWMDEBUG("");
    wmINT iBit = 0;
    if(wmNULL == pBuff)
    {
        DWMERROR("pBuff is NULL");
        return iBit;
    }
    iBit = ::send(iSoecket, pBuff, siBuffSize, 0);
    if (SOCKET_ERROR == iBit) {
        DWMERROR("send error");
    }
    return iBit;
}

wmBOOL RecvTimeOutOptBySocket(wmINT iSoecket, wmINT iTime)
{
    if(iTime < SOCKET_WAIT_TIME)
    {
        iTime = SOCKET_WAIT_TIME;
    }
    struct timeval tTimeOut= {iTime,wmFalse}; 
    ::setsockopt(iSoecket, SOL_SOCKET, SO_RCVTIMEO, (wmCCHAR *)&tTimeOut, sizeof(struct timeval));
    return wmTRUE;
}

wmBOOL SendTimeOutOptBySocket(wmINT iSoecket, wmINT iTime)
{
    if(iTime < SOCKET_WAIT_TIME)
    {
        iTime = SOCKET_WAIT_TIME;
    }
    struct timeval tTimeOut= {iTime,wmFalse}; 
    ::setsockopt(iSoecket, SOL_SOCKET, SO_SNDTIMEO, (wmCCHAR *)&tTimeOut, sizeof(struct timeval));
    return wmTRUE;
}

wmBOOL SetReuseaddrOptBySocket(wmINT iSoecket)
{
    wmINT iOpt = SO_REUSEADDR;  
    ::setsockopt(iSoecket, SOL_SOCKET, iOpt, &iOpt, sizeof(iOpt));
    return wmTRUE;
}

wmINT RecvBySocket(wmINT iSoecket, wmCHAR*pBuff, wmSIZET siBuffSize)
{
    //DWMDEBUG("");
    wmINT iBit = 0;
    if(wmNULL == pBuff)
    {
        DWMERROR("pBuff is NULL");
        return iBit;
    }
    iBit = ::recv(iSoecket, pBuff, siBuffSize, 0);
    return iBit;
}

wmINT SendToBySocket(wmINT iSoecket, wmCCHAR* pBuff, wmSIZET siBuffSize, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort)
{
    //DWMDEBUG("");
    wmINT iBit = 0;
    if((wmNULL == pBuff) ||
        (wmNULL == pSendtoAddr) ||
        (usSendtoPort < SOCKET_PORT_MIN) ||
        (usSendtoPort > SOCKET_PORT_MAX))
    {
        DWMERROR("send date is error");
        return iBit;
    }
    struct sockaddr_in siAddr;
    DwmMemset(&siAddr, 0x0, sizeof(siAddr));
    siAddr.sin_family = AF_INET;
    siAddr.sin_port = htons(usSendtoPort);
    siAddr.sin_addr.s_addr=inet_addr(pSendtoAddr);
    
    socklen_t addrlen = sizeof siAddr;
    
    iBit =::sendto(iSoecket, pBuff, siBuffSize, 0, sockaddr_cast(&siAddr), addrlen);
    return iBit;
}

wmINT RecvfromBySocket(wmINT iSoecket, wmCHAR*pBuff, wmSIZET siBuffSize, wmUSHORT& usSendtoPort, wmCHAR* pSendtoAddr = wmNULL)
{
    //DWMDEBUG("");
    wmINT iBit = 0;
    if(wmNULL == pBuff)
    {
        DWMERROR("pBuff is NULL");
        return iBit;
    }
    
    struct sockaddr_in siAddr;
    DwmMemset(&siAddr, 0x0, sizeof(siAddr));
    socklen_t siAddrlen = sizeof(siAddr);
    
    iBit =::recvfrom(iSoecket, pBuff, siBuffSize, 0, sockaddr_cast(&siAddr), &siAddrlen);
    if (SOCKET_ERROR == iBit) {
        DWMERROR("recvfrom error");
    }

    if(pSendtoAddr)
    {
        DwmStrcpy(pSendtoAddr, inet_ntoa(siAddr.sin_addr));
        usSendtoPort = ntohs(siAddr.sin_port);
    }
    return iBit;
}

wmBOOL ConnectBySocket(wmINT iSoecket, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort)
{
    DWMDEBUG("");
    if( (wmNULL == pSendtoAddr) ||
        (usSendtoPort < SOCKET_PORT_MIN) ||
        (usSendtoPort > SOCKET_PORT_MAX))
    {
        DWMERROR("send date is error");
        return wmFALSE;
    }
    struct sockaddr_in siAddr;
    //::DwmMemset(&siAddr, 0x0, sizeof(siAddr));
    siAddr.sin_family = AF_INET;
    siAddr.sin_port = htons(usSendtoPort);
    siAddr.sin_addr.s_addr=inet_addr(pSendtoAddr);
    
    socklen_t addrlen = sizeof siAddr;
    
    wmINT iReturn =::connect(iSoecket, sockaddr_cast(&siAddr), addrlen);
    if (SOCKET_ERROR == iReturn) {
        DWMERROR("connect error");
        return wmFALSE;
    }
    return wmTRUE;
}

wmBOOL BindBySocket(wmINT iSoecket, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort)
{
    DWMDEBUG("");
    if( (wmNULL == pSendtoAddr) ||
        (usSendtoPort < SOCKET_PORT_MIN) ||
        (usSendtoPort > SOCKET_PORT_MAX))
    {
        DWMERROR("send date is error");
        return wmFALSE;
    }
    struct sockaddr_in siAddr;
    DwmMemset(&siAddr, 0x0, sizeof(siAddr));
    siAddr.sin_family = AF_INET;
    siAddr.sin_port = UseHtonsto(usSendtoPort);
    if(strcmp(pSendtoAddr,pLOCAL_IP_ADDR) == 0)
    {
        siAddr.sin_addr.s_addr = UseHtonlto(INADDR_ANY);
    }
    else
    {
        siAddr.sin_addr.s_addr=inet_addr(pSendtoAddr);
    }
    
    
    socklen_t addrlen = sizeof siAddr;
    
    wmINT iReturn =::bind(iSoecket, sockaddr_cast(&siAddr), addrlen);
    if (SOCKET_ERROR == iReturn) {
        DWMERROR("bind error");
        return wmFALSE;
    }
    return wmTRUE;
}

wmBOOL ListenBySocket(wmINT iSoecket)
{
    DWMDEBUG("");

    wmINT iReturn = ::listen(iSoecket, SOMAXCONN);
    if (SOCKET_ERROR == iReturn) {
        DWMERROR("listen error");
        return wmFALSE;
    }
    return wmTRUE;
}

wmINT AcceptBySocket(wmINT iSoecket, wmUSHORT& usSendtoPort, wmCHAR* pSendtoAddr = wmNULL)
{
    DWMDEBUG("");

    if( (wmNULL == pSendtoAddr) ||
        (usSendtoPort < SOCKET_PORT_MIN) ||
        (usSendtoPort > SOCKET_PORT_MAX))
    {
        DWMERROR("send date is error");
        return INVALID_SOCKET;
    }
    struct sockaddr_in siAddr;
    DwmMemset(&siAddr, 0x0, sizeof(siAddr));
    socklen_t siAddrlen;
    
    SOCKET iSoecketClient = ::accept(iSoecket, sockaddr_cast(&siAddr), &siAddrlen);
    if (INVALID_SOCKET == iSoecketClient) {
        DWMERROR("accept error");
    }

    if(pSendtoAddr)
    {
        DwmStrcpy(pSendtoAddr, inet_ntoa(siAddr.sin_addr));
        usSendtoPort = ntohs(siAddr.sin_port);
    }
    return iSoecketClient;
}

#define pAndroidMacPatch "/sys/class/net/wlan0/address"

wmVOID GetMacAdd(wmCHAR* pData, wmINT iDateLen)
{
    DWMDEBUG("");
    if(wmNULL == pData)
    {
        DWMERROR("pData is null");
        return ;
    }
    
    if(WMFileExist(pAndroidMacPatch))
    {
        wmQWORD qwFileSize = WMFileSize(pAndroidMacPatch);
        if(qwFileSize > (wmQWORD)iDateLen)
        {
            qwFileSize = iDateLen;
        }
        
        wmINT iFd = wmOpen(pAndroidMacPatch,O_RDONLY);
        if(ErrorCodeALL == iFd)
        {
            DWMERROR("wmOpen is ErrorCodeALL");
            return ;
        }
        DwmMemset(pData, 0x0, sizeof(pData));
        wmINT iError = wmRead(iFd,pData,qwFileSize);
        if(ErrorCodeALL == iError)
        {
            DWMERROR("wmRead is ErrorCodeALL");
            wmClose(iFd);
            return ;
        }
        wmClose(iFd);
    }
}

wmU32T UseHtonlto(wmU32T u32Host)
{
    return htonl(u32Host);
}

wmU16T UseHtonsto(wmU16T u16Host)
{
    return htons(u16Host);
}

wmU32T UseNtohlto(wmU32T u32Net)
{
    return ntohl(u32Net);
}

wmU16T UseNtohsto(wmU16T u16Net)
{
    return ntohs(u16Net);
}


wmVOID CreateArpPacket(SArpPacket& apPacket, wmCCHAR* pCurrentMac)
{
    DWMDEBUG("");
    if(wmNULL == pCurrentMac)
    {
        DWMERROR("pCurrentMac is null");
        return ;
    }

    FOR(ETHERNET_ADDR_LEN)
    {
        wmCHAR pBit[3] = {0};
        DwmSafeStrcpy(pBit, 2, pCurrentMac);
        DWMDEBUG(pBit);
        apPacket.eEthernetHeader.pSrcEthernetAddr[i] = strtol(pBit,wmNULL,16);
        DWMDEBUG("apPacket.eEthernetHeader.pSrcEthernetAddr[i] = [%d]",apPacket.eEthernetHeader.pSrcEthernetAddr[i]);
        pCurrentMac+=3;
    }

    apPacket.eEthernetHeader.uEthernetType = UseHtonsto(ETHERTYPE_ARP);
    apPacket.eEthernetArp.uArpHardwareAddress = UseHtonsto(ARPHRD_ETHER);
    apPacket.eEthernetArp.uArpProtocolAddress = UseHtonsto(ETHERTYPE_IP);
    apPacket.eEthernetArp.uArpHardwareAddressLen = ETHERNET_ADDR_LEN;
    apPacket.eEthernetArp.uArpProtocolAddressLen = 4;
    apPacket.eEthernetArp.uArpRarpOperation = UseHtonsto(ARPOP_REQUEST);

    DwmMemcpy(apPacket.eEthernetArp.pSenderHardwareAddress, apPacket.eEthernetHeader.pSrcEthernetAddr, 0x0, ETHERNET_ADDR_LEN);
}
    


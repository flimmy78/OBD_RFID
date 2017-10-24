#ifndef _SOCKETLIB_H_
#define _SOCKETLIB_H_
#include "BaseDefine.h"
#include "BaseType.h"

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(SOCKET)(~0)
#endif 

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	(-1)
#endif  

#define pLOCAL_IP_ADDR                            "127.0.0.1"

/*
#define IPPROTO_IP 0            // dummy for IP 
#define IPPROTO_ICMP 1      // control message protocol 
#define IPPROTO_IGMP 2      // internet group management protocol 
#define IPPROTO_GGP 3       // gateway^2 (deprecated) 
#define IPPROTO_TCP 6       // tcp
#define IPPROTO_PUP 12      // pup
#define IPPROTO_UDP 17      // user datagram protocol 
#define IPPROTO_IDP 22      // xns idp 
#define IPPROTO_ND 77       // UNOFFICIAL net disk proto 
#define IPPROTO_RAW 255 // raw IP packet 
#define IPPROTO_MAX 256
*/
enum IPPROTO_TYPE{
    IPPROTO_TYPE_IP ,
    IPPROTO_TYPE_ICMP,
    IPPROTO_TYPE_IGMP,
    IPPROTO_TYPE_GGP,
    IPPROTO_TYPE_TCP,
    IPPROTO_TYPE_PUP,
    IPPROTO_TYPE_UDP,
    IPPROTO_TYPE_IDP,
    IPPROTO_TYPE_ND,
    IPPROTO_TYPE_RAW,
    IPPROTO_TYPE_MAX,
};

#if defined(_WINDOWS_OS_)
wmVOID SocketLibInit();
wmVOID SocketLibFree();
#endif
    
// Creates a socket.
// @return: true if succeeded, false on error
wmBOOL CreateSocket(wmINT& iSoecket, IPPROTO_TYPE itType);

// Closes a socket.
// @param iSoecket: socket.
// @return: true if succeeded, false on error
wmBOOL CloseSocket(wmINT& iSoecket);

// Connects a socket to remote address
// @param iSoecket: socket to operate on
// @param pSendtoAddr: address to Connec the socket to
// @param usSendtoPort: Port to Connec the socket to
// @return: true if succeeded, false on error
wmBOOL ConnectBySocket(wmINT iSoecket, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort);

// Binds a socket to a specific address
// @param iSoecket: socket to operate on
// @param pSendtoAddr: address to bind the socket to
// @param usSendtoPort: Port to bind the socket to
// @return: true if succeeded, false on error
wmBOOL BindBySocket(wmINT iSoecket, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort);

// Start listening
// @param iSoecket: socket to operate on
// @return: true if succeeded, false on error
wmBOOL ListenBySocket(wmINT iSoecket);

// Accepts a client
// @param iSoecket: client will be accepted from this socket
// @param pSendtoAddr: client address
// @param usSendtoPort: client Port
// @return: a valid client socket if succeeded, INVALID_SOCKET on error
wmINT AcceptBySocket(wmINT iSoecket, wmUSHORT usSendtoPort, wmCHAR* pSendtoAddr);

// TCP Send Interface
// @param iSoecket: socket to operate on
// @param pBuff: Buff to send
// @param siBuffSize: Size to send
// @return: return send bit, other then SOCKET_ERROR if succeeded, INVALID_SOCKET on error
wmINT SendBySocket(wmINT iSoecket, wmCCHAR* pBuff, wmSIZET siBuffSize);

// TCP receive/send timeout Interface
// @param iSoecket: socket to operate on
// @param iTime: wait time
// @return: true if succeeded, false on error
wmBOOL RecvTimeOutOptBySocket(wmINT iSoecket, wmINT iTime);
wmBOOL SendTimeOutOptBySocket(wmINT iSoecket, wmINT iTime);
wmBOOL SetReuseaddrOptBySocket(wmINT iSoecket);

// TCP receive Interface
// @param iSoecket: socket to operate on
// @param pBuff: Buff to receive
// @param siBuffSize: Size to receive
// @return: return send bit, other then SOCKET_ERROR if succeeded, INVALID_SOCKET on error
wmINT RecvBySocket(wmINT iSoecket, wmCHAR* pBuff, wmSIZET siBuffSize);

// UDP Send Interface
// @param iSoecket: socket to operate on
// @param pBuff: Buff to send
// @param siBuffSize: Size to send
// @param pSendtoAddr: client address
// @param usSendtoPort: client Port
// @return: return send bit, other then SOCKET_ERROR if succeeded, INVALID_SOCKET on error
wmINT SendToBySocket(wmINT iSoecket, wmCCHAR* pBuff, wmSIZET siBuffSize, wmCCHAR* pSendtoAddr, wmUSHORT usSendtoPort);

// UDP Send Interface
// @param iSoecket: socket to operate on
// @param pBuff: Buff to send
// @param siBuffSize: Size to send
// @param pSendtoAddr: client address
// @param usSendtoPort: client Port
// @return: return send bit, other then SOCKET_ERROR if succeeded, INVALID_SOCKET on error
wmINT RecvfromBySocket(wmINT iSoecket, wmCHAR*pBuff, wmSIZET siBuffSize, wmUSHORT& usSendtoPort, wmCHAR* pSendtoAddr);

// get Host ip addr.
wmVOID GetMacAdd(wmCHAR* pData, wmINT iDateLen);

// Utility functions for endian conversion.
wmU32T UseHtonlto(wmU32T u32Host);
wmU16T UseHtonsto(wmU16T u16Host);
wmU32T UseNtohlto(wmU32T u32Net);
wmU16T UseNtohsto(wmU16T u16Net);
#endif // _SOCKETLIB_H_

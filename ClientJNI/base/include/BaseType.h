/**
 * @file BaseType.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include "BaseDefine.h"
#include "BaseSize.h"
#include "BaseMemory.h"

#define BIT(n)                (0x1 << n)

#define FOR(cnt) for(wmINT i=0; i<(cnt); i++)

#define DECLARE_MASKABLE(T)    T operator| (T const& a, T const& b); \
                            T operator|=(T& self, T const& b);

#define DEFINE_MASKABLE(T)    T operator| (T const& a, T const& b) { return static_cast<T>(unsigned(a) | unsigned(b)); } \
                            T operator|=(T& self, T const& b)    { self = static_cast<T>(unsigned(self) | unsigned(b)); return self; }

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define STRCMP(pC1,pC2) \
(((wmNULL == pC1)||(wmNULL == pC2))?wmTRUE:(strcmp(pC1,pC2)))\


enum ReceivedCmd{
    RECEIVEDCMDUSER,
    RECEIVEDCMDALL
};

enum SkpFrameEnum {
    SKP_FRAME_BIT = 0,
    SKP_FRAME_ID = 10,
    SKP_FRAME_QUE = 12,
    SKP_FRAME_LEN = 20 ,
    SKP_FRAME_MAX = 28
};

enum {
	ZIP_TYPE_NONE = 0,
	ZIP_TYPE_ZLIB
};

#define MAX_BLOCKSIZE  184320

#define ENCODE_ZLIB_MIN_COMP_SIZE (17)


typedef enum
{
    EN_STRING_CHECK_TYPE_NOCHECK = 0,
    EN_STRING_CHECK_TYPE_FUZZY_CHECK,
    EN_STRING_CHECK_TYPE_EXACT_CHECK,
} EN_STRING_CHECK_TYPE;

typedef enum
{
    SOCKET_DATA_DATA_LEN_BIT_START = 0,
    SOCKET_DATA_DATA_LEN_BIT_DATA_LEN = 4,
    SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN = 8,
    SOCKET_DATA_DATA_LEN_BIT_DEST_LEN = 12,
    SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN = 16,
    SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN = 20,
} SOCKET_DATA_DATA_LEN_BIT;


typedef enum
{
    EN_SYSTEM_INFORMATION_TYPE_NONE = 0,
    EN_SYSTEM_INFORMATION_TYPE_CONFIG,
    EN_SYSTEM_INFORMATION_TYPE_WIRTE_CHANNELS,
    EN_SYSTEM_INFORMATION_TYPE_REQUEST_APPLY,
    EN_SYSTEM_INFORMATION_TYPE_KEY_CODE,
    EN_SYSTEM_INFORMATION_TYPE_AREA_CODE,
    EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,
    EN_SYSTEM_INFORMATION_TYPE_DISPLAYS_DEV,
    EN_SYSTEM_INFORMATION_TYPE_VIDEO_H264 = 0x10,
    EN_SYSTEM_INFORMATION_TYPE_VIDEO_YV12 = 0x11,
    EN_SYSTEM_INFORMATION_TYPE_VIDEO_RGB32 = 0x12,
    EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO = 0x97,
    EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO_STOP = 0x98,
    EN_SYSTEM_INFORMATION_TYPE_RFID_REQUEST_TO_PAY = 0x99,
    EN_SYSTEM_INFORMATION_TYPE_RFID_CONFIRM_PAY_MENT,
    EN_SYSTEM_INFORMATION_TYPE_MAX,
} EN_SYSTEM_INFORMATION_TYPE;

typedef enum
{
    EN_RECEIV_BUFF_TYPE_NONE = 0,
    EN_RECEIV_BUFF_TYPE_IPADDR = 5,
    EN_RECEIV_BUFF_TYPE_TCP = 11,
    EN_RECEIV_BUFF_TYPE_UDP = 21,
    EN_RECEIV_BUFF_TYPE_MAX,
} EN_RECEIV_BUFF_TYPE;

typedef struct SDataBuff
{
    wmINT  iType;
    wmINT  iSocket;
    wmSIZET sBuffSize;
    wmVOID* pBuff;

    void Init()
    {
        pBuff = wmNULL;
        sBuffSize = 0;
        iType = 0;
        iSocket = 0;
    }

    SDataBuff()
    {
        Init();
    }
}SDataBuff;

typedef enum
{
    EN_KEY_CODE_TYPE_MOUSE = 0,
    EN_KEY_CODE_TYPE_KEYBD,
    EN_KEY_CODE_TYPE_MAX,
} EN_KEY_CODE_TYPE;

typedef enum
{
    EN_MOUSE_CODE_TYPE_LEFT = 0,
    EN_MOUSE_CODE_TYPE_RIGHT,
    EN_MOUSE_CODE_TYPE_MID,
    EN_MOUSE_CODE_TYPE_MOVE,
    EN_MOUSE_CODE_TYPE_MAX,
} EN_MOUSE_CODE_TYPE;

typedef enum
{
    EN_MOUSE_CODE_EVENT_DOWN = 0,
    EN_MOUSE_CODE_EVENT_UP,
    EN_MOUSE_CODE_EVENT_UP_DOWN,
    EN_MOUSE_CODE_EVENT_DOUBLE_UP_DOWN,
    EN_MOUSE_CODE_EVENT_ROLL = 5,
    EN_MOUSE_CODE_EVENT_MAX,
} EN_MOUSE_CODE_EVENT;

typedef enum
{
    EN_KEYBD_CODE_EVENT_DOWN = 0,
    EN_KEYBD_CODE_EVENT_UP,
} EN_KEYBD_CODE_EVENT;


#define ETHERNET_ADDR_LEN 6

typedef struct SEthernetHeader 
{
    wmU8T pDstEthernetAddr[ETHERNET_ADDR_LEN];   
    wmU8T pSrcEthernetAddr[ETHERNET_ADDR_LEN];  
    wmUSHORT uEthernetType;   

    wmVOID Init()
    {
        DwmMemset(pDstEthernetAddr,0xFF,sizeof(pDstEthernetAddr));
        DwmMemset(pSrcEthernetAddr,0x0,sizeof(pSrcEthernetAddr));
        uEthernetType = wmFalse;
    }

    SEthernetHeader()
    {
        Init();
    }
}SEthernetHeader;

typedef struct SEthernetArp
{
    wmUSHORT uArpHardwareAddress;   
    wmUSHORT uArpProtocolAddress;    
    wmU8T uArpHardwareAddressLen;    
    wmU8T uArpProtocolAddressLen;  
    wmUSHORT uArpRarpOperation;    

    wmU8T pSenderHardwareAddress[ETHERNET_ADDR_LEN];    
    wmULONG uSenderProtocolAddress;   
    wmU8T pTargetHardwareAddress[ETHERNET_ADDR_LEN];    
    wmULONG uTargetProtocolAddress;  


    wmVOID Init()
    {
        uArpHardwareAddress = wmFalse;
        uArpProtocolAddress = wmFalse;
        uArpHardwareAddressLen = wmFalse;
        uArpProtocolAddressLen = wmFalse;
        uArpRarpOperation = wmFalse;
    
        DwmMemset(pSenderHardwareAddress,0x0,sizeof(pSenderHardwareAddress));
        uSenderProtocolAddress = wmFalse;
        DwmMemset(pTargetHardwareAddress,0xFF,sizeof(pTargetHardwareAddress));
        uTargetProtocolAddress = wmFalse;
    }

    SEthernetArp()
    {
        Init();
    }
}SEthernetArp;

typedef struct SArpPacket
{
    SEthernetHeader eEthernetHeader;
    SEthernetArp eEthernetArp;
}*PArpPacket;

#endif //end _BASE_TYPE_H_


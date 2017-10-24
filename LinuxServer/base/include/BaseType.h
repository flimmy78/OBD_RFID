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
    EN_RFID_GROUP_TYPE_RFID_DEVICE,
    EN_RFID_GROUP_TYPE_DISPLAY_DEVICE,
    EN_RFID_GROUP_TYPE_USERS,
    EN_RFID_GROUP_TYPE_MAX
} EN_RFID_GROUP_TYPE;

typedef struct SDataBuffRfidDevItem
{
    wmINT  iSocketId;
    
    wmINT iStatus;

    wmCHAR pDevtId[BASE_STRING_LEN_MIN];
    wmCHAR pIpaddr[BASE_STRING_LEN_MIN];
    wmCHAR pPassword[NAME_STRING_LEN];
    wmQWORD llRegTime;
    wmULONG ulActiveTime;
    
    void Init()
    {
        iSocketId = wmFalse;
        iStatus = wmFalse;
        llRegTime = wmFalse;
        ulActiveTime = wmFalse;
        DwmMemset(pDevtId, 0x0, sizeof(pDevtId));
        DwmMemset(pIpaddr, 0x0, sizeof(pIpaddr));
        DwmMemset(pPassword, 0x0, sizeof(pPassword));
    }

    SDataBuffRfidDevItem()
    {
        Init();
    }
}SDataBuffRfidDevItem;

typedef struct SDataBuffUserItem
{
    wmINT  iSocketId;
    wmINT  iLimit;
    wmINT iStatus;
    
    wmCHAR pName[NAME_STRING_LEN];
    wmCHAR pPassword[NAME_STRING_LEN];
    wmQWORD llRegTime;
    wmQWORD llLoginTime;
    wmULONG ulActiveTime;
    
    void Init()
    {
        iSocketId = wmFalse;
        iLimit = wmFalse;
        iStatus = wmFalse;
        llRegTime = wmFalse;
        llLoginTime = wmFalse;
        ulActiveTime = wmFalse;
        DwmMemset(pName, 0x0, sizeof(pName));
        DwmMemset(pPassword, 0x0, sizeof(pPassword));
    }

    SDataBuffUserItem()
    {
        Init();
    }
}SDataBuffUserItem;

typedef struct SDataBuff
{
    EN_RFID_GROUP_TYPE  iType;
    wmSIZET sBuffSize;
    wmVOID* pBuff;

    void Init()
    {
        pBuff = wmNULL;
        sBuffSize = wmFalse;
        iType = EN_RFID_GROUP_TYPE_MAX;
    }

    SDataBuff()
    {
        Init();
    }
}SDataBuff;

typedef enum
{
    EN_RFID_CODE_EVENT_START = 0xbb,
    EN_RFID_CODE_EVENT_NETINFO = 0x3a,
    EN_RFID_CODE_EVENT_NETINFO_ALL = 0xab,
    EN_RFID_CODE_EVENT_HEARBAET = 0x40,
    EN_RFID_CODE_EVENT_EPC_DATA = 0x99,
    EN_RFID_CODE_EVENT_EPC_DATA_EPCID = 0x9a,
    EN_RFID_CODE_EVENT_EPC_AUTO = 0x97,
    EN_RFID_CODE_EVENT_EPC_AUTO_STOP = 0x98,
} EN_RFID_CODE_EVENT;


#define cstrGoodsType  "goodsType"
#define cstrGoodsName  "goodsName"
#define cstrGoodsID  "goodsID"
#define cstrPutInTime  "putInTime"
#define cstrGoodsProperty  "goodsProperty"
#define cstrGoodsSIC  "goodsSIC"
#define cstrGoodsCost  "cost"
#define cstrGoodsCostoff "costoff"

#define cstrBGoodsType  "bGoodsType"
#define cstrIGoodsTypeFrom  "iGoodsTypeFrom"
#define cstrIGoodsTypeTo  "iGoodsTypeTo"

#define cstrIGoodsName  "iGoodsName"
#define cstrPGoodsNameFrom  "pGoodsNameFrom"

#define cstrBGoodsID  "bGoodsID"
#define cstrIGoodsIDFrom  "iGoodsIDFrom"
#define cstrIGoodsIDTo  "iGoodsIDTo"

#define cstrBPutInTime  "bPutInTime"
#define cstrPPutInTimeFrom  "pPutInTimeFrom"
#define cstrPPutInTimeTo  "pPutInTimeTo"

#define cstrBGoodsProperty  "bGoodsProperty"
#define cstrIGoodsPropertyFrom  "iGoodsPropertyFrom"
#define cstrIGoodsPropertyTo  "iGoodsPropertyTo"

#define cstrIGoodsSIC  "iGoodsSIC"
#define cstrPGoodsSICFrom  "pGoodsSICFrom"

#define cstrBGoodsCost  "bCost"
#define cstrIGoodsCostFrom  "iGoodsCostFrom"
#define cstrIGoodsCostTo  "iGoodsCostTo"

#define cstrBGoodsCostoff "bCostoff"
#define cstrIGoodsCostoffFrom  "iGoodsCostoffFrom"
#define cstrIGoodsCostoffTo  "iGoodsCostoffTo"

#endif //end _BASE_TYPE_H_


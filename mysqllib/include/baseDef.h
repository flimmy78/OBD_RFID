/**
 * @file baseDef.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */

#ifndef __BASE_DEF_H__
#define __BASE_DEF_H__
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
#include <typeinfo>
#include <stdio.h>


//base def
#define wmVECTOR    std::vector
#define wmMAP          std::map
#define wmDEQUE      std::deque
#define wmUNIQUEPTR   std::unique_ptr
#define wmSHAREDPTR   std::shared_ptr

#define wmINT         int
#define wmLONG        long
#define wmCHAR        char
#define wmCCHAR       const char
#define wmBOOL        bool
#define wmVOID        void
#define wmNULL        NULL
#define wmNull          "null"
#define wmTRUE        true
#define wmFALSE      false 
#define wmTrue        1
#define wmFalse      0 
#define wmULONG     unsigned long
#define wmQWORD     unsigned long long
#define wmUINT        unsigned int
#define wmUSHORT   unsigned short
#define wmSHORT     short
#define wmFLOAT     float
#define wmDOUBLE   double
#define wmSIZET      size_t
#define wmU8T         unsigned char
#define wmU16T       wmUSHORT
#define wmU32T       wmUINT
#define wmV4l2Buffer v4l2_buffer

#define wmTHREAD   Thread
#define wmGUID       GUID
#define wmSUCCEED SUCCEEDED
#define wmHRESULT  HRESULT
#define wmEXTERNC  EXTERN_C
#define wmSOK          S_OK
#define wmEFAIL       E_FAIL
#define wmEPOINTER  E_POINTER
#define wmEINVALIDARG  E_INVALIDARG
#define wmSFALSE  S_FALSE
#define wmVFWENOTFOUND  VFW_E_NOT_FOUND

#define _P_LINUX_ 1

//base unit
#define FILE_STRING_LEN 256
#define NAME_STRING_LEN 255
#define BASE_STRING_LEN_MIN 256
#define BASE_STRING_LEN_MAX 1024
#define INTERFACE_STRING_LEN 4096
#define SOCKET_STRING_ALL_LEN 1500
#define SOCKET_STRING_LEN 924//576
#define SOCKET_STRING_DATA_LEN 896//548
#define SOCKET_STRING_DATA_TAGET 28
#define SOCKET_DATA_CONNECT_LEN 8
#define SOCKET_DATA_CONNECT_FLAG 4
#define SOCKET_DATA_CMDPACKET_MINLEN 12
#define SOCKET_DATA_CMDPACKET_MIN_WAIT_TIME 200 //ms
#define TIME_STRING_LEN 128
#define IDATA_STRING_LEN 64
#define SQL_CMD_MAX       10*4096  
#define JS_DATA_MAX       512*4096    

//Base File 
#define pCONFIG_FILE_PATCH                            ""
#define DwmFILEEND                                          SEEK_END
#define DwmFILECUR                                          SEEK_CUR
#define DwmFILESET                                          SEEK_SET
#define DwmFILE                                                FILE
#define DwmFopen(filename, mode)                  fopen(filename, mode)
#define DwmFeof                                                feof
#define DwmFclose(file)                                     fclose(file)
#define DwmFprintf(file, format, ...)                   fprintf(file, format, ##__VA_ARGS__)
#define DwmFgetc(file)                                      fgetc(file)
#define DwmFgets                                              fgets
#define DwmFread(buffer, size, count, file)      fread(buffer, size, count, file)
#define DwmFwrite(buffer, size, count, file)     fwrite(buffer, size, count, file)
#define DwmFseek(file,offset,start)                  fseek(file,offset,start)
#define DwmFflush(file)                                     fflush(file)
#define DwmFremove(path)                              remove(path)
#define DwmIOCTL                                             ioctl
#define DwmREAD                                              read
#define DwmMMAP                                              mmap
#define wmDELETE                                            delete
#define wmNEW                                                 new
#define DwmMYSQL                                            MYSQL
#define DwmMYSQLRES                                      MYSQL_RES
#define DwmMYSQLROW                                     MYSQL_ROW

#define PTRVECT_LOCK_NAME    "LOCK_OF_PTRVECTOR"


#define ErrorCodeALL          -1
#define ErrorCodeNoError          1

//Base printf


#define DFunctiom(a,b)                    \
 void CCmdPacket::a(wmCCHAR* strName,b  value) \
{ \
	wmCHAR szValue[IDATA_STRING_LEN]; \
	DwmMemset(szValue,0x0,sizeof(szValue)); \
	DwmSnprintf(szValue,IDATA_STRING_LEN,"%u",value); \
	if(!m_MapNameToValue.isKeyExist(strName)) \
	{ \
		m_MapNameToValue.addProperty(strName,szValue); \
	} \
	else \
	{ \
		m_MapNameToValue[strName]=szValue; \
	} \
}

#define BIT(n)                (0x1 << n)

#define FOR(cnt) for(wmINT i=0; i<(cnt); i++)

#define DECLARE_MASKABLE(T)    T operator| (T const& a, T const& b); \
                            T operator|=(T& self, T const& b);

#define DEFINE_MASKABLE(T)    T operator| (T const& a, T const& b) { return static_cast<T>(unsigned(a) | unsigned(b)); } \
                            T operator|=(T& self, T const& b)    { self = static_cast<T>(unsigned(self) | unsigned(b)); return self; }

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define STRCMP(pC1,pC2) \
(((wmNULL == pC1)||(wmNULL == pC2))?wmTRUE:(strcmp(pC1,pC2)))\


//Base function
#define DWMAssert  assert

wmVOID *DwmMalloc(wmULONG size);
wmVOID *DwmRealloc(wmVOID *ptr, wmUINT size);
wmVOID DwmFree(wmVOID *ptr);
wmVOID DwmMemset(wmVOID * ptr, wmINT value, wmULONG size);
wmVOID DwmMemcpy(wmVOID *desptr, wmVOID const *originptr, wmULONG offset, wmULONG length);
wmVOID DwmSleep(wmU32T milliseconds);
wmQWORD DwmGenerateSSRC();
wmQWORD DwmGetTimestamp();
wmVOID DwmGetGUID(wmCHAR * cBuf);
wmVOID DwmAllocpy(wmCHAR* &to, const wmCHAR * from);
wmVOID DwmLog(wmCHAR const* cata,wmCHAR const* pFilename,wmINT line, wmCHAR  const *tag , wmCHAR  const *info);
wmVOID DwmDeleteLog();
wmBOOL DwmFileCreate(wmCHAR const *pFilename);
wmBOOL DwmFileExists(wmCHAR const *pFilename);
wmBOOL DwmFileMvname(wmCHAR const *pFilename,wmCHAR const *pNewFilename);
wmBOOL DwmFileRemove(wmCHAR const *pFilename);
wmCHAR *DwmStrcpy(wmCHAR *to, wmCHAR const *from);
wmCHAR *DwmSafeStrcpy(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *src);
wmSIZET DwmStrlen(wmCHAR const *str);
wmVOID DwmStrncat(wmCHAR *dest, wmCHAR const *src, wmSIZET bufsize);
wmINT DwmSnprintf(wmCHAR *buffer, wmSIZET bufsize, wmCHAR const *format, ...);
wmQWORD DwmGetMonotonic();

wmVOID DwmTime(wmCHAR *buffer);
wmFLOAT DwmValueClamp(wmFLOAT val, wmFLOAT minVal, wmFLOAT maxVal);

wmU8T  read8 (wmVOID const *data, wmINT offset);
wmU16T read16(wmVOID const *data, wmINT offset);
wmUINT read32(wmVOID const *data, wmINT offset);
wmVOID Write8 (wmCHAR *data, wmINT offset, wmU8T  value);
wmVOID Write16(wmCHAR *data, wmINT offset, wmU16T value);
wmVOID Write32(wmCHAR *data, wmINT offset, wmINT  value);

#define DWMDEBUG(...)                    \
    do {                            \
        wmCHAR pBuffer[INTERFACE_STRING_LEN] = {0};        \
        ::DwmSnprintf(pBuffer, sizeof pBuffer, __VA_ARGS__);    \
        ::DwmLog("DEBUG",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
    } while (wmFALSE)

#define DWMWORNG(...)                    \
    do {                            \
        wmCHAR pBuffer[INTERFACE_STRING_LEN] = {0};        \
        ::DwmSnprintf(pBuffer, sizeof pBuffer, __VA_ARGS__);    \
        ::DwmLog("WORNG",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
    } while (wmFALSE)

#define DWMERROR(...)                    \
    do {                            \
        wmCHAR pBuffer[INTERFACE_STRING_LEN] = {0};        \
        ::DwmSnprintf(pBuffer, sizeof pBuffer, __VA_ARGS__);    \
        ::DwmLog("ERROR",__FILE__, __LINE__, __FUNCTION__, pBuffer);                \
    } while (wmFALSE)



#define SendDataByTcp(a,b)                    \
    C2SSession* pC2SSession = C2SSession::GetInstance();  \
    if(pC2SSession)  \
    {  \
    	wmCHAR *pData = wmNULL;   \
	wmULONG uLength = b.GetData(&pData); \
	if (pData) {  \
		pC2SSession->sendData(a,pData, uLength); \
		DwmFree(pData); \
	} \
    }

#define SendValueByTcp(a,b,c)                    \
    C2SSession* pC2SSession = C2SSession::GetInstance();  \
    if(pC2SSession)  \
    {  \
	if (b) {  \
		pC2SSession->sendData(a,b, c); \
		DwmFree(b); \
	} \
    }

#define toChattingControl(a,b)                    \
    chattingControl* pChattingControl = chattingControl::GetInstance();  \
    if(pChattingControl)  \
    {  \
	if (a) {  \
		pChattingControl->C2SProc(a,b); \
	} \
    }

enum ReceivedCmd{
    RECEIVEDCMDUSER,
    RECEIVEDCMDALL
};

typedef struct Camerabuffer {
    wmVOID *start;
    wmSIZET length;
    void Init()
    {
        start = wmNULL;
        length = 0;
    }
    
    Camerabuffer()
    {
        Init();
    }
}Camerabuffer;

//base struct
typedef struct SUserInfoParam
{
    wmULONG uID;  /*!< user id from the server   */
    wmULONG uServerID;  /*!<  channel id from the server  */
    wmINT      iStatus;  
    
    wmCHAR pName[NAME_STRING_LEN];   /*!< user name for login */
    wmCHAR pPassword[NAME_STRING_LEN]; /*!< user password for login  */

    wmCHAR pNickName[NAME_STRING_LEN];
    wmINT    iGender;
    wmCHAR pEmail[BASE_STRING_LEN_MIN];
    wmCHAR pBirthday[BASE_STRING_LEN_MIN];
    wmCHAR pJob[BASE_STRING_LEN_MAX];
    wmCHAR pEducation[BASE_STRING_LEN_MAX];
    wmCHAR pPhone[BASE_STRING_LEN_MIN];
    wmCHAR pAddress[BASE_STRING_LEN_MAX];
    
    wmULONG uOnlineTime;
    wmULONG uAllOnlineTime;
    void Init()
    {
        uID                    = 0;
        uServerID          = 0;
        iStatus               = 0;
        iGender              = 0;
        uOnlineTime       = 0;
        uAllOnlineTime    = 0;
        DwmMemset(pName, 0x0, sizeof(pName));
        DwmMemset(pPassword, 0x0, sizeof(pPassword));
        DwmMemset(pNickName, 0x0, sizeof(pNickName));
        DwmMemset(pEmail, 0x0, sizeof(pEmail));
        DwmMemset(pBirthday, 0x0, sizeof(pBirthday));
        DwmMemset(pJob, 0x0, sizeof(pJob));
        DwmMemset(pEducation, 0x0, sizeof(pEducation));
        DwmMemset(pPhone, 0x0, sizeof(pPhone));
        DwmMemset(pAddress, 0x0, sizeof(pAddress));
    }

    SUserInfoParam()
    {
        Init();
    }
} SUserInfoParam;

typedef struct SSystemInfoParam
{
    wmCHAR pServerIpAddress[BASE_STRING_LEN_MIN]; /*!<local ip addr  */
    wmINT    iServerIpPort;    

    wmCHAR pLocalIpv4Address[BASE_STRING_LEN_MIN]; /*!<local ip addr  */
    wmINT    iLocalIpv4Port;

    wmCHAR pLocalNetIpv4Address[BASE_STRING_LEN_MIN]; /*!<net ip addr  */
    wmINT    iLocalNetIpv4Port;

    wmCHAR pVersion[BASE_STRING_LEN_MIN]; /*!<dev version*/
    
    void Init()
    {
        iServerIpPort =0;
        iLocalIpv4Port = 0;
        iLocalNetIpv4Port   = 0;

        DwmMemset(pServerIpAddress, 0x0, sizeof(pServerIpAddress));
        DwmMemset(pLocalIpv4Address, 0x0, sizeof(pLocalIpv4Address));
        DwmMemset(pLocalNetIpv4Address, 0x0, sizeof(pLocalNetIpv4Address));
        DwmMemset(pVersion, 0x0, sizeof(pVersion));
    }

    SSystemInfoParam()
    {
        Init();
    }
} SSystemInfoParam;

typedef struct SMemberParam
{
    wmULONG uMemberId;
    wmULONG uMemberChannelId;

    wmCHAR pNetIpv4Addr[BASE_STRING_LEN_MIN]; /*!<net ip addr  */
    wmINT    iNetIpv4TcpPort;
    wmINT    iNetIpv4UdpPort;
    wmINT    iStatus;  

    wmBOOL bIsVideoOpen;
    wmBOOL bIsAudioOpen;
    
    void Init()
    {
        iNetIpv4TcpPort        = 0;
        iNetIpv4UdpPort        = 0;
        iStatus                     = 0;
        uMemberId               = 0;
        uMemberChannelId   = 0;
        
        DwmMemset(pNetIpv4Addr, 0x0, sizeof(pNetIpv4Addr));
    }

    SMemberParam()
    {
        Init();
    }
}SMemberParam;

typedef struct SMeetingList
{
    wmULONG uMeetingId;
    wmCHAR   pMeetingName[BASE_STRING_LEN_MIN];
    wmCHAR   pMeetingTitle[BASE_STRING_LEN_MIN];
    wmINT      iMeetingMode;
    
    wmULONG uCreateUserId;
    wmCHAR   pCreatorUserName[BASE_STRING_LEN_MIN];
    
    wmULONG uLectureId;
    
    wmINT      iMeetingStatus;
    wmULONG uStartTime;
    wmULONG uEndTime;

    wmCHAR   pHostPassword[BASE_STRING_LEN_MIN];
    wmCHAR   pPassword[BASE_STRING_LEN_MIN];

    wmVECTOR<SMemberParam *>  pMembersInfo;
    void Init()
    {
        uMeetingId              = 0;
        iMeetingMode           = 0;
        uCreateUserId         = 0;
        uLectureId               = 0;
        iMeetingStatus          = 0;
        uStartTime               = 0;
        uEndTime                 = 0;
        
        DwmMemset(pMeetingName, 0x0, sizeof(pMeetingName));
        DwmMemset(pMeetingTitle, 0x0, sizeof(pMeetingTitle));
        DwmMemset(pCreatorUserName, 0x0, sizeof(pCreatorUserName));
        DwmMemset(pHostPassword, 0x0, sizeof(pHostPassword));
        DwmMemset(pPassword, 0x0, sizeof(pPassword));
    }

    SMeetingList()
    {
        Init();
    }
}SMeetingList;


typedef struct SMemberFile
{
    wmULONG  uFileId;
    wmCHAR    pFileName[FILE_STRING_LEN];
    wmCHAR    pFilePath[FILE_STRING_LEN];
    wmCHAR    pFileMessage[FILE_STRING_LEN];
    wmCHAR    pFileUserName[FILE_STRING_LEN];
    
    wmULONG  uFileSize;
    wmCHAR    pFileModifyTime[BASE_STRING_LEN_MIN];

    void Init()
    {
        uFileId        = 0;
        uFileSize     = 0;
        
        DwmMemset(pFileName, 0x0, sizeof(pFileName));
        DwmMemset(pFilePath, 0x0, sizeof(pFilePath));
        DwmMemset(pFileMessage, 0x0, sizeof(pFileMessage));
        DwmMemset(pFileUserName, 0x0, sizeof(pFileUserName));
        DwmMemset(pFileModifyTime, 0x0, sizeof(pFileModifyTime));
    }

    SMemberFile()
    {
        Init();
    }
}SMemberFile;

typedef enum
{
    EN_STRING_CHECK_TYPE_NOCHECK = 0,
    EN_STRING_CHECK_TYPE_FUZZY_CHECK,
    EN_STRING_CHECK_TYPE_EXACT_CHECK,
} EN_STRING_CHECK_TYPE;



typedef struct SCheckItem
{
    wmBOOL bGoodsType;
    wmBOOL bGoodsID;
    wmBOOL bGutInTime;
    wmBOOL bGoodsProperty;
    wmBOOL bGoodsCost;
    wmBOOL bGoodsCostOff;

   
    wmINT iGoodsTypeFrom;
    wmINT iGoodsTypeTo;
    wmINT iGoodsIDFrom;
    wmINT iGoodsIDTo;
    wmINT iGoodsPropertyFrom;
    wmINT iGoodsPropertyTo;
    wmINT iGoodsCostFrom;
    wmINT iGoodsCostTo;
    wmINT iGoodsCostOffFrom;
    wmINT iGoodsCostOffTo;     

    EN_STRING_CHECK_TYPE iGoodsName;
    EN_STRING_CHECK_TYPE iGoodsSIC;
    
    
    wmCHAR pGoodsNameFrom[NAME_STRING_LEN+1];
    wmCHAR pGutInTimeFrom[NAME_STRING_LEN+1];
    wmCHAR pGutInTimeTo[NAME_STRING_LEN+1];
    wmCHAR pGoodsSICFrom[NAME_STRING_LEN+1];
    
    void Init()
    {
        bGoodsType = wmFALSE;
        bGoodsID = wmFALSE;
        bGutInTime = wmFALSE;
        bGoodsProperty = wmFALSE;
        bGoodsCost = wmFALSE;
        bGoodsCostOff = wmFALSE;

        iGoodsName = EN_STRING_CHECK_TYPE_NOCHECK;
        iGoodsSIC = EN_STRING_CHECK_TYPE_NOCHECK;
        
        iGoodsTypeFrom = wmFalse;
        iGoodsTypeTo = wmFalse;
        iGoodsIDFrom = wmFalse;
        iGoodsIDTo = wmFalse;
        iGoodsPropertyFrom = wmFalse;
        iGoodsPropertyTo = wmFalse;
        iGoodsCostFrom = wmFalse;
        iGoodsCostTo = wmFalse;
        iGoodsCostOffFrom = wmFalse;
        iGoodsCostOffTo = wmFalse;     
        
        memset(pGoodsNameFrom, 0x0, sizeof(pGoodsNameFrom));
        memset(pGutInTimeFrom, 0x0, sizeof(pGutInTimeFrom));
        memset(pGutInTimeTo, 0x0, sizeof(pGutInTimeTo));
        memset(pGoodsSICFrom, 0x0, sizeof(pGoodsSICFrom));
    }

    SCheckItem()
    {
        Init();
    }
}SCheckItem;

typedef struct SSqlItem
{
    wmCHAR pGoodsType[NAME_STRING_LEN+1];
    wmCHAR pGoodsID[NAME_STRING_LEN+1];
    wmCHAR pGutInTime[NAME_STRING_LEN+1];
    wmCHAR pGoodsProperty[NAME_STRING_LEN+1];
    wmCHAR pGoodsCost[NAME_STRING_LEN+1];
    wmCHAR pGoodsCostOff[NAME_STRING_LEN+1];
    wmCHAR pGoodsName[NAME_STRING_LEN+1];
    wmCHAR pGoodsSIC[NAME_STRING_LEN+1];
    
    void Init()
    {
        memset(pGoodsType, 0x0, sizeof(pGoodsType));
        memset(pGoodsID, 0x0, sizeof(pGoodsID));
        memset(pGutInTime, 0x0, sizeof(pGutInTime));
        memset(pGoodsProperty, 0x0, sizeof(pGoodsProperty));
        memset(pGoodsCost, 0x0, sizeof(pGoodsCost));
        memset(pGoodsCostOff, 0x0, sizeof(pGoodsCostOff));
        memset(pGoodsName, 0x0, sizeof(pGoodsName));
        memset(pGoodsSIC, 0x0, sizeof(pGoodsSIC));
    }

    SSqlItem()
    {
        Init();
    }
}SSqlItem;

#endif  /* __BASE_DEF_H__ */

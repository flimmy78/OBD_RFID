#include "ServerControl.h"
#include "BaseType.h"
#include "BaseSize.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "BaseMemory.h"
#include "BaseJson.h"
#include "SocketLib.h"
#include "Rfid.h"
#include "RfidData.h"
#include "MariaDBLibInterface.h"

CServerControl::CServerControl()
    : ServerControlThread(ThreadProcServerControl)
    , bIsServerControlStop(wmFALSE)
    , pTcpServerForRfid(wmNULL)
    , pTcpServerForUserAndDisDevs(wmNULL)
    , pBufferQueueNode(wmNULL)
    , mutexLock(wmNULL)
{
    
}

wmBOOL CServerControl::CServerControlInit()
{
    DWMDEBUG("");
    wmBOOL bRet = wmFALSE;
    
    if(wmNULL == pTcpServerForRfid)
    {
        pTcpServerForRfid = new TcpServer();
    }
    if(wmNULL == pTcpServerForUserAndDisDevs)
    {
        pTcpServerForUserAndDisDevs = new TcpServer();
    }

    if(wmNULL == pTcpServerForRfid || wmNULL == pTcpServerForUserAndDisDevs)
    {
       DWMERROR("pTcpServerForUserAndDisDevs or pTcpServerForRfid was NULL");
       return wmFALSE;
    }
    
    if(MariaDBLibInitialze())
    {
            wmBOOL bConnect = ConnectToMariaDB(pstrHostName,usDatabasePort,pstrUserName,pstrPassword,pstrDataBase);
            if(wmFALSE == bConnect)
            {
                DWMERROR("ConnectToMariaDB was filed");
                return wmFALSE;
            }
    }
    else
    {
       DWMERROR("MariaDBLibInitialze was filed");
       return wmFALSE;
    }

    mutexLock = WMCreateLock(DEQUE_LOCK_NAME);

    if(pTcpServerForRfid->CreateTcpServer(usRfidTcpServerPort))
    {
        if(pTcpServerForUserAndDisDevs->CreateTcpServer(usBaseTcpServerPort))
        {
            pBufferQueueNode = BitSideNodeNew();
            ServerControlThread.start(this);
            bRet = wmTRUE;
        }
        else
        {
           DWMERROR("band BaseTcpServerPort was filed");
        }
    }
    else
    {
       DWMERROR("band RfidTcpServerPort was filed");
    }

    if(!bRet)
    {
        if(mutexLock)
        {
            WMDestroyLock(mutexLock);    
        }
        
        if(pTcpServerForRfid)
        {
            pTcpServerForRfid->CloseTcpServer();
            wmDELETE pTcpServerForRfid;
            pTcpServerForRfid = wmNULL;
        }
        
        if(pTcpServerForUserAndDisDevs)
        {
            wmDELETE pTcpServerForUserAndDisDevs;
            pTcpServerForUserAndDisDevs = wmNULL;
        }
    }
    
    return bRet;
}

wmVOID CServerControl::ReceiveTcpHandleBuff(wmINT iType, wmINT iSocketId, wmCHAR* pBuff,wmINT iBuffLen, wmINT iDest, wmULONG lValue1, wmULONG lValue2)
{
    if(wmNULL == pBuff && wmNULL == pBufferQueueNode)
    {
       DWMERROR("pBuff or pBufferQueueNode is null");
       return;
    }

    unsigned wmCHAR* pvData = (unsigned wmCHAR* )DwmMalloc(iBuffLen+SOCKET_DATA_CONNECT_FLAG*2);
    if(wmNULL == pvData)
    {
        DWMERROR("DwmMalloc error");
        return;
    }
    DwmMemset(pvData,0x0,sizeof(pvData));
    
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_START)  = UseHtonlto(iBuffLen);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DATA_LEN)  = UseHtonlto(iType);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN)  = UseHtonlto(iSocketId);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_DEST_LEN)  = UseHtonlto(iDest);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN)  = UseHtonlto(lValue1);
    *reinterpret_cast<wmULONG *>(pvData + SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN)  = UseHtonlto(lValue2);
    DwmMemcpy(pvData+SOCKET_DATA_CONNECT_FLAG, pBuff, 0x0,iBuffLen);

    AddBitSideHead(pBufferQueueNode,0,iBuffLen+SOCKET_DATA_CONNECT_FLAG,pvData);
    DwmFree(pvData);
}

wmINT CServerControl::ThreadProcServerControl(void *pThis)
{
    CServerControl *pCServerControl = reinterpret_cast<CServerControl *>(pThis);
    if(wmNULL == pCServerControl)
    {       
       DWMERROR("pCServerControl is null");
       return wmFalse;
    }
    while (!pCServerControl->bIsServerControlStop)  
    {
        pBitSide pBitSideBuffer = BitSideNodeRemove(pCServerControl->pBufferQueueNode);
        if(pBitSideBuffer)
        {  
            pCServerControl->OnReceivedData((wmCHAR*)pBitSideBuffer->pData,pBitSideBuffer->uLen);
            if(pBitSideBuffer->pData)
                DwmFree(pBitSideBuffer->pData);
            DwmFree(pBitSideBuffer);
        }
    }
    return wmFalse;
}

wmBOOL CServerControl::RFIDAutoScan()
{
    DWMDEBUG("");
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    pTcpServerForRfid->SendData(wmFalse,EN_SYSTEM_INFORMATION_TYPE_RFID_CODE, (wmCHAR*)RFIDAutoScanComm, RFID_SIZE_AUTO_SCAN, 
    wmFalse,wmFalse,wmFalse);
}

wmBOOL CServerControl::RFIDStopScan()
{
    DWMDEBUG("");
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    pTcpServerForRfid->SendData(wmFalse, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,(wmCHAR*)RFIDStopScanComm, RFID_SIZE_STOP_SCAN, 
    wmFalse,wmFalse,wmFalse);
}

wmBOOL CServerControl::RFIDWrite(wmINT iImput)   
{
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    FOR(0x05)
    {
        RFIDWriteDataComm[0x13-i] = Get16to10(iImput - (iImput/0x64)*0x64);
        iImput /= 0x64;
        RFIDWriteDataComm[0x16] += RFIDWriteDataComm[0x13-i];
    }

    pTcpServerForRfid->SendData(wmFalse, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,(wmCHAR*)RFIDWriteDataComm, RFID_SIZE_WRITE_DATA, 
        wmFalse,wmFalse,wmFalse);
}
 
wmBOOL CServerControl::RFIDReadData() 
{
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    pTcpServerForRfid->SendData(wmFalse, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,(wmCHAR*)RFIDReadDataComm, RFID_SIZE_READ_DATA, 
    wmFalse,wmFalse,wmFalse);
}

wmBOOL CServerControl::RFIDReadEPCID() 
{
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    pTcpServerForRfid->SendData(wmFalse, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,(wmCHAR*)RFIDReadEPCIDComm, RFID_SIZE_READ_EPCID, 
        wmFalse,wmFalse,wmFalse);
}

wmBOOL CServerControl::RFIDDeviceID() 
{
    if(wmNULL == pTcpServerForRfid)
    {
       DWMERROR("pTcpServerForRfid is null");
       return wmFalse;
    }
    pTcpServerForRfid->SendData(wmFalse, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,(wmCHAR*)RFIDReadDeviceIDComm, RFID_SIZE_READ_DEVICEID, 
    wmFalse,wmFalse,wmFalse);
}

wmVOID createJsonDate(Json& jsCheckItems)
{
    jsCheckItems.addProperty(cstrBGoodsProperty, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrIGoodsPropertyFrom, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsPropertyTo, wmFalse);
    jsCheckItems.addProperty(cstrBGoodsType, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrIGoodsTypeFrom, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsTypeTo, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsName, EN_STRING_CHECK_TYPE_NOCHECK);
    jsCheckItems.addProperty(cstrPGoodsNameFrom, wmNULLCCSTR, wmTRUE);
    jsCheckItems.addProperty(cstrBPutInTime, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrPPutInTimeFrom, wmNULLCCSTR, wmTRUE);
    jsCheckItems.addProperty(cstrPPutInTimeTo, wmNULLCCSTR, wmTRUE);
    jsCheckItems.addProperty(cstrBGoodsID, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrIGoodsIDFrom, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsIDTo, wmFalse);
    jsCheckItems.addProperty(cstrBGoodsCostoff, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrIGoodsCostoffFrom, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsCostoffTo, wmFalse);
    jsCheckItems.addProperty(cstrBGoodsCost, wmFalseStr, wmTRUE);
    jsCheckItems.addProperty(cstrIGoodsCostFrom, wmFalse);
    jsCheckItems.addProperty(cstrIGoodsCostTo, wmFalse);   
    jsCheckItems.addProperty(cstrIGoodsSIC, EN_STRING_CHECK_TYPE_NOCHECK);   
    jsCheckItems.addProperty(cstrPGoodsSICFrom, wmNULLCCSTR, wmTRUE);   
}

wmVOID CServerControl::RFIDData(wmCHAR* pData,wmULONG nLen)
{
    if(wmNULL == pData)
    {
       DWMERROR("pData is null");
       return;
    }

    CRfid* pCRfid = CRfid::GetInstance();
    if(wmNULL == pCRfid)
    {
       DWMERROR("pCRfid is null");
       return;
    }

    wmUSHORT iType = wmFalse;
    wmCHAR* pRFIDCmd = (wmCHAR*)DwmMalloc(BASE_STRING_LEN_MIN);
    if(wmNULL == pRFIDCmd)
    {
       DWMERROR("pRFIDCmd is null");
       return;
    }
    DwmMemset(pRFIDCmd, 0x0, sizeof(pRFIDCmd));
    pCRfid->SwitchRfid(pData+SOCKET_DATA_CONNECT_FLAG,nLen,iType,pRFIDCmd);
    switch(iType)
    {
        case EN_RFID_CODE_EVENT_HEARBAET:
        {
            break;
        }
        case EN_RFID_CODE_EVENT_NETINFO_ALL:
        case EN_RFID_CODE_EVENT_NETINFO:
        {
            wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            SDataBuffRfidDevItem* pParam = new SDataBuffRfidDevItem();
            //Because there is no need to delete so here can be new, but do not delete
            
            pParam->iSocketId = iSocketId;
            pParam->iStatus = wmTrue;
            DwmStrcpy(pParam->pDevtId, pRFIDCmd);
            DWMDEBUG("pParam->pDevtId = [%s][%d]",pParam->pDevtId,pParam->iSocketId);
            RfidDeviceDeque.addBufferMap(pParam);
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_DATA:
        {
            wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            wmINT iGSocket = wmFalse;
            wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
            RfidDeviceDeque.GetDeviceId(iSocketId, pDeviceID, wmNULL);
            wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
            GetDeviceInfo(pstrRfidDevice,pDeviceID,pstrLedDevice,pLedDeviceID);
            RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
            if(wmFalse == iGSocket)
            {
                DWMERROR("can't get pstrLedDevice iSocketId=[%d],pLedDeviceID=[%s]",iSocketId,pDeviceID);
                break;
            }
            pTcpServerForUserAndDisDevs->SendData(iGSocket, EN_RFID_CODE_EVENT_EPC_DATA, pRFIDCmd, DwmStrlen(pRFIDCmd),wmFalse,wmFalse,wmFalse);
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_DATA_EPCID:
        {
            wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            wmINT iGSocket = wmFalse;
            wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
            RfidDeviceDeque.GetDeviceId(iSocketId, pDeviceID, wmNULL);
            wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
            GetDeviceInfo(pstrRfidDevice,pDeviceID,pstrLedDevice,pLedDeviceID);
            RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
            if(wmFalse == iGSocket)
            {
                DWMERROR("can't get pstrLedDevice iSocketId=[%d],pLedDeviceID=[%s]",iSocketId,pDeviceID);
                break;
            }
            pTcpServerForUserAndDisDevs->SendData(iGSocket, EN_RFID_CODE_EVENT_EPC_DATA_EPCID,pRFIDCmd, DwmStrlen(pRFIDCmd), wmFalse,wmFalse,wmFalse);
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_AUTO:
        {
            if(AddCurrentList(pRFIDCmd))
            {
                Json jsCheckItems("{}");
                createJsonDate(jsCheckItems);
                jsCheckItems[cstrIGoodsSIC] = EN_STRING_CHECK_TYPE_EXACT_CHECK;
                jsCheckItems[cstrPGoodsSICFrom] = pRFIDCmd;
                DWMDEBUG("[%s]",jsCheckItems.c_str());
                wmCHAR* pQequestCmd= (wmCHAR*)DwmMalloc(SQL_CMD_MAX+1);
                if(wmNULL == pQequestCmd)
                {
                    DWMERROR("DwmMalloc error");
                    break;
                }
                DwmMemset(pQequestCmd, 0x0, sizeof(pQequestCmd));
                if(!SelectItemsFromTable(jsCheckItems.c_str(),pQequestCmd))
                {
                    DWMERROR("SelectItemsFromTable error");
                    DwmFree(pQequestCmd);
                    break;         
                }
                DWMDEBUG("[%s]",pQequestCmd);
                wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
                wmINT iGSocket = wmFalse;
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                RfidDeviceDeque.GetDeviceId(iSocketId, pDeviceID, wmNULL);
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                GetDeviceInfo(pstrRfidDevice,pDeviceID,pstrLedDevice,pLedDeviceID);
                RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice iSocketId=[%d],pLedDeviceID=[%s]",iSocketId,pDeviceID);
                    break;
                }
                pTcpServerForUserAndDisDevs->SendData(iGSocket, EN_RFID_CODE_EVENT_EPC_AUTO, pQequestCmd, DwmStrlen(pQequestCmd), wmFalse, wmFalse, wmFalse);
                DwmFree(pQequestCmd);
            }
            break;
        }
        case EN_RFID_CODE_EVENT_EPC_AUTO_STOP:
        {
            ClearCurrentList();
            /*
            wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            wmINT iGSocket = wmFalse;
            wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
            RfidDeviceDeque.GetDeviceId(iSocketId, pDeviceID, wmNULL);
            wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
            GetDeviceInfo(pstrRfidDevice,pDeviceID,pstrLedDevice,pLedDeviceID);
            RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
            if(wmFalse == iGSocket)
            {
                DWMDEBUG("can't get pstrLedDevice iSocketId=[%d],pLedDeviceID=[%s]",iSocketId,pDeviceID);
                break;
            }
            pTcpServerForUserAndDisDevs->SendData(iGSocket, EN_RFID_CODE_EVENT_EPC_AUTO_STOP, "EN_RFID_CODE_EVENT_EPC_AUTO_STOP", 
                DwmStrlen("EN_RFID_CODE_EVENT_EPC_AUTO_STOP"), wmFalse, wmFalse, wmFalse);
                */
            DWMDEBUG("EN_RFID_CODE_EVENT_EPC_AUTO_STOP");
            break;
        }
        default:
        {
            DWMWORNG("iType = [%d]",iType);
            break;
        }    
    }
    DwmFree(pRFIDCmd);
}

wmVOID CServerControl::OnReceivedData(wmCHAR* pData,wmULONG nLen)
{
    if(wmNULL == pData)
    {
       DWMERROR("pData is null");
       return;
    }
    
    wmULONG iType = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DATA_LEN));

    switch(iType)
    {
        case EN_SYSTEM_INFORMATION_TYPE_RFID_CODE:
        {
            RFIDData(pData,nLen);
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_DISPLAYS_DEV:
        {
            wmULONG iSocketId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            SDataBuffRfidDevItem* pParam = new SDataBuffRfidDevItem();
            pParam->iSocketId = iSocketId;
            pParam->iStatus = wmTrue;
            DwmStrcpy(pParam->pDevtId, pData+SOCKET_DATA_CONNECT_FLAG);
            pParam->pDevtId[0x11] = '\0';
            RfidDisplayDeque.addBufferMap(pParam);
            DWMDEBUG("GetMacAdd = [%s][%d]",pParam->pDevtId,iSocketId);
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_H264:
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_YV12:
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_RGB32:
        {
            wmINT iGSocket = wmFalse;
            if(pTcpServerForUserAndDisDevs)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrLedDevice,pLedDeviceID);
                RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                wmULONG lValue1 = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_VALUE1_LEN));
                wmULONG lValue2 = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_VALUE2_LEN));
                pTcpServerForUserAndDisDevs->SendData(iGSocket, iType, pData+SOCKET_DATA_CONNECT_FLAG,
                    nLen-SOCKET_DATA_CONNECT_FLAG, lDest,lValue1,lValue2 );
            }
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO:
        {
            wmINT iGSocket = wmFalse;
            if(pTcpServerForRfid)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrRfidDevice,pLedDeviceID);
                
                RfidDeviceDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                pTcpServerForRfid->SendData(iGSocket,EN_SYSTEM_INFORMATION_TYPE_RFID_CODE, 
                    (wmCHAR*)RFIDAutoScanComm, RFID_SIZE_AUTO_SCAN, wmFalse,wmFalse,wmFalse);
            }
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO_STOP:
        {
            wmINT iGSocket = wmFalse;
            if(pTcpServerForRfid)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrRfidDevice,pLedDeviceID);
                
                RfidDeviceDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                pTcpServerForRfid->SendData(iGSocket, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,
                    (wmCHAR*)RFIDStopScanComm, RFID_SIZE_STOP_SCAN, wmFalse,wmFalse,wmFalse);
            }

            iGSocket = wmFalse;
            if(pTcpServerForUserAndDisDevs)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrLedDevice,pLedDeviceID);
                RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                wmULONG lSourceId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
                pTcpServerForUserAndDisDevs->SendData(iGSocket, EN_RFID_CODE_EVENT_EPC_AUTO_STOP, "EN_RFID_CODE_EVENT_EPC_AUTO_STOP", 
                DwmStrlen("EN_RFID_CODE_EVENT_EPC_AUTO_STOP"), lSourceId, wmFalse, wmFalse);
            }
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_RFID_REQUEST_TO_PAY:
        {
            wmINT iGSocket = wmFalse;
            if(pTcpServerForRfid)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrRfidDevice,pLedDeviceID);
                
                RfidDeviceDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                pTcpServerForRfid->SendData(iGSocket, EN_SYSTEM_INFORMATION_TYPE_RFID_CODE,
                    (wmCHAR*)RFIDStopScanComm, RFID_SIZE_STOP_SCAN, wmFalse,wmFalse,wmFalse);
            }

            iGSocket = wmFalse;
            if(pTcpServerForUserAndDisDevs)
            {             
                wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
                wmCHAR pDeviceID[IDATA_STRING_LEN] = {wmFalse};
                wmCHAR pLedDeviceID[IDATA_STRING_LEN] = {wmFalse};
                DwmSnprintf(pDeviceID, IDATA_STRING_LEN, "%d",lDest);
                GetDeviceInfo(pstrDeviceID,pDeviceID,pstrLedDevice,pLedDeviceID);
                RfidDisplayDeque.GetSocketId(iGSocket,pLedDeviceID, wmNULL);
                if(wmFalse == iGSocket)
                {
                    DWMERROR("can't get pstrLedDevice lDest=[%d],pLedDeviceID=[%s]",lDest,pLedDeviceID);
                    break;
                }
                wmULONG lSourceId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
                DWMDEBUG("from [%d] pay",lSourceId);
                pTcpServerForUserAndDisDevs->SendData(iGSocket, iType, pData+SOCKET_DATA_CONNECT_FLAG,
                    nLen-SOCKET_DATA_CONNECT_FLAG, lSourceId,wmFalse,wmFalse);
            }
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_RFID_CONFIRM_PAY_MENT:
        {
            wmULONG lDest = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_DEST_LEN));
            wmULONG lSourceId = UseNtohlto(*(unsigned long*)(pData+SOCKET_DATA_DATA_LEN_BIT_SOURCE_LEN));
            DWMDEBUG("To [%d] pay =[%s]",lDest,pData+SOCKET_DATA_CONNECT_FLAG);
            pTcpServerForUserAndDisDevs->SendData(lDest, iType, pData+SOCKET_DATA_CONNECT_FLAG,
                    nLen-SOCKET_DATA_CONNECT_FLAG, lSourceId,wmFalse,wmFalse);
            break;
        }
        default:
        {
            break;
        }    
    }
}


wmBOOL CServerControl::AddCurrentList(wmCHAR* pEPCID)
{
    if(wmNULL == mutexLock || wmNULL == pEPCID)
    {
       DWMERROR("mutexLock is null");
       return wmFalse;
    }   
    
    wmBOOL bRet = wmTRUE;
    WMLock(mutexLock);
    for (wmLIST<wmCHAR*>::iterator iTerator = CurrentList.begin(); iTerator != CurrentList.end();++iTerator)
    {
        if(DwmStrcmp(*iTerator, pEPCID))
        {
            bRet = wmFALSE;
            break;
        }
    }
    
    if(bRet)
    {
        wmCHAR* pCurrentID = (wmCHAR*)DwmMalloc(BASE_STRING_LEN_MIN);
        if(wmNULL == pCurrentID)
        {
            bRet = wmFALSE;
        }
        else
        {
            DwmMemset(pCurrentID, 0x0, sizeof(pCurrentID));
            DwmStrcpy(pCurrentID, pEPCID);
            CurrentList.push_back(pCurrentID);
            DWMDEBUG("pCurrentID = [%s]",pCurrentID);
        }
    }
    WMUnLock(mutexLock);
    return bRet;
}

wmBOOL CServerControl::ClearCurrentList()
{
    if(wmNULL == mutexLock)
    {
       DWMERROR("mutexLock is null");
       return wmFalse;
    }    
    WMLock(mutexLock);
    while(CurrentList.size() > 0)
    {
        wmLIST<wmCHAR*>::iterator iTerator = CurrentList.begin();
        wmCHAR* pCurrentValue = *iTerator;
        CurrentList.erase(iTerator);
        if(pCurrentValue)
        {
            DwmFree(pCurrentValue);
        }
    }
    WMUnLock(mutexLock);
}
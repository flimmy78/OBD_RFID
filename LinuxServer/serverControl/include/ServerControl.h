/**
 * @file ServerControl.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1
 * 
 */
#ifndef __SERVER_CONTROL_H__
#define __SERVER_CONTROL_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseVector.h"
#include "BaseThread.h"
#include "TCPServerHandle.h"
#include "RfidDeque.h"


class CServerControl: public TInstance<CServerControl>
{
    friend class TInstance<CServerControl>;
public:
    // Server Control To Init
    // return: true is set success,  false is other error
    wmBOOL CServerControlInit();

    // return tcp call back data
    // @param iType: tcp data type 
    // @param iSocketId: tcp iSocketId  
    // @param pBuff: data buff
    // @param iBuffLen: data Len
    wmVOID ReceiveTcpHandleBuff(wmINT iType, wmINT iSocketId, wmCHAR* pBuff,wmINT iBuffLen, wmINT iDest,wmULONG lValue1, wmULONG lValue2);
    wmVOID RFIDData(wmCHAR* pData,wmULONG nLen);
    wmVOID OnReceivedData(wmCHAR* pData,wmULONG nLen);
    
public:
    wmBOOL RFIDAutoScan();
    wmBOOL RFIDStopScan();
    wmBOOL RFIDWrite(wmINT iImput);    
    wmBOOL RFIDReadData();  
    wmBOOL RFIDReadEPCID();
    wmBOOL RFIDDeviceID();

private:
    CServerControl();
    static wmINT  ThreadProcServerControl(void *pThis);
    
    pBitSideNode pBufferQueueNode;
    RfidDeque RfidDeviceDeque;
    RfidDeque RfidDisplayDeque;
    RfidDeque RfidUsersDeque;
    
    TcpServer* pTcpServerForRfid;
    TcpServer* pTcpServerForUserAndDisDevs;
    Thread    ServerControlThread;
    wmBOOL bIsServerControlStop;

    wmBOOL AddCurrentList(wmCHAR* pEPCID);
    wmBOOL ClearCurrentList();
    wmLIST<wmCHAR*> CurrentList;
    wmLock mutexLock;
};


#endif  // end __SERVER_CONTROL_H__
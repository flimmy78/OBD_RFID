/**
 * @file MainControl.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __MAINCONTORL_H__
#define __MAINCONTORL_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseEvent.h"
#include "BaseJson.h"
#include "BaseThread.h"
#include "BaseVector.h"

class CMainControl: public TInstance<CMainControl>
{
    friend class TInstance<CMainControl>;
public:
    // system Init, include sql , network , configure file
    // return: true is set success,  false is other error
    wmBOOL MainControlInit(wmINT iWidth, wmINT iHeight,wmINT iKeyFrameInterval,  wmINT ifpsNum);
    
    // connect to server via tcp
    // @param pConnectAddr: ID
    // @param usConnectPort: port
    // return: true is send success,  false is other error
    wmBOOL RequestConnectToServer(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort);
    
    // Set Callback For System information  
    // @param CALLBACK_SYSTEM_INFORMATION is function to regist 
    wmVOID SetCallbackForSystem(CALLBACK_SYSTEM_INFORMATION cbCallback);

    // login in server via tcp
    // @param pName: login name
    // @param pPassword: login password
    // return: true is send success,  false is other error
    wmBOOL LoginInServer(wmCCHAR* pName,wmCCHAR * pPassword);

    // login out server via tcp
    // return: true is send success,  false is other error
    wmBOOL LoginOutServer();

    wmBOOL SendBuff(wmINT iType,  wmVOID* pBuff, wmINT iBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmINT GetSendBuffCount();
    
    // return tcp call back data
    // @param iType: tcp data type 
    // @param pBuff: data buff
    // @param iBuffLen: data Len
    wmVOID CBTcpHandleBuff(wmINT iType, wmCHAR* pBuff,wmINT iBuffLen);
private:
    CMainControl();
    
    Json jsSerachItem;
    CALLBACK_SYSTEM_INFORMATION cbSystemInformation;

    pBitSideNode pEncodeBufferQueueNode;
};


#endif  // end __MAINCONTORL_H__
/**
 * @file ClientMainControl.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef _CLIENT_MAIN_CONTROL_H_
#define _CLIENT_MAIN_CONTROL_H_
#include "BaseEvent.h"
 
#ifdef _WINDOWS_OS_
#define TOP_API __stdcall
#else
#define TOP_API
#endif


#ifdef __cplusplus
extern "C" {
#endif  

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
	// return: true is send success,  false is other error
    wmBOOL SetCallbackForSystem(CALLBACK_SYSTEM_INFORMATION cbCallback);

    // login in server via tcp
    // @param pName: login name
    // @param pPassword: login password
    // return: true is send success,  false is other error
    wmBOOL LoginInServer(wmCCHAR* pName, wmCCHAR * pPassword);

    // login out server via tcp
    // return: true is send success,  false is other error
    wmBOOL LoginOutServer();

    wmBOOL SendBuff(wmINT iType,  wmVOID* pBuff, wmINT iBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2);
    wmINT GetSendBuffCount();
#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // end _CLIENT_MAIN_CONTROL_H_




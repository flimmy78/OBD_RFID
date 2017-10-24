#include "MainControl.h"
#include "BaseType.h"
#include "BaseSize.h"
#include "BaseDebug.h"
#include "TCPHandleLibInterface.h"
#include "VideoEncodeHandle.h"
#include "SocketLib.h"
#include "BaseMemory.h"
#include "BaseTime.h"
#include <math.h>
#include "stdio.h"
#include "opencv2/opencv.hpp"

CMainControl::CMainControl()
    : cbSystemInformation(wmNULL)
    , pEncodeBufferQueueNode(wmNULL)
{

}

// system Init, include sql , network , configure file
wmBOOL CMainControl::MainControlInit(wmINT iWidth, wmINT iHeight, wmINT iKeyFrameInterval,  wmINT ifpsNum)
{
    DWMDEBUG("");
    pEncodeBufferQueueNode = BitSideNodeNew();

    if(wmFALSE == TCPClientLibInitialze(wmNULL))  
    {
        DWMERROR("TCPClientLibInitialze error");
        return wmFALSE;
    }  

    CVideoEncodeHandle* pVideoEncodeHandle = CVideoEncodeHandle::GetInstance();
    if(wmNULL == pVideoEncodeHandle)
    {
       DWMERROR("CVideoEncodeHandle::GetInstance error");
       return wmFALSE;
    }
    pVideoEncodeHandle->Init(iWidth, iHeight, iKeyFrameInterval, ifpsNum);
    return wmTRUE;
}

wmBOOL CMainControl::RequestConnectToServer(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort)
{
    return ConnectToServer(pConnectAddr,usConnectPort);
}

// Set Callback For System information  
wmVOID CMainControl::SetCallbackForSystem(CALLBACK_SYSTEM_INFORMATION cbCallback)
{
    DWMDEBUG("");
    if(cbCallback)
        cbSystemInformation =  cbCallback;
}

// login in server via tcp
wmBOOL CMainControl::LoginInServer(wmCCHAR* pName,wmCCHAR * pPassword)
{
    DWMDEBUG("");
    Json jsUserLogin(pstrLoginServerJson);
    jsUserLogin[pstrLoginName] = pName;
    jsUserLogin[pstrPassword] = pPassword;
    jsUserLogin[pstrType] = pstrLogin;
    
    return SendDataToServer(EN_SYSTEM_INFORMATION_TYPE_REQUEST_APPLY, jsUserLogin.c_str(),jsUserLogin.getStrLen(),wmFalse, wmFalse, wmFalse);
}


// login in server via tcp
wmBOOL CMainControl::LoginOutServer()
{
    DWMDEBUG("");
    Json jsUserLogin(pstrLoginServerJson);
    jsUserLogin[pstrType] = pstrLoginOut;
    
    SendDataToServer(EN_SYSTEM_INFORMATION_TYPE_REQUEST_APPLY,jsUserLogin.c_str(),jsUserLogin.getStrLen(), wmFalse, wmFalse, wmFalse);
    return TCPClientLibExit();
}

wmBOOL CMainControl::SendBuff(wmINT iType,  wmVOID* pBuff, wmINT iBuffLen, wmINT iDest, wmINT iValue1, wmINT iValue2)
{
    return SendDataToServer(iType,reinterpret_cast<char const *>(pBuff),iBuffLen,iDest,iValue1,iValue2);
}

wmINT CMainControl::GetSendBuffCount()
{
    if(pEncodeBufferQueueNode)
    {
        return BitSideNodeGetCount(pEncodeBufferQueueNode);
    }
    else
    {
        return wmFalse;
    }
}

wmVOID CMainControl::CBTcpHandleBuff(wmINT iType, wmCHAR* pBuff,wmINT iBuffLen)
{
    DWMDEBUG("");
    if(cbSystemInformation)
    {
         cbSystemInformation(iType,pBuff,iBuffLen);
    }
}


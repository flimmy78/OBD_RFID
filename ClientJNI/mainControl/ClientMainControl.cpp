#include "MainControl.h"
#include "BaseDebug.h"
#include "ClientMainControl.h"


// system Init, include sql , network , configure file
wmBOOL MainControlInit(wmINT iWidth, wmINT iHeight,wmINT iKeyFrameInterval,  wmINT ifpsNum)
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->MainControlInit(iWidth,iHeight,iKeyFrameInterval,ifpsNum);
}

wmBOOL RequestConnectToServer(wmCCHAR *pConnectAddr, wmUSHORT usConnectPort)
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->RequestConnectToServer(pConnectAddr,usConnectPort);
}


// Set Callback For System information  
wmBOOL SetCallbackForSystem(CALLBACK_SYSTEM_INFORMATION cbCallback)
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    pCMainControl->SetCallbackForSystem(cbCallback);
    return wmTRUE;
}

// login in server via tcp
wmBOOL LoginInServer(wmCCHAR* pName,wmCCHAR * pPassword)
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->LoginInServer(pName,pPassword);
    
}


// login in server via tcp
wmBOOL LoginOutServer()
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->LoginOutServer();
    
}

wmBOOL SendBuff(wmINT iType,  wmVOID* pBuff, wmINT iBuffLen, wmINT iDest,wmINT iValue1, wmINT iValue2)
{
    DWMDEBUG("");
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->SendBuff(iType,pBuff,iBuffLen,iDest,iValue1,iValue2);
}

wmINT GetSendBuffCount()
{
    CMainControl* pCMainControl = CMainControl::GetInstance();
    if(wmNULL == pCMainControl)
    {
       DWMERROR("CMainControl::GetInstance error");
       return wmFALSE;
    }
    return pCMainControl->GetSendBuffCount();
}


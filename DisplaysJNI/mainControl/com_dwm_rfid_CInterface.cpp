#include "com_dwm_rfid_CInterface.h"
#include <android/log.h>
#include <pthread.h>
#include "BaseDefine.h"
#include "BaseDebug.h"
#include "BaseTime.h"
#include "TCPHandle.h"
#include "BaseType.h"
#include "VideoFrameConverter.h"

extern "C" {
static wmCCHAR* psttClass = "com/dwm/rfid/CInterface";
#define  DwmGetString(a)    ev->GetStringUTFChars(a, &iscopy)
#define  DwmFreeString(a,b)  ev->ReleaseStringUTFChars(a,b)
#define  DwmNewByteArray(a)    jniEnvControl->NewByteArray(a)
#define  DwmGetArray(a)    jniEnvControl->GetByteArrayElements(a,&iscopy)
#define  DwmFreeArray(a,b)  jniEnvControl->ReleaseByteArrayElements(a,b,JNI_ABORT)

#define  DwmNewString(a)        jniEnvControl->NewString((const jchar*)a,strlen(a))
#define  DwmNewStringUTF(a)  jniEnvControl->NewStringUTF(a)
#define  DwmNewByteArray(a)     jniEnvControl->NewByteArray(a)

/*
 * Class:    com_interface
 * Method:    getver
 * Signature: ([B)V
 */
static JavaVM *vmCurrentVm = wmFalse;
static jclass jcCallBack;
static JNIEnv* jniEnvControl = wmFalse;
static TCPHandle* pTCPHandle = wmNULL;
static jmethodID jmBufMethodId;
static jmethodID jmStringMethodId;
static VideoFrameConverter Fconverter;

wmVOID CallbackForSystemBuf(wmINT iType, wmCHAR* pBuff, wmINT iBuffLen, wmINT iValue1, wmINT iValue2);

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pVm, void *)
{
    if(wmNULL == pVm)
    {
        DWMERROR("pVm is NULL");
        return wmFalse;
    }
    
    JNIEnv* pJniEnv = wmNULL;
    pVm->GetEnv((void**)&pJniEnv,JNI_VERSION_1_6);
    if(wmNULL == pJniEnv)
    {
        DWMERROR("pJniEnv is NULL");
        return wmFalse;
    }
    
    jniEnvControl = pJniEnv;
    vmCurrentVm = pVm;
    
    jclass jcClass = pJniEnv->FindClass(psttClass);
    jcCallBack = reinterpret_cast<jclass>(pJniEnv->NewGlobalRef(jcClass));
    jmBufMethodId = jniEnvControl->GetStaticMethodID(jcCallBack,"NotifyForSystemBuf","(I[BIII)V");
    
    jmStringMethodId = jniEnvControl->GetStaticMethodID(jcCallBack,"NotifyForSystemString","(ILjava/lang/String;I)V");
    return JNI_VERSION_1_6;
}


DFJNIBool Java_com_dwm_rfid_CInterface_Init(DFJNIEc)
{
    DWMDEBUG("");
    if(wmNULL == pTCPHandle)
    {
        pTCPHandle = new TCPHandle();
    }
    else
    {
        DWMERROR("double init pTCPHandle");
        return wmFALSE;
    }

    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is NULL");
        return wmFALSE;
    }
    return pTCPHandle->Init(CallbackForSystemBuf);
}

DFJNIBool Java_com_dwm_rfid_CInterface_Close(DFJNIEc)
{
    DWMDEBUG("");
    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is NULL");
        return wmFALSE;
    }
    pTCPHandle->Close();
    wmDELETE(pTCPHandle);
    pTCPHandle = wmNULL;
    return wmTRUE;
}

DFJNIBool Java_com_dwm_rfid_CInterface_ConnectToServer(DFJNIEc, jstring strServerAddr, jint iPort)
{
    if(wmNULL == pTCPHandle)
    {
        DWMDEBUG("pTCPHandle is nuLL");
        return wmFALSE;
    }
    jboolean iscopy = JNI_FALSE;
    wmCCHAR * pServerAddr = DwmGetString(strServerAddr);
    wmBOOL bRet = pTCPHandle->Connect(pServerAddr,iPort);
    DwmFreeString(strServerAddr,pServerAddr);
    return bRet;
}

DFJNIInt Java_com_dwm_rfid_CInterface_IsConnected(DFJNIEc)
{
    DWMDEBUG("");
    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is NULL");
        return wmFalse;
    }
    return pTCPHandle->IsConnected();
}

DFJNIBool Java_com_dwm_rfid_CInterface_RegardToServer(DFJNIEc)
{
    if(wmNULL == pTCPHandle)
    {
        DWMDEBUG("pTCPHandle is nuLL");
        return wmFALSE;
    }    
    wmBOOL bRet = pTCPHandle->RegardToServer();
    return bRet;
}

DFJNIBool Java_com_dwm_rfid_CInterface_RequestToPay(DFJNIEc, jdouble dM)
{
    if(wmNULL == pTCPHandle)
    {
        DWMDEBUG("pTCPHandle is nuLL");
        return wmFALSE;
    }    
    wmCHAR pBuff[IDATA_STRING_LEN] = {0};
    DwmSnprintf(pBuff, IDATA_STRING_LEN, "%f",dM);
    DWMDEBUG("pBuff = [%s]",pBuff);
    DWMDEBUG("dM = [%f]",dM);
    wmBOOL bRet = pTCPHandle->SendData(pBuff, DwmStrlen(pBuff), EN_SYSTEM_INFORMATION_TYPE_RFID_CONFIRM_PAY_MENT);
    return bRet;
}

DFJNIBool Java_com_dwm_rfid_CInterface_DisConnect(DFJNIEc)
{
    DWMDEBUG("");
    if(wmNULL == pTCPHandle)
    {
        DWMERROR("pTCPHandle is NULL");
        return wmFALSE;
    }
    return pTCPHandle->DisConnect();
}


wmVOID CallbackForSystemBuf(wmINT iType, wmCHAR* pBuff, wmINT iBuffLen, wmINT iValue1, wmINT iValue2)
{
    DWMDEBUG("start");
    if((wmNULL == vmCurrentVm) ||(wmNULL == jniEnvControl) ||(wmNULL == pBuff))
    {
        DWMERROR("vmCurrentVm or jniEnvControl is null");
        return;
    }

    vmCurrentVm->AttachCurrentThread(&jniEnvControl, wmNULL);
    
    jboolean iscopy = JNI_FALSE;

    switch(iType)
    {
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_H264:
        {
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_YV12:
        {
            wmINT iSize = iValue1*iValue2*3;
            wmU8T * pOutBGR24Frame = (wmU8T *)DwmMalloc(iSize);
            if(wmNULL == pOutBGR24Frame)
            {
                DWMERROR("DwmMalloc error");
                break;
            }
            DwmMemset(pOutBGR24Frame, 0x0, sizeof(pOutBGR24Frame));
            Fconverter.YV12ToBGR24((wmU8T*)pBuff, iValue1, iValue2, pOutBGR24Frame);
            
            jbyteArray ptr = DwmNewByteArray(iSize);
            jbyte* pData = DwmGetArray(ptr);
            DwmMemcpy(pData, pOutBGR24Frame, 0, iSize);
            jniEnvControl->CallStaticVoidMethod(jcCallBack,jmBufMethodId,iType,ptr,iBuffLen,iValue1,iValue2);
            DwmFreeArray(ptr,pData);
            DwmFree(pOutBGR24Frame);
            break;
        }            
        case EN_SYSTEM_INFORMATION_TYPE_VIDEO_RGB32:
        {
            jbyteArray ptr = DwmNewByteArray(iBuffLen);
            jbyte* pData = DwmGetArray(ptr);
            DwmMemcpy(pData, pBuff, 0, iBuffLen);
            jniEnvControl->CallStaticVoidMethod(jcCallBack,jmBufMethodId,iType,ptr,iBuffLen,iValue1,iValue2);
            DwmFreeArray(ptr,pData);
            break;
        }
        case EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO_STOP:
        case EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO:
        case EN_SYSTEM_INFORMATION_TYPE_RFID_REQUEST_TO_PAY:
        {
            wmCHAR pCurrentBuff[BASE_STRING_LEN_MIN] = {0};
            DwmMemcpy(pCurrentBuff, pBuff, 0x0, iBuffLen);
            wmINT iStringLen = DwmStrlen(pCurrentBuff);
            jstring jsString = DwmNewStringUTF(pCurrentBuff);
            jniEnvControl->CallStaticVoidMethod(jcCallBack,jmStringMethodId,iType,jsString,iStringLen);
            jniEnvControl->DeleteLocalRef(jsString);
            break;
        }
        default:
        {
            break;
        }
    }
    vmCurrentVm->DetachCurrentThread();
    DWMDEBUG("end");
}

}

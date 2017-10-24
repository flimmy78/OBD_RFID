#include "com_dwm_control_mainControl.h"
#include <android/log.h>
#include <pthread.h>
#include "ClientMainControl.h"
#include "BaseDefine.h"
#include "BaseDebug.h"
#include "UDPHandle.h"
#include "BaseTime.h"

extern "C" {

#define  DwmGetString(a)    jniEnvControl->GetStringUTFChars(a, &iscopy)
#define  DwmFreeString(a,b)  jniEnvControl->ReleaseStringUTFChars(a,b)

#define  DwmGetArray(a)    jniEnvControl->GetByteArrayElements(a,&iscopy)
#define  DwmGetArrayLength(a)    jniEnvControl->GetArrayLength(a)
#define  DwmFreeArray(a,b)  jniEnvControl->ReleaseByteArrayElements(a,b,JNI_ABORT)

#define  DwmNewString(a)        jniEnvControl->NewString((const jchar*)a,strlen(a))
#define  DwmNewStringUTF(a)  jniEnvControl->NewStringUTF(a)
#define  DwmNewByteArray(a)     jniEnvControl->NewByteArray(a)

static wmCCHAR* psttClass = "com/dwm/base/CInterface";

/*
 * Class:    com_interface
 * Method:    getver
 * Signature: ([B)V
 */
static JavaVM *vmCurrentVm = wmFalse;
static JNIEnv* enCurrentEnv = wmFalse;
static jclass jcCallBack;
static JNIEnv* jniEnvControl = wmFalse;

static jmethodID jmStringMethodId;

wmVOID CallbackForSystem(wmINT iType, wmCHAR* pBuff, wmINT iBuffLen);

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
    return JNI_VERSION_1_6;
}


/*
 * note:    com_interface
 */
#define NUMTHREADS 1
DFJNIBool Java_com_dwm_base_CInterface_Init(DFJNIEc, jint iBaseWidth, jint iBaseHight, jint nKeyFrameInterval,  jint ifpsNum)
{
    enCurrentEnv = ev;
    MainControlInit(iBaseWidth,iBaseHight,nKeyFrameInterval,ifpsNum);
    jmStringMethodId = jniEnvControl->GetStaticMethodID(jcCallBack,"NotifyForSystemString","(ILjava/lang/String;I)V");
    SetCallbackForSystem(CallbackForSystem);
    return wmTRUE;
}

DFJNIBool Java_com_dwm_base_CInterface_RequestConnectToServer(DFJNIEc, jstring strServerAddr, jint iPort)
{
    jboolean iscopy = JNI_FALSE;
    wmCCHAR * pServerAddr = DwmGetString(strServerAddr);
    wmBOOL bReturn = RequestConnectToServer(pServerAddr,iPort); 
    DwmFreeString(strServerAddr,pServerAddr);
    return bReturn;
}

DFJNIBool Java_com_dwm_base_CInterface_SendBuff(DFJNIEc, jint iType, jbyteArray pBuff, jlong lLen, jint iW, jint iH)
{
    jboolean iscopy = JNI_FALSE;
    jbyte *pJbData = DwmGetArray(pBuff);
    wmBOOL bRet = SendBuff(iType,pJbData,lLen,55660,iW,iH);
    DwmFreeArray(pBuff,pJbData);
    return bRet;
}

DFJNIBool Java_com_dwm_base_CInterface_RequestToScan(DFJNIEc)
{
    wmCHAR pData[IDATA_STRING_LEN] = {0};
    DwmSnprintf(pData, IDATA_STRING_LEN, "EPC_AUTO");
    wmBOOL bRet = SendBuff(EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO,
        pData,DwmStrlen(pData),55660,wmFalse,wmFalse);
    return bRet;
}

DFJNIBool Java_com_dwm_base_CInterface_RequestToStopScan(DFJNIEc)
{
    wmCHAR pData[IDATA_STRING_LEN] = {0};
    DwmSnprintf(pData, IDATA_STRING_LEN, "EPC_AUTO_STOP");
    wmBOOL bRet = SendBuff(EN_SYSTEM_INFORMATION_TYPE_RFID_COMM_EPC_AUTO_STOP,
        pData,DwmStrlen(pData),55660,wmFalse,wmFalse);
    return bRet;
}

DFJNIBool Java_com_dwm_base_CInterface_RequestToPay(DFJNIEc)
{
    wmCHAR pData[IDATA_STRING_LEN] = {0};
    DwmSnprintf(pData, IDATA_STRING_LEN, "REQUEST_TO_PAY");
    wmBOOL bRet = SendBuff(EN_SYSTEM_INFORMATION_TYPE_RFID_REQUEST_TO_PAY,
        pData,DwmStrlen(pData),55660,wmFalse,wmFalse);
    return bRet;
}

DFJNIInt Java_com_dwm_base_CInterface_GetSendBuffCount()
{
    return GetSendBuffCount();
}

wmVOID CallbackForSystem(wmINT iType, wmCHAR* pBuff, wmINT iBuffLen)
{
    DWMDEBUG("start");
    if((wmNULL == vmCurrentVm) ||(wmNULL == jniEnvControl) )
    {
        DWMERROR("vmCurrentVm or jniEnvControl is null");
        return;
    }

    vmCurrentVm->AttachCurrentThread(&jniEnvControl, wmNULL);
    wmCHAR pCurrentBuff[BASE_STRING_LEN_MIN] = {0};
    DwmMemcpy(pCurrentBuff, pBuff, 0x0, iBuffLen);
    wmINT iStringLen = DwmStrlen(pCurrentBuff);
    jstring jsString = DwmNewStringUTF(pCurrentBuff);
    jniEnvControl->CallStaticVoidMethod(jcCallBack,jmStringMethodId,iType,jsString,iStringLen);
    jniEnvControl->DeleteLocalRef(jsString);
    vmCurrentVm->DetachCurrentThread();
    DWMDEBUG("end");
}

}

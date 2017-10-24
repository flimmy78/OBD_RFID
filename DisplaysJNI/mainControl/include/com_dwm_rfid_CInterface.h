/* Header for class com_dwm_rfid_mainControl */

#ifndef _Included_com_dwm_rfid_CInterface
#define _Included_com_dwm_rfid_CInterface
#include <jni.h>
#include "BaseDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_interface
 * Method:    init
 * Signature: ()V
 */

DFJNIBool Java_com_dwm_rfid_CInterface_Init(DFJNIEc);
DFJNIBool Java_com_dwm_rfid_CInterface_Close(DFJNIEc);
DFJNIBool Java_com_dwm_rfid_CInterface_ConnectToServer(DFJNIEc, jstring strServerAddr, jint iPort);
DFJNIInt Java_com_dwm_rfid_CInterface_IsConnected(DFJNIEc);
DFJNIBool Java_com_dwm_rfid_CInterface_RegardToServer(DFJNIEc);
DFJNIBool Java_com_dwm_rfid_CInterface_RequestToPay(DFJNIEc, jdouble dM)
DFJNIBool Java_com_dwm_rfid_CInterface_DisConnect(DFJNIEc);


#ifdef __cplusplus
}
#endif
#endif

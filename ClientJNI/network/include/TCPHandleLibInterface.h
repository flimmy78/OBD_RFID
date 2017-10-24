#ifndef _TCP_CLIENT_HANDLE_H_
#define _TCP_CLIENT_HANDLE_H_

#include "BaseEvent.h"
#ifdef __cplusplus
extern "C" {
#endif 

bool TCPClientLibInitialze(CALLBACK_RECEIV_BUFF pCbReceivedBuff);
bool ConnectToServer(const char *pConnectAddr, unsigned short usConnectPort);
bool SendDataToServer(int iType, char const *pBuff, unsigned int sBuffLen, int iDest, int iValue1, int iValue2);
bool TCPClientLibExit();

#ifdef __cplusplus
} 
#endif  

#endif  // end _TCP_CLIENT_HANDLE_H_
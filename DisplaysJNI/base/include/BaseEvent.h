/**
 * @file BaseEvent.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_EVENT_H_
#define _BASE_EVENT_H_

// return system information 
// @param iType: system info type
// @param pBuff: system info
// @param iBuffLen: system info size
typedef void (*CALLBACK_SYSTEM_INFORMATION)(int iType, char* pBuff,int iBuffLen);

// return sock buff 
// @param iType: sock buff form id
// @param pBuff: sock buff 
// @param iBuffLen: sock buff size
typedef void (*CALLBACK_RECEIV_BUFF)(int iType, char* pBuff,int iBuffLen,int iValue1, int iValue2);

#endif //end _BASE_EVENT_H_


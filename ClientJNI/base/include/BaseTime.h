/**
 * @file BaseTime.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_TIME_H_
#define _BASE_TIME_H_

#include "BaseDefine.h"

wmQWORD MPGetMonotonic();
wmQWORD MPGetTimestamp();
wmVOID getTimeValue(wmCHAR* pTime);
wmVOID DwmSleep(wmU32T milliseconds);

#endif //end _BASE_TIME_H_


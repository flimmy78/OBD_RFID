/**
 * @file RunStataus.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __RUN_STATUS_H__
#define __RUN_STATUS_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseThread.h"
#include "BaseLock.h"

class CRunStatus: public TInstance<CRunStatus>
{
    friend class TInstance<CRunStatus>;
public:
    wmVOID setTcpServerPort(wmUSHORT iPort);
    wmUSHORT  getTcpServerPort();
private:
    CRunStatus();
    ~CRunStatus();
    wmUSHORT usTcpServerPort;
};


#endif  // end __RUN_STATUS_H__
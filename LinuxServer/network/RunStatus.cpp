#include "RunStatus.h"
#include "BaseTime.h"
#include "BaseDebug.h"

CRunStatus::CRunStatus()
    :usTcpServerPort(wmFalse)
{
    
}

CRunStatus::~CRunStatus()
{
    
}

wmVOID CRunStatus::setTcpServerPort(wmUSHORT iPort)
{
    usTcpServerPort = iPort;
}

wmUSHORT CRunStatus::getTcpServerPort()
{
    return usTcpServerPort;
}


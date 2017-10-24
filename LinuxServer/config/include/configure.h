/**
 * @file configure.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include "BaseDefine.h"
#include "BaseTable.h"
#include "BaseJson.h"


class CConfigure: public TInstance<CConfigure>
{
    friend class TInstance<CConfigure>;
public:
    wmBOOL ConfigureInit();
    wmCCHAR* Get(wmCCHAR* pTarget);
    wmBOOL Set(wmCCHAR* pTarget, wmCCHAR* pValue);
    wmBOOL ConfigureClose();

private:
    Json jsData;
};

#endif  // end __CONFIGURE_H__
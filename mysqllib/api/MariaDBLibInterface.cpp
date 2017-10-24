#include "MariaDBLibInterface.h"
#include "MysqlDB.h"
#include <time.h>
#include <deque>
#include "json.h"

static CMysqlDB* pCMysqlDB = wmNULL;

#define cstrGoodsType  "goodsType"
#define cstrGoodsName  "goodsName"
#define cstrGoodsID  "goodsID"
#define cstrPutInTime  "putInTime"
#define cstrGoodsProperty  "goodsProperty"
#define cstrGoodsSIC  "goodsSIC"
#define cstrGoodsCost  "cost"
#define cstrGoodsCostoff "costoff"

#define cstrBGoodsType  "bGoodsType"
#define cstrIGoodsTypeFrom  "iGoodsTypeFrom"
#define cstrIGoodsTypeTo  "iGoodsTypeTo"

#define cstrIGoodsName  "iGoodsName"
#define cstrPGoodsNameFrom  "pGoodsNameFrom"

#define cstrBGoodsID  "bGoodsID"
#define cstrIGoodsIDFrom  "iGoodsIDFrom"
#define cstrIGoodsIDTo  "iGoodsIDTo"

#define cstrBPutInTime  "bPutInTime"
#define cstrPPutInTimeFrom  "pPutInTimeFrom"
#define cstrPPutInTimeTo  "pPutInTimeTo"

#define cstrBGoodsProperty  "bGoodsProperty"
#define cstrIGoodsPropertyFrom  "iGoodsPropertyFrom"
#define cstrIGoodsPropertyTo  "iGoodsPropertyTo"

#define cstrIGoodsSIC  "iGoodsSIC"
#define cstrPGoodsSICFrom  "pGoodsSICFrom"

#define cstrBGoodsCost  "bCost"
#define cstrIGoodsCostFrom  "iGoodsCostFrom"
#define cstrIGoodsCostTo  "iGoodsCostTo"

#define cstrBGoodsCostoff "bCostoff"
#define cstrIGoodsCostoffFrom  "iGoodsCostoffFrom"
#define cstrIGoodsCostoffTo  "iGoodsCostoffTo"

wmVOID getInsertValue(wmCCHAR *pImputJsString, wmCHAR* pInsertValue)
{
    Json jsImputString(pImputJsString);
    wmCCHAR* pGoodsType = jsImputString[cstrGoodsType].toStr();
    wmCCHAR* pGoodsName = jsImputString[cstrGoodsName].toStr();
    wmCCHAR* pGoodsID = jsImputString[cstrGoodsID].toStr();
    wmCCHAR* pPutInTime = jsImputString[cstrPutInTime].toStr();
    wmCCHAR* pGoodsProperty = jsImputString[cstrGoodsProperty].toStr();
    wmCCHAR* pGoodsSIC = jsImputString[cstrGoodsSIC].toStr();
    wmCCHAR* pGoodsCost = jsImputString[cstrGoodsCost].toStr();
    wmCCHAR* pGoodsCostoff = jsImputString[cstrGoodsCostoff].toStr();
    if((wmNULL == pGoodsType) ||
        (wmNULL == pGoodsName) ||
        (wmNULL == pGoodsID) ||
        (wmNULL == pPutInTime) ||
        (wmNULL == pGoodsProperty) ||
        (wmNULL == pGoodsSIC) ||
        (wmNULL == pGoodsCost) ||
        (wmNULL == pGoodsCostoff) 
        )
    {
        DWMERROR("pImputJsString is null");
        return;
    }  

    DwmSnprintf(pInsertValue,SQL_CMD_MAX,
        "insert into current_goods_table (goodsType,goodsName,goodsID,putInTime,goodsProperty,goodsSIC,cost,costoff) values(%u,'%s',%u,'%s',%u,'%s',%u,%u)",
        atoi(pGoodsType),pGoodsName,atoi(pGoodsID),pPutInTime,atoi(pGoodsProperty),pGoodsSIC,atoi(pGoodsCost),atoi(pGoodsCostoff));
    DWMDEBUG("pInsertValue = [%s]",pInsertValue);
}

wmVOID setCheckItems(SCheckItem& ciItem, wmCCHAR* pImputJsString)
{
    Json jsCheckItems(pImputJsString);
    
    ciItem.bGoodsType = jsCheckItems[cstrBGoodsType].toBool();
    if(ciItem.bGoodsType)
    {
        ciItem.iGoodsTypeFrom = jsCheckItems[cstrIGoodsTypeFrom].toInt();
        ciItem.iGoodsTypeTo = jsCheckItems[cstrIGoodsTypeTo].toInt();
    }    

    if(EN_STRING_CHECK_TYPE_NOCHECK != jsCheckItems[cstrIGoodsName].toInt())
    {
        wmCCHAR* pGoodsNameFrom = jsCheckItems[cstrPGoodsNameFrom].toStr();
        if(wmNULL != pGoodsNameFrom)
        {
            DwmSafeStrcpy(ciItem.pGoodsNameFrom,NAME_STRING_LEN,pGoodsNameFrom);
            if(EN_STRING_CHECK_TYPE_FUZZY_CHECK == jsCheckItems[cstrIGoodsName].toInt())
            {
                ciItem.iGoodsName = EN_STRING_CHECK_TYPE_FUZZY_CHECK;
            }
            else
            {
                ciItem.iGoodsName = EN_STRING_CHECK_TYPE_EXACT_CHECK;
            }
        }
    }

    ciItem.bGutInTime = jsCheckItems[cstrBPutInTime].toBool();
    if(ciItem.bGutInTime)
    {
        wmCCHAR* pGutInTimeFrom = jsCheckItems[cstrPPutInTimeFrom].toStr();
        wmCCHAR* pGutInTimeTo = jsCheckItems[cstrPPutInTimeTo].toStr();
        if(pGutInTimeFrom && pGutInTimeTo)
        {
            DwmSafeStrcpy(ciItem.pGutInTimeFrom,NAME_STRING_LEN,jsCheckItems[cstrPPutInTimeFrom].toStr());
            DwmSafeStrcpy(ciItem.pGutInTimeTo,NAME_STRING_LEN,jsCheckItems[cstrPPutInTimeTo].toStr());
        }
        else
        {
            ciItem.bGutInTime = wmFALSE;
        }
    }    

    ciItem.bGoodsID = jsCheckItems[cstrBGoodsID].toBool();
    if(ciItem.bGoodsID)
    {
        ciItem.iGoodsIDFrom = jsCheckItems[cstrIGoodsIDFrom].toInt();
        ciItem.iGoodsIDTo = jsCheckItems[cstrIGoodsIDTo].toInt();
    }    

    ciItem.bGoodsProperty = jsCheckItems[cstrBGoodsProperty].toBool();
    if(ciItem.bGoodsProperty)
    {
        ciItem.iGoodsPropertyFrom = jsCheckItems[cstrIGoodsPropertyFrom].toInt();
        ciItem.iGoodsPropertyTo = jsCheckItems[cstrIGoodsPropertyTo].toInt();
    }

    if(EN_STRING_CHECK_TYPE_NOCHECK != jsCheckItems[cstrIGoodsSIC].toInt())
    {
        wmCCHAR* pGoodsSICFrom = jsCheckItems[cstrPGoodsSICFrom].toStr();
        if(wmNULL != pGoodsSICFrom)
        {
            DwmSafeStrcpy(ciItem.pGoodsSICFrom,NAME_STRING_LEN,pGoodsSICFrom);
            if(EN_STRING_CHECK_TYPE_FUZZY_CHECK == jsCheckItems[cstrPGoodsSICFrom].toInt())
            {
                ciItem.iGoodsSIC = EN_STRING_CHECK_TYPE_FUZZY_CHECK;
            }
            else
            {
                ciItem.iGoodsSIC = EN_STRING_CHECK_TYPE_EXACT_CHECK;
            }
        }
    }

    ciItem.bGoodsCost = jsCheckItems[cstrBGoodsCost].toBool();
    if(ciItem.bGoodsCost)
    {
        ciItem.iGoodsCostFrom = jsCheckItems[cstrIGoodsCostFrom].toInt();
        ciItem.iGoodsCostTo = jsCheckItems[cstrIGoodsCostTo].toInt();
    }    

    ciItem.bGoodsCostOff = jsCheckItems[cstrBGoodsCostoff].toBool();
    if(ciItem.bGoodsCostOff)
    {
        ciItem.iGoodsCostOffFrom = jsCheckItems[cstrIGoodsCostoffFrom].toInt();
        ciItem.iGoodsCostOffTo = jsCheckItems[cstrIGoodsCostoffTo].toInt();
    }      
}

wmVOID getCheckItems(const SCheckItem ciItem, char* pQequestCmd)
{
    if(wmNULL == pQequestCmd)
    {
        return;
    }

    DwmSnprintf(pQequestCmd,SQL_CMD_MAX,"select * from current_goods_table ");
    
    wmBOOL IsHaveItems = wmFALSE;
    
    if(ciItem.bGoodsType)
    {
        IsHaveItems = wmTRUE;
        wmCHAR pbGoodsTypeCmd[NAME_STRING_LEN+1] = {0};
        if(ciItem.iGoodsTypeFrom == ciItem.iGoodsTypeTo)
        {
            DwmSnprintf(pbGoodsTypeCmd,NAME_STRING_LEN," where goodsType = %d ",ciItem.iGoodsTypeFrom);     
        }
        else
        {
            DwmSnprintf(pbGoodsTypeCmd,NAME_STRING_LEN," where goodsType >= %d and goodsType <= %d ",ciItem.iGoodsTypeFrom,ciItem.iGoodsTypeTo);      
        }
        DwmStrncat(pQequestCmd,pbGoodsTypeCmd,SQL_CMD_MAX);
    }

    if(EN_STRING_CHECK_TYPE_FUZZY_CHECK == ciItem.iGoodsName)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }

        wmCHAR pbGoodsNameCmd[NAME_STRING_LEN+1] = {0};
        DwmSnprintf(pbGoodsNameCmd,NAME_STRING_LEN,"goodsName REGEXP BINARY '^%s.*'  ",ciItem.pGoodsNameFrom);       
        DwmStrncat(pQequestCmd,pbGoodsNameCmd,SQL_CMD_MAX);
    }
    else if(EN_STRING_CHECK_TYPE_EXACT_CHECK == ciItem.iGoodsName)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsNameCmd[NAME_STRING_LEN+1] = {0};
        DwmSnprintf(pbGoodsNameCmd,NAME_STRING_LEN,"goodsName = '%s' ",ciItem.pGoodsNameFrom);     
        DwmStrncat(pQequestCmd,pbGoodsNameCmd,SQL_CMD_MAX);
    }


    if(ciItem.bGoodsID)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsIDCmd[NAME_STRING_LEN+1] = {0};
        if(ciItem.iGoodsIDFrom == ciItem.iGoodsIDTo)
        {
            DwmSnprintf(pbGoodsIDCmd,NAME_STRING_LEN,"goodsID = %d  ",ciItem.iGoodsIDFrom);
        }
        else
        {
            DwmSnprintf(pbGoodsIDCmd,NAME_STRING_LEN,"goodsID >= %d and goodsID <= %d ",ciItem.iGoodsIDFrom,ciItem.iGoodsIDTo);    
        }  
        DwmStrncat(pQequestCmd,pbGoodsIDCmd,SQL_CMD_MAX);
    } 


    if(ciItem.bGutInTime)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGutInTimeCmd[NAME_STRING_LEN+1] = {0};
        DwmSnprintf(pbGutInTimeCmd,NAME_STRING_LEN,"date_format(putInTime,'%%Y-%%m-%%d %%H:%%i:%%s') >='%s' and date_format(putInTime,'%%Y-%%m-%%d %%H:%%i:%%s')  <= '%s' ",ciItem.pGutInTimeFrom,ciItem.pGutInTimeTo);       
        DwmStrncat(pQequestCmd,pbGutInTimeCmd,SQL_CMD_MAX);
    }    

    if(ciItem.bGoodsProperty)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsPropertyCmd[NAME_STRING_LEN+1] = {0};
        if(ciItem.iGoodsPropertyFrom == ciItem.iGoodsPropertyTo)
        {
            DwmSnprintf(pbGoodsPropertyCmd,NAME_STRING_LEN,"goodsProperty = %d ",ciItem.iGoodsPropertyFrom);      
        }
        else
        {
            DwmSnprintf(pbGoodsPropertyCmd,NAME_STRING_LEN,"goodsProperty >= %d and goodsProperty <= %d ",ciItem.iGoodsPropertyFrom,ciItem.iGoodsPropertyTo);       
        }    
        DwmStrncat(pQequestCmd,pbGoodsPropertyCmd,SQL_CMD_MAX);
    }    

 
    if(EN_STRING_CHECK_TYPE_FUZZY_CHECK == ciItem.iGoodsSIC)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsSICCmd[NAME_STRING_LEN+1] = {0};
        DwmSnprintf(pbGoodsSICCmd,NAME_STRING_LEN,"goodsSIC REGEXP BINARY '^%s.*'  ",ciItem.pGoodsSICFrom);  
        DwmStrncat(pQequestCmd,pbGoodsSICCmd,SQL_CMD_MAX);
    }
    else if(EN_STRING_CHECK_TYPE_EXACT_CHECK == ciItem.iGoodsSIC)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsSICCmd[NAME_STRING_LEN+1] = {0};
        DwmSnprintf(pbGoodsSICCmd,NAME_STRING_LEN,"goodsSIC = '%s' ",ciItem.pGoodsSICFrom);     
        DwmStrncat(pQequestCmd,pbGoodsSICCmd,SQL_CMD_MAX);
    }

    if(ciItem.bGoodsCost)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsCostCmd[NAME_STRING_LEN+1] = {0};
        if(ciItem.iGoodsCostFrom == ciItem.iGoodsCostTo)
        {
            DwmSnprintf(pbGoodsCostCmd,NAME_STRING_LEN,"cost = %d ",ciItem.iGoodsCostFrom);      
        }
        else
        {
            DwmSnprintf(pbGoodsCostCmd,NAME_STRING_LEN,"cost >= %d and cost <= %d ",ciItem.iGoodsCostFrom,ciItem.iGoodsCostTo);       
        }     
        DwmStrncat(pQequestCmd,pbGoodsCostCmd,SQL_CMD_MAX);
    }       


    if(ciItem.bGoodsCostOff)
    {
        if(wmFALSE == IsHaveItems)
        {
            DwmStrncat(pQequestCmd," where ",SQL_CMD_MAX);
            IsHaveItems = wmTRUE;
        }
        else
        {
            DwmStrncat(pQequestCmd," and ",SQL_CMD_MAX);
        }
        wmCHAR pbGoodsCostOffCmd[NAME_STRING_LEN+1] = {0};
        if(ciItem.iGoodsCostOffFrom == ciItem.iGoodsCostOffTo)
        {
            DwmSnprintf(pbGoodsCostOffCmd,NAME_STRING_LEN,"costoff = %d ",ciItem.iGoodsCostOffFrom);      
        }
        else
        {
            DwmSnprintf(pbGoodsCostOffCmd,NAME_STRING_LEN,"costoff >= %d and costoff <= %d ",ciItem.iGoodsCostOffFrom,ciItem.iGoodsCostOffTo);       
        }   
        DwmStrncat(pQequestCmd,pbGoodsCostOffCmd,SQL_CMD_MAX);
    } 
        
    DWMDEBUG("pQequestCmd = [%s]",pQequestCmd);
}


wmBOOL getSqlItemsList(wmDEQUE<SSqlItem*>& stqSSqlItem, CMysqlDB* pCMysqlDBLib, char* pQequestCmd)
{
    wmBOOL bCMysql = wmFALSE;
    if(pCMysqlDBLib->SQLExec(pQequestCmd))
    {
        wmINT iRecordCount = pCMysqlDBLib->RecordCount();
        FOR(iRecordCount)
        {
            DwmMYSQLROW frFetchRows = pCMysqlDBLib->FetchRows();
            if(frFetchRows)
            {
                SSqlItem* pSSqlItem = new SSqlItem();
                DwmSafeStrcpy(pSSqlItem->pGoodsType, NAME_STRING_LEN, frFetchRows[0]);
                DwmSafeStrcpy(pSSqlItem->pGoodsName, NAME_STRING_LEN, frFetchRows[1]);
                DwmSafeStrcpy(pSSqlItem->pGoodsID, NAME_STRING_LEN, frFetchRows[2]);
                DwmSafeStrcpy(pSSqlItem->pGutInTime, NAME_STRING_LEN, frFetchRows[3]);
                DwmSafeStrcpy(pSSqlItem->pGoodsProperty, NAME_STRING_LEN, frFetchRows[4]);
                DwmSafeStrcpy(pSSqlItem->pGoodsSIC, NAME_STRING_LEN, frFetchRows[5]);
                DwmSafeStrcpy(pSSqlItem->pGoodsCost, NAME_STRING_LEN, frFetchRows[6]);
                DwmSafeStrcpy(pSSqlItem->pGoodsCostOff, NAME_STRING_LEN, frFetchRows[7]);
                stqSSqlItem.push_back(pSSqlItem);
            }
        }
        bCMysql = wmTRUE;
    }
    return bCMysql;
}


extern "C" {
    
bool MariaDBLibInitialze()
{
    DWMDEBUG("");
    if(wmNULL ==  pCMysqlDB)
    {
        pCMysqlDB = CMysqlDB::GetInstance();
        if(pCMysqlDB)
        {
            pCMysqlDB->MysqlInit();
            return wmTRUE;
        }
    }
    return wmFALSE;
}

bool ConnectToMariaDB(const char* pHostName,unsigned short usDbPort,const char* pUserName,const char* pPassword,const char* pDataBase)
{
    DWMDEBUG("");
    if(wmNULL ==  pCMysqlDB)
    {
        DWMERROR("pCMysqlDB is null");
        return wmFALSE;
    }

    wmBOOL bConnect = pCMysqlDB->Connect(pHostName,usDbPort,pUserName,pPassword,pDataBase);
    if(wmFALSE == bConnect)
    {
        DWMERROR("Connect mysql was failed");
    }
    return bConnect;
}
    
bool InsertValueIntoTable(char const *pImputJsString)
{
    DWMDEBUG("");
    if(wmNULL ==  pCMysqlDB)
    {
        DWMERROR("pCMysqlDB is null");
        return wmFALSE;
    }

    if(wmNULL ==  pImputJsString)
    {
        DWMERROR("pImputJsString is null");
        return wmFALSE;
    }

    wmCHAR pQequestCmd[SQL_CMD_MAX+1] = {0};
    getInsertValue(pImputJsString,pQequestCmd);
    wmBOOL bSQLExec = pCMysqlDB->SQLExec(pQequestCmd);
    if(wmFALSE == bSQLExec)
    {
        DWMERROR("SQLExec is failed");
    }
    return bSQLExec;
}

bool DeleteValueFromTable(wmULONG ulGoodsID)
{
    DWMDEBUG("");
    wmCHAR pQequestCmd[SQL_CMD_MAX+1] = {0};
    DwmSnprintf(pQequestCmd,SQL_CMD_MAX,"delete from current_goods_table where goodsID=%u",ulGoodsID);
    wmBOOL bSQLExec = pCMysqlDB->SQLExec(pQequestCmd);
    if(wmFALSE == bSQLExec)
    {
        DWMERROR("SQLExec is failed");
    }
    return bSQLExec;
}

bool SelectItemsFromTable(char const *pImputJsString, char *pOutJsString)
{
    if(wmNULL == pImputJsString || wmNULL == pOutJsString)
    {
        DWMERROR("pImputJsString || pOutJsString is null");
        return wmFALSE;
    }
    SCheckItem sSCheckItem;
    wmDEQUE<SSqlItem*> stqSSqlItem;
    setCheckItems(sSCheckItem,pImputJsString);
    wmCHAR pQequestCmd[SQL_CMD_MAX+1] = {0};
    getCheckItems(sSCheckItem,pQequestCmd);
    wmBOOL bSQLExec = getSqlItemsList(stqSSqlItem,pCMysqlDB,pQequestCmd);
    if(wmFALSE == bSQLExec)
    {
        DWMERROR("SQLExec is failed");
    }

    Json allSettingsData("[]");
    Json settingsTreeValue("{\"count\":null,\"target\":null,\"candidate\":[]}");
    
    wmINT iItemSize = stqSSqlItem.size();
    settingsTreeValue["count"]  = iItemSize;
    while (!stqSSqlItem.empty()){
        SSqlItem *pSSqlItem = stqSSqlItem.front();
        if(pSSqlItem)
        {
            Json childData("{\"val\":null}");
            wmCHAR pSSqlItemStr[BASE_STRING_LEN_MAX+1] = {0};
            DwmSnprintf(pSSqlItemStr,BASE_STRING_LEN_MAX,"%s>%s>%s>%s>%s>%s>%s>%s",
                    pSSqlItem->pGoodsType,pSSqlItem->pGoodsName,pSSqlItem->pGoodsID,
                    pSSqlItem->pGutInTime,pSSqlItem->pGoodsProperty,pSSqlItem->pGoodsSIC,pSSqlItem->pGoodsCost,pSSqlItem->pGoodsCostOff);
                    childData["val"] = pSSqlItemStr;
                    settingsTreeValue["candidate"].addVal(childData);
        }
        stqSSqlItem.pop_front();
        delete pSSqlItem;
    }
    allSettingsData.addVal(settingsTreeValue);
    DwmSnprintf(pOutJsString,JS_DATA_MAX,"%s",allSettingsData.c_str());
    DWMDEBUG("[%s]",pOutJsString);
    return (iItemSize > 0);
}

bool GetDeviceInfo(wmCCHAR* pFrom, wmCCHAR* pFromValue, wmCCHAR* pTarget, wmCHAR* pValue)
{
    if(wmNULL == pFrom ||
        wmNULL == pFromValue ||
        wmNULL == pTarget ||
        wmNULL == pValue )
    {
        DWMERROR("pDeviceId || pTarget || pValue is NULL ");
        return wmFALSE;
    }
    wmCHAR pQequestCmd[BASE_STRING_LEN_MIN+1] = {0};
    DwmSnprintf(pQequestCmd,BASE_STRING_LEN_MIN,"select %s from rfid_device_table where %s='%s'",pTarget,pFrom,pFromValue);
    DWMDEBUG("[%s]",pQequestCmd);
    if(pCMysqlDB->SQLExec(pQequestCmd))
    {
        wmINT iRecordCount = pCMysqlDB->RecordCount();
        if(iRecordCount > 0)
        {
            DwmMYSQLROW frFetchRows = pCMysqlDB->FetchRows();
            if(frFetchRows)
            {
                DwmSafeStrcpy(pValue, NAME_STRING_LEN, frFetchRows[0]);
                DWMDEBUG("[%s]",pValue);
                return wmTRUE;
            }
        }
    }
    DWMERROR("pCMysqlDB->SQLExec error");
    return wmFALSE;
}


bool MariaDBLibExit()
{
    if(pCMysqlDB)
    {
        pCMysqlDB->ReleaseInstance();
    }
    return wmTRUE;
}

}  // extern "C"

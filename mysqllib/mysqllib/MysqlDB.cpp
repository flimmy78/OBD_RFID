// MysqlDB.cpp : implementation file
//
#include "MysqlDB.h"
#define TEST_STR    "show tables"

wmVOID CMysqlDB::MysqlInit()
{
    DWMDEBUG("");
    mysql_init(&Mysql);
    pResult = wmNULL;    
}

wmBOOL CMysqlDB::Connect(wmCCHAR* pHostName,wmUSHORT usDbPort,
                                                wmCCHAR* pUserName,wmCCHAR* pPassword,wmCCHAR* pDataBase)
{
    DWMDEBUG("");

    if((wmNULL == pHostName)  || 
        (wmNULL == pUserName)  || 
        (wmNULL == pDataBase)  ||
         (wmNULL == pPassword))
    {
        DWMERROR("connect sql name or db is NULL\n");
        return wmFALSE;
    }
    mysql_real_connect(&Mysql,pHostName,pUserName,pPassword,pDataBase,usDbPort,wmNULL,wmFalse);
    if(Mysql.net.last_errno != wmFalse)
    {
        DWMERROR("mysql_real_connect filed");
        return wmFALSE;
    }
    
    return wmTRUE;  
}

wmBOOL CMysqlDB::SQLExec(wmCCHAR* pSQLCmd)
{
    if(SQLClose() == wmFALSE)
    {
        DWMERROR("SQLClose failed\n");
        return wmFALSE;
    }
    
    if(wmNULL == pSQLCmd)
    {
        DWMERROR("sql cmd is NULL\n");
        return wmFALSE;
    }

    mysql_query(&Mysql,pSQLCmd);
    if(Mysql.net.last_errno != wmFalse)
    {
        DWMERROR("mysql_query filed");
        return wmFALSE;
    }
    
    pResult = mysql_store_result(&Mysql);
    if(Mysql.net.last_errno != wmFalse)
    {
        DWMERROR("mysql_store_result filed");
        return wmFALSE;
    }
    return wmTRUE;  
}

DwmMYSQLROW CMysqlDB::FetchRows()
{
    DwmMYSQLROW tmpRow;
    tmpRow=mysql_fetch_row(pResult);
    if(Mysql.net.last_errno!=0)
    {
        DWMERROR("mysql_fetch_row filed");
        return wmNULL;
    }
    return tmpRow;
}

wmULONG CMysqlDB::RecordCount()
{
    if(wmNULL == pResult)
    {
        DWMERROR("pResult is null");
        return wmFalse;
    }
    
    wmULONG ulCount = (wmULONG)mysql_num_rows(pResult);
    if(Mysql.net.last_errno != wmFalse)
    {
        DWMERROR("mysql_num_rows filed");
        ulCount = wmFalse;
    }
    
    return ulCount;
}

wmBOOL CMysqlDB::CheckConnect()
{
    return (mysql_ping(&Mysql) == wmFalse);
}

bool CMysqlDB::SQLClose()
{
    if (pResult)
    {
        mysql_free_result(pResult); 
        if(Mysql.net.last_errno!=0)
        {
            DWMERROR("mysql_free_result filed");
            return wmFALSE;
        }
        pResult=wmNULL;
    }
    return wmTRUE;  
}


/**
 * @file MysqlDB.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __MYSQLDB_H__
#define __MYSQLDB_H__

#include "baseDef.h"
#include "baseTable.h"
#include "mysql/mysql.h"


class CMysqlDB: public TInstance<CMysqlDB>
{
    friend class TInstance<CMysqlDB>;
public:
    wmVOID MysqlInit();
    wmBOOL Connect(wmCCHAR* pHostName,wmUSHORT usDbPort,wmCCHAR* pUserName,wmCCHAR* pPassword,wmCCHAR* pDataBase);
    wmBOOL SQLExec(wmCCHAR* pSQLCmd);
    wmULONG RecordCount();
    wmBOOL CheckConnect();
    DwmMYSQLROW FetchRows();

private:
    DwmMYSQL Mysql;    
    DwmMYSQLRES *pResult;
    
    wmBOOL SQLClose();
};

#endif
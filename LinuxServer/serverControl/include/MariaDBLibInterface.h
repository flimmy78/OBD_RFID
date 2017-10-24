#ifndef DEFINE_MYSQL_API_H_
#define DEFINE_MYSQL_API_H_

#ifdef _P_WINDOWS_
#define DEFINE_API __declspec(dllexport)
#else
#define DEFINE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif 

DEFINE_API bool MariaDBLibInitialze();
DEFINE_API bool ConnectToMariaDB(const char* pHostName,unsigned short usDbPort,const char* pUserName,const char* pPassword,const char* pDataBase);
DEFINE_API bool InsertValueIntoTable(char const *pImputJsString);
DEFINE_API bool DeleteValueFromTable(unsigned long ulGoodsID);
DEFINE_API bool SelectItemsFromTable(char const *pImputJsString, char *pOutJsString);
DEFINE_API bool GetDeviceInfo(char const * pFrom, char const * pFromId, char const * pTarget, char* pValue);
DEFINE_API bool MariaDBLibExit();

#ifdef __cplusplus
} 
#endif  

#endif

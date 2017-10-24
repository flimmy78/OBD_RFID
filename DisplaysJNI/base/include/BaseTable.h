/**
 * @file BaseTable.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */

#ifndef __BASE_TABLE_H__
#define __BASE_TABLE_H__
#include <stdlib.h>
#include <typeinfo>
#include <stdio.h>

#define MEM_INFO_NAME_LEN   128

template <typename T> class TInstance
{
public:
    static T * GetInstance(){
        if (NULL == pInstance)
        {
            pInstance = new T;
        }
        return pInstance;
    }

    static void ReleaseInstance(){
        if(pInstance){
            delete pInstance;
            pInstance = NULL;
        }
    }
protected:
    virtual ~TInstance(){pInstance = NULL;}
    static T* pInstance;
private:
    T& operator=(const T &){
    }
};

template <typename T> T* TInstance<T>::pInstance = NULL;

#endif  /* __BASE_TABLE_H__ */

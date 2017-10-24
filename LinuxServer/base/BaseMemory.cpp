/**
 * @file BaseMemory.cpp
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */
#include "BaseMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>

void *DwmMallocDebug(wmULONG ulSize , wmCCHAR* pFileName, wmINT iLine, wmCCHAR* pFunction)
{
    void *ptr = wmNULL;
    ptr = malloc(ulSize);
    if(ptr && pFileName && pFunction)
        printf("[memory][malloc][%s:%d][%s] ptr = [%p]\n",pFileName,iLine,pFunction,ptr);
    return ptr;
}
 
wmVOID *DwmRealloc(wmVOID *ptr, wmUINT uiSize)
{
    if(wmNULL == ptr)
        return wmNULL;
    return realloc(ptr, uiSize);
}
 
wmVOID DwmFreeDebug(wmVOID *ptr , wmCCHAR* pFileName, wmINT iLine, wmCCHAR* pFunction)
{
    if(wmNULL == ptr)
        return;
    if(pFileName && pFunction)
        printf("[memory][free][%s:%d][%s] ptr = [%p]\n",pFileName,iLine,pFunction,ptr);
    free(ptr);
    ptr = NULL;
}
 
wmVOID DwmMemset(wmVOID * ptr, wmINT value, wmULONG ulSize)
{
    if(wmNULL == ptr)
        return;
    memset(ptr, value, ulSize);
}
 
wmVOID DwmMemcpy(wmVOID *desptr, wmVOID const *originptr, wmULONG offset, wmULONG length)
{
    if(wmNULL == desptr)
        return;
    if(wmNULL == originptr)
        return;
    memcpy(desptr, reinterpret_cast<char const *>(originptr) + offset, length);
}

wmVOID DwmAllocopy(wmCHAR* &to, const wmCHAR * from)
{
    if(strlen(from)<=0)
        return;
    to = (wmCHAR*)malloc(strlen(from)+1);
    if(wmNULL == to)
        return;
    *(to+strlen(from)) = 0;
    ::DwmMemcpy(to, from, 0, strlen(from));
}
 


/**
 * @file BaseMemory.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_MEMORY_H_
#define _BASE_MEMORY_H_
#include "BaseDefine.h"

#ifdef  _WINDOWS_OS_
#define DwmMalloc(size)   DwmMallocDebug(size, wmNULL, wmFalse, wmNULL)
#define DwmFree(ptr)   DwmFreeDebug(ptr,wmNULL, wmFalse, wmNULL)
#else 
#define DwmMalloc(size)   DwmMallocDebug(size,__FILE__, __LINE__, __FUNCTION__)
#define DwmFree(ptr)   DwmFreeDebug(ptr,__FILE__, __LINE__, __FUNCTION__)
#endif


wmVOID *DwmMallocDebug(wmULONG size , wmCCHAR* pFileName, wmINT iLine, wmCCHAR* pFunction);
wmVOID *DwmRealloc(wmVOID *ptr, wmUINT size);
wmVOID DwmFreeDebug(wmVOID *ptr, wmCCHAR* pFileName, wmINT iLine, wmCCHAR* pFunction);
wmVOID DwmMemset(wmVOID * ptr, wmINT value, wmULONG size);
wmVOID DwmMemcpy(wmVOID *desptr, wmVOID const *originptr, wmULONG offset, wmULONG length);
wmVOID DwmAllocopy(wmCHAR* &to, const wmCHAR * from);

#endif //end _BASE_MEMORY_H_


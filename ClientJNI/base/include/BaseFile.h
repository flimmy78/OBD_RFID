/**
 * @file BaseFile.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_FILE_H_
#define _BASE_FILE_H_
#include "BaseDefine.h"

#ifdef _WINDOWS_OS_

wmBOOL WMFileExist(const wmCHAR *pFileName);
wmQWORD WMFileSize(const wmCHAR *pFileName);
wmVOID* wmOpen(const wmCHAR *pFileName, const wmCHAR *pFlags);
wmVOID wmClose(wmVOID* pFile);
wmVOID wmFSeek(wmVOID* pFile, wmINT offset, wmINT whence);
wmQWORD wmTell(wmVOID* pFile);
wmVOID wmFFlush(wmVOID* pFile);
wmULONG wmRead(wmVOID* pFile, wmVOID *buf, wmUINT length);
wmULONG wmWrite(wmVOID* pFile, const wmVOID *buf, wmUINT length);
wmBOOL wmDelete(const wmCHAR *pFileName);

#else
wmBOOL WMFileExist(const wmCHAR *pathname);
wmQWORD WMFileSize(const wmCHAR *pathname);
wmQWORD wmSize(wmINT fd);
wmINT wmOpen(const wmCHAR *pathname, wmINT flags);
wmINT wmClose(wmINT fd);
wmINT wmLseek(wmINT fd, wmINT offset, wmINT whence);
wmINT wmTell(wmINT fd);
wmBOOL wmEOF(wmINT fd);
wmINT wmRead(wmINT fd, wmVOID *buf, wmUINT length);
wmINT wmWrite(wmINT fd, const wmVOID *buf, wmUINT length);
wmINT wmRename(const wmCHAR *oldname, const wmCHAR *newname);
wmINT wmDelete(const wmCHAR *pathname);
wmINT wmCreateDir(const wmCHAR *pathname);
wmINT wmDeleteDir(const wmCHAR *pathname);
#endif

#endif //end _BASE_FILE_H_


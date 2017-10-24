/**
 * @file BaseFile.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#include "BaseFile.h"
#include "BaseSize.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#ifdef _WINDOWS_OS_
wmBOOL WMFileExist(const wmCHAR *pFileName)
{
    FILE *pFile = ::fopen(pFileName, "rb");
    if (pFile) {
        ::fclose(pFile);
        return wmTRUE;
    }
    return wmFALSE;
}

wmQWORD WMFileSize(const wmCHAR *pFileName)
{
    FILE *pFile = ::fopen(pFileName, "rb");
    if (pFile) {
        ::fseek(pFile, 0, SEEK_END);
        wmQWORD qwFileSize = ::ftell(pFile);
        ::fclose(pFile);
        return qwFileSize;
    }
    return wmFalse;
}

wmVOID* wmOpen(const wmCHAR *pFileName, const wmCHAR *pFlags)
{
    FILE *pFile = ::fopen(pFileName, pFlags);
    return pFile;
}

wmVOID wmClose(wmVOID* pFile)
{
    if(pFile)
        ::fclose((FILE*)pFile);   
}

wmVOID wmFSeek(wmVOID* pFile, wmINT offset, wmINT whence)
{
    if(pFile)
    {
        ::fseek((FILE*)pFile, offset, whence);
    }
}

wmQWORD wmTell(wmVOID* pFile)
{
    if(pFile)
        return ::ftell((FILE*)pFile);
    else
        return wmFalse;
}

wmVOID wmFFlush(wmVOID* pFile)
{
    if(pFile)
        ::fflush((FILE*)pFile);
}

wmULONG wmRead(wmVOID* pFile, wmVOID *buf, wmUINT length)
{
    wmULONG ulReadSize = wmFalse;
    if(pFile)
    {
        ulReadSize = ::fread(buf, 1, length, (FILE*)pFile);
    }
    return ulReadSize;
}

wmULONG wmWrite(wmVOID* pFile, const wmVOID *buf, wmUINT length)
{
    wmULONG ulReadSize = wmFalse;
    if(pFile)
    {
        ulReadSize = ::fwrite(buf, 1, length, (FILE*)pFile);
    }
    return ulReadSize;
}

wmBOOL wmDelete(const wmCHAR *pFileName)
{
    return (::remove(pFileName) == 0);
}


#else
#include <sys/param.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>




wmBOOL WMFileExist(const wmCHAR *pathname)
{
    struct stat state = {0};

    if (pathname == wmNULL) return wmFALSE;

    if (lstat(pathname, &state)) return wmFALSE;
    return wmTRUE;
}

wmQWORD WMFileSize(const wmCHAR *pathname)
{
    if (WMFileExist(pathname) == wmFALSE) return 0;
    int fd = open(pathname, O_RDONLY);
    if (fd == -1) return 0;
    wmLONG t = lseek(fd, 0, SEEK_END);
    close(fd);
    if (t == -1) return 0;
    return (wmQWORD)t;
}

wmQWORD wmSize(wmINT fd)
{
    if (fd == -1) return 0;
    wmLONG t_curr = lseek(fd, 0, SEEK_CUR);
    wmLONG t_size = lseek(fd, 0, SEEK_END);
    lseek(fd, t_curr, SEEK_SET);
    if (t_curr == -1 || t_size == -1) return 0;
    return t_size;
}

wmINT wmOpen(const wmCHAR *pathname, wmINT flags)
{
    if (pathname == wmNULL) return -1;

    if (WMFileExist(pathname) == wmFALSE)
    {
        creat(pathname, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }

    return open(pathname, flags);
}

wmINT wmClose(wmINT fd)
{
    if (close(fd)) return wmFalse;

    return wmTrue;
}

wmINT wmLseek(wmINT fd, wmINT offset, wmINT whence)
{
    return lseek(fd, offset, whence);
}

wmINT wmTell(wmINT fd)
{
    return lseek(fd, 0, SEEK_CUR);
}

wmBOOL wmEOF(wmINT fd)
{
    wmINT currentPosition = 0;
    currentPosition = lseek(fd, 0, SEEK_CUR);

    if (currentPosition >= lseek(fd, 0, SEEK_END))
    {
        lseek(fd, currentPosition, SEEK_SET);
        return wmTRUE;
    }

    lseek(fd, currentPosition, SEEK_SET);
    return wmFALSE;
}

wmINT wmRead(wmINT fd, wmVOID *buf, wmUINT length)
{
    if (buf == wmNULL) return -1;
    return ::read(fd, buf, length);
}

wmINT wmWrite(wmINT fd, const wmVOID *buf, wmUINT length)
{
    if ((buf == wmNULL) || (length >= MAX_FILE_SIZE)) return -1;
    return ::write(fd, buf, length);
}

wmINT wmRename(const wmCHAR *oldname, const wmCHAR *newname)
{
    if ((oldname == wmNULL) || (newname ==wmNULL)) return wmFalse;

    if (rename(oldname, newname))
    {
        printf("Rename failed for %s\n", strerror(errno));
        return wmFalse;
    }
    return wmTrue;
}

wmINT wmDelete(const wmCHAR *pathname)
{
    if (pathname == wmNULL) return wmFalse;

    if (access(pathname, R_OK) == 0)
    {
        //if (CanDelete(pathname) == wmFALSE) return wmFalse;

        if (remove(pathname))
        {
            printf("Delete %s failed for %s\n", pathname, strerror(errno));
            return wmFalse;
        }
        return wmTrue;
    }
    return wmFalse;
}

wmINT wmCreateDir(const wmCHAR *pathname)
{
    const wmINT LENOFNAME = 128;
    wmCHAR dirname[LENOFNAME] = {0};
    wmINT dirlen = 0;

    if ((pathname == wmNULL) || strlen(pathname) >= LENOFNAME-1) return wmFalse;
    if (WMFileExist(pathname)) return wmTrue;

    strncpy(dirname, pathname, LENOFNAME-2);
    dirlen = strlen(pathname);
    if (dirname[dirlen-1] != '/')
    {
        dirname[dirlen] = '/';
        dirname[dirlen+1] = '\0';
        dirlen += 1;
    }

    for (wmINT i=1; i<dirlen; i++)
    {
        if (dirname[i] == '/')
        {
            dirname[i] = '\0';
            if (!WMFileExist(dirname))
            {
                if (mkdir(dirname, S_IRWXU | S_IRWXG))
                {
                    printf("CreateDir failed for %s\n", strerror(errno));
                    return wmFalse;
                }
            }
            dirname[i] = '/';
        }
    }

    return wmTrue;
}

wmINT wmDeleteDir(const wmCHAR *pathname)
{
    struct stat state = {0};
    DIR *dir = wmNULL;
    struct dirent *direntry = wmNULL;
    const wmINT LENOFNAME = 256;

    if ((pathname == wmNULL) || strlen(pathname) >= (wmINT)LENOFNAME) return wmFalse;

    if (lstat(pathname, &state)) 
    {
        printf("DeleteDir failed for %s\n", strerror(errno));
        return wmFalse;
    }

    if (!S_ISDIR(state.st_mode)) return wmDelete(pathname); // file

    if (access(pathname, R_OK) == 0)
    {
        //if (CanDelete(pathname) == wmFALSE) return wmFalse;

        dir = opendir(pathname); // directory
        if (dir == wmNULL) return wmFalse;
    }
    else
    {
        return wmFalse;
    }

    while ((direntry = readdir(dir)) != wmNULL)
    {
        wmCHAR dirname[LENOFNAME] = {0};
        if (!strcmp(direntry->d_name, "..") || !strcmp(direntry->d_name, "."))
            continue;
        snprintf(dirname, LENOFNAME-1,  "%s/%s", pathname, direntry->d_name);
        if (wmDeleteDir(dirname) != wmTrue) break;
    }

    if (closedir(dir)) return wmFalse;

    if (rmdir(pathname)) return wmFalse;

    return wmTrue;
}

#endif

/**
 * @file baseDefine.h
 * @brief all DEF
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */

#ifndef _BASE_DEFINE_H_
#define _BASE_DEFINE_H_
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>
#include <stdio.h>
#include <vector>
#include <map>
#include <list>


#define wmVECTOR    std::vector
#define wmSTRSTRING    std::string
#define wmMAP          std::map
#define wmLIST         std::list
#define wmMAPPAIR          std::make_pair
#define wmDEQUE      std::deque
#define wmUNIQUEPTR   std::unique_ptr
#define wmSHAREDPTR   std::shared_ptr

#define wmINT         int
#define wmLONG        long
#define wmCHAR        char
#define wmCCHAR       const char
#define wmBOOL        bool
#define wmVOID        void
#define wmNULL        NULL
#define wmNULLCCSTR        ""
#define wmFalseStr          "false"
#define wmTrueStr          "true"
#define wmNull          "null"
#define wmTRUE        true
#define wmFALSE      false 
#define wmTrue        1
#define wmFalse      0 
#define wmULONG     unsigned long
#define wmQWORD     unsigned long long
#define wmUINT        unsigned int
#define wmUSHORT   unsigned short
#define wmSHORT     short
#define wmFLOAT     float
#define wmDOUBLE   double
#define wmSIZET      size_t
#define wmU8T         unsigned char
#define wmU16T       wmUSHORT
#define wmU32T       wmUINT
#define wmV4l2Buffer v4l2_buffer

#define wmTHREAD   Thread
#define wmGUID       GUID
#define wmSUCCEED SUCCEEDED
#define wmHRESULT  HRESULT
#define wmEXTERNC  EXTERN_C
#define wmSOK          S_OK
#define wmEFAIL       E_FAIL
#define wmEPOINTER  E_POINTER
#define wmEINVALIDARG  E_INVALIDARG
#define wmSFALSE  S_FALSE
#define wmVFWENOTFOUND  VFW_E_NOT_FOUND

#define _P_LINUX_ 1

#ifdef _WINDOWS_OS_

#else
typedef int SOCKET;
#endif

#define DwmFILEEND                                          SEEK_END
#define DwmFILECUR                                          SEEK_CUR
#define DwmFILESET                                          SEEK_SET
#define DwmFILE                                                FILE
#define DwmFopen(filename, mode)                  fopen(filename, mode)
#define DwmFeof                                                feof
#define DwmFclose(file)                                     fclose(file)
#define DwmFprintf(file, format, ...)                   fprintf(file, format, ##__VA_ARGS__)
#define DwmFgetc(file)                                      fgetc(file)
#define DwmFgets                                              fgets
#define DwmFread(buffer, size, count, file)      fread(buffer, size, count, file)
#define DwmFwrite(buffer, size, count, file)     fwrite(buffer, size, count, file)
#define DwmFseek(file,offset,start)                  fseek(file,offset,start)
#define DwmFflush(file)                                     fflush(file)
#define DwmFremove(path)                              remove(path)
#define DwmIOCTL                                             ioctl
#define DwmREAD                                              read
#define DwmMMAP                                              mmap
#define wmZStream                                           z_stream
#define wmFDSET                                              fd_set
#define wmTIMEVAL                                          timeval
#define wmSOCKADDR                                       sockaddr
#define wmFDZERO                                            FD_ZERO
#define wmFD_SET                                            FD_SET
#define wmFD_ISSET                                         FD_ISSET

#define wmDELETE                                            delete
#define wmNEW                                                 new
#define DWMAssert                                             assert



#define PTRVECT_LOCK_NAME                          "LOCK_OF_PTRVECTOR"
#define TCP_SERVER_LOCK_NAME                          "LOCK_OF_TCP_SERVER"
#define TIMER_LOCK_NAME                          "LOCK_OF_TIMER"
#define DEQUE_LOCK_NAME                          "LOCK_OF_DEQUE"
#define CURRENT_STARTUS_LOCK_NAME                          "LOCK_OF_CURRENT_STARTUS"

#define DFJNIBool   JNIEXPORT wmBOOL JNICALL
#define DFJNIVoid   JNIEXPORT wmVOID JNICALL
#define DFJNIInt   JNIEXPORT wmINT JNICALL
#define DFJNIEc   JNIEnv *ev, jclass

#define ErrorCodeALL          -1
#define ErrorCodeNoError          1


#define pstrType  "type"
#define pstrStatus "status"
#define pstrKeyEventPathBackup  "/dev/input/event0"
#define pstrKeyEventPathBackup1  "/dev/input/event1"
#define pstrKeyEventPathBackup2  "/dev/input/event2"
#define pstrKeyEventPathBackup3  "/dev/input/event3"
#define pstrKeyEventPathBackup4  "/dev/input/event4"
#define pstrKeyEventPathBackup5  "/dev/input/event5"
#define pstrKeyEventPathBackup6  "/dev/input/event6"
#define pstrKeyEventPathBackup7  "/dev/input/event7"
#define pstrKeyEventPathBackup8  "/dev/uinput"

#define pstrDeviceID  "deviceid"
#define pstrLedDevice  "leddevice"
#define pstrRfidDevice  "rfiddevice"

#define pstrHostName  "127.0.0.1"
#define usDatabasePort   3306
#define pstrUserName  "root"
#define pstrPassword  ""
#define pstrDataBase  "dwm"

#define usBaseTcpServerPort 15566 
#define usRfidTcpServerPort 6000 



#endif //end _BASE_DEFINE_H_

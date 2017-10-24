/**
 * @file BaseDeque.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __BASE_DEQUE_H__
#define __BASE_DEQUE_H__

#include "BaseDefine.h"
#include "BaseLock.h"
#include "BaseType.h"
#include <deque>

class BufferDeque
{
public:
    BufferDeque();
    ~BufferDeque();

    bool addBufferDeque(SDataBuff *pSSqlItem);
    SDataBuff* DeleteBufferDeque();

    int getcount() const;
    bool isempty() const;
    void flush();
private:
    typedef std::deque<SDataBuff *> DataBufferDeque;

    DataBufferDeque dBufferDeque;
    wmLock mutexLock;
};

#endif // __BASE_DEQUE_H__
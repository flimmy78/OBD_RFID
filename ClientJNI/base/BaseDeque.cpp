#include "BaseDeque.h"
#include "BaseMemory.h"
#include "BaseDebug.h"

BufferDeque::BufferDeque()
{
    mutexLock = WMCreateLock(DEQUE_LOCK_NAME);
}

BufferDeque::~BufferDeque()
{
    WMDestroyLock(mutexLock);
}

bool BufferDeque::addBufferDeque(SDataBuff *pSSqlItem)
{
    WMLock(mutexLock);
    dBufferDeque.push_back(pSSqlItem);
    WMUnLock(mutexLock);
    return true;
}

SDataBuff *BufferDeque::DeleteBufferDeque()
{
    WMLock(mutexLock);
    if (!dBufferDeque.empty()) {
        
        SDataBuff *pSSqlItem = dBufferDeque.front();
        dBufferDeque.pop_front();
        WMUnLock(mutexLock);
        return pSSqlItem;
    }
    WMUnLock(mutexLock);
    return NULL;
}

int BufferDeque::getcount() const
{
    WMLock(mutexLock);
    int iSize = dBufferDeque.size();
    WMUnLock(mutexLock);
    return iSize;
}

bool BufferDeque::isempty() const
{
    WMLock(mutexLock);
    bool bL = dBufferDeque.empty();
    WMUnLock(mutexLock);
    return bL;
}

void BufferDeque::flush()
{
    WMLock(mutexLock);

    while (!dBufferDeque.empty()) {
        SDataBuff* pSDataBuff = dBufferDeque.front();
        if(pSDataBuff)
        {
            if(pSDataBuff->pBuff)
                DwmFree(pSDataBuff->pBuff);
            delete dBufferDeque.front();
        }
        dBufferDeque.pop_front();
    }
    WMUnLock(mutexLock);
}

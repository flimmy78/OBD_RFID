/**
 * @file BaseThread.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 2017
 * 
 */
#ifndef _H_BASE_THREAD_H_
#define _H_BASE_THREAD_H_

class Thread
{
public:
    typedef int (* ThreadFunc)(void *param);

    explicit Thread(ThreadFunc func);

    bool start(void *param);
    int  join();

private:
    Thread(Thread const&);
    Thread& operator=(Thread const&);

    ThreadFunc threadFunc;
    void *pData;
};

#endif // _H_BASE_THREAD_H_
#include <thread>
#include <vector>
#include "Interfaces/Queue.h"
#include "Prv/TP/StableThreadPool.h"
#include "Prv/TP/ThreadPoolImplBase.h"

namespace Threading
{

struct __I
{
    using TaskExcFunc = typename ThreadPoolImplBase<Threading::Queue<Runnable*> >::TaskExc;
    __I(unsigned int threadCount,  TaskExcFunc runFunc, TaskExcFunc stopFunc, TaskExcFunc doneFunc) :
        thepool(threadCount, runFunc, stopFunc)
    {
    }
    ThreadPoolImplBase<Threading::Queue<Runnable*> >*  operator->()
    {
        return &thepool;
    }

    ThreadPoolImplBase<Threading::Queue<Runnable*> > thepool;
};

StableThreadPool::StableThreadPool(unsigned int threadCount) :
    _pI(new __I{threadCount, &Threading::run, &Threading::stop, &Threading::done})
{

    for(unsigned int i = 0; i < (*_pI)->maxThreadCount(); ++i)
    {
        (*_pI)->tryLaunchNewThread();
    }
}

StableThreadPool::~StableThreadPool()
{
    shutdown();
    delete _pI;
}

void StableThreadPool::run(Runnable *pRuner, unsigned int /*priority*/)
{
    (*_pI)->run(pRuner);
}

void StableThreadPool::setMaxThreadCount(unsigned int /*nThreadCount*/)
{

}

unsigned int StableThreadPool::activeThreadCount()
{
    return (*_pI)->activeThreadCount();
}

void StableThreadPool::shutdown()
{
    (*_pI)->shutdown();
}

}
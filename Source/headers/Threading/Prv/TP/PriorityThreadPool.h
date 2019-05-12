#ifndef PRIORITYTHREADPOOL_H
#define PRIORITYTHREADPOOL_H

#include "Interfaces/IThreadPool.h"
#include "Interfaces/Queue.h"
#include "Prv/TP/ThreadPoolImplBase.h"

namespace Threading
{

class PriorityThreadPool : public IThreadPool
{
public:
    PriorityThreadPool(unsigned int threadCount = 0);
    ~PriorityThreadPool() override;
    virtual void run(Runnable* pRuner, unsigned int priority = 0) override;
    virtual void setMaxThreadCount(unsigned int /*nThreadCount*/) override {}
    virtual unsigned int activeThreadCount() override;
    virtual void shutdown() override;

private:
    struct TheImpl* _pImpl;
};

}
#endif // PRIORITYTHREADPOOL_H
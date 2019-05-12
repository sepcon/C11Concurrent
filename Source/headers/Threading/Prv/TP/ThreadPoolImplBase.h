#ifndef THREADPOOLIMPLBASE_H
#define THREADPOOLIMPLBASE_H

#include "Interfaces/IThreadPool.h"
#include "Interfaces/ThreadJoiner.h"
#include "Interfaces/Runnable.h"
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <atomic>
#include <functional>
#include <iostream>

namespace Threading {

/*! \brief The base class used for implement the variants of threadpool
 * class ThreadPoolImplBase
 *
 */

template< class TaskQueue>
class ThreadPoolImplBase
{
	
public:
    typedef typename TaskQueue::value_type Task;
    typedef typename TaskQueue::reference TaskRef;
    typedef typename TaskQueue::const_reference TaskCRef;
    typedef std::function<void (TaskRef)> TaskExc;
	static inline void fDoNothing(TaskRef) {}
    ThreadPoolImplBase(size_t maxCount, TaskExc fRun, TaskExc fStop = &fDoNothing, TaskExc fDone = &fDoNothing):
        _fRun(fRun),
        _fStop(fStop),
        _fDone(fDone)
    {
        if(maxCount == 0)
        {
            _maxThreadCount = std::thread::hardware_concurrency();
        }
        else
        {
            _maxThreadCount = maxCount;
        }
    }

    ~ThreadPoolImplBase()
    {
        shutdown();
    }

    void tryLaunchNewThread()
    {
        try
        {
            _pool.emplace_back(std::thread{&ThreadPoolImplBase::coptRunPendingTask, this});
        }
        catch(const std::system_error& err)
        {
            std::cout << "Cannot launch new thread due to: " << err.what() << std::endl;
        }
    }

    void run(Task task)
    {
        _taskQueue.push(task);
    }

    unsigned int maxThreadCount() const
    {
        return _maxThreadCount;
    }

    unsigned int activeThreadCount()
    {
        return static_cast<unsigned int>(_pool.size());
    }

    void setMaxThreadCount(unsigned int nThreadCount)
    {
        if(nThreadCount == 0)
        {
            _maxThreadCount = std::thread::hardware_concurrency();
        }
        else
        {
            _maxThreadCount = nThreadCount;
        }
    }

    void shutdown()
    {
        std::call_once(_shutdowned, &ThreadPoolImplBase::stopThePool, this);
    }

    void stopThePool()
    {
        _taskQueue.close();
        stopRunningTasks();
        _taskQueue.clear(_fDone);
        waitForThreadsExited();
    }

private:
    //copt = Called On Pool Threads
    void coptRun(Task task)
    {
        addToRunningTasks(task);
        _fRun(task);
        removeFromRunningTasks(task);
        _fDone(task);
    }
    void coptRunPendingTask()
    {
        Task task;
        while(_taskQueue.wait(task))
        {
            coptRun(task);
        }
    }

    std::vector<std::thread> _pool;
    std::once_flag _shutdowned;
    TaskQueue _taskQueue;
    std::list<Task> _runningTasks;
    std::mutex _runningTaskMutex;
    unsigned int _maxThreadCount;
    TaskExc _fRun;
    TaskExc _fStop;
    TaskExc _fDone;


    void addToRunningTasks(Task task)
    {
        std::lock_guard<std::mutex> lock(_runningTaskMutex);
        _runningTasks.push_back(task);
    }
    void removeFromRunningTasks(Task task)
    {
        std::lock_guard<std::mutex> lock(_runningTaskMutex);
        for(auto iTask = _runningTasks.begin(); iTask != _runningTasks.end(); ++iTask)
        {
            if(*iTask == task)
            {
                _runningTasks.erase(iTask);
                break;
            }
        }
    }
    void stopRunningTasks()
    {
        std::lock_guard<std::mutex> lock(_runningTaskMutex);
        for(auto& task : _runningTasks)
        {
            _fStop(task);
        }
    }

    void waitForThreadsExited()
    {
        for(auto& th : _pool)
        {
            if(th.joinable())
            {
                th.join();
            }
        }
    }
};
}

#endif // THREADPOOLIMPLBASE_H
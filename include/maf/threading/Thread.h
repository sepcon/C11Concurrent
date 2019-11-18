#ifndef THREAD_H
#define THREAD_H

#include <maf/patterns/Patterns.h>
#include <maf/utils/cppextension/Invoker.h>
#include <thread>

namespace maf {
namespace threading {

class Thread : public pattern::UnCopyable
{
public:
    using OnSignalCallback = std::function<void(int)>;
    Thread() = default;
    Thread(Thread&& th);
    Thread& operator=(Thread&& th);
    template<typename Callable, typename... Args>
    Thread(Callable&& f, Args&&... args)
    {
        auto invoker = util::makeInvoker(std::forward<Callable>(f), std::forward<Args>(args)...);
        _callable = [invoker = std::move(invoker), sigHandler = std::move(_sigHandlerCallback)] () mutable {
            _tlSigHandlerCallback = std::move(sigHandler);
            regSignals();
            invoker.invoke();
        };
    }

    Thread &start();
    void join();
    void detach();
    bool joinable();
    void setSignalHandler(OnSignalCallback sigHandlerCallback);
protected:
    static void regSignals();
    static void onSystemSignal(int sig);
    void takeFrom(Thread&& th);

    static thread_local OnSignalCallback _tlSigHandlerCallback;
    std::thread _thread;
    std::function<void()> _callable;
    OnSignalCallback _sigHandlerCallback;
};
}
}
#endif // THREAD_H

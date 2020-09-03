#include "GlobalThreadPool.h"

#include <maf/logging/Logger.h>
#include <maf/messaging/Component.h>
#include <maf/threading/Lockable.h>

#include <thread>
#include <vector>

namespace maf {
namespace messaging {
namespace global_threadpool {

struct ThreadInterupt {};

struct ThePool {
  ThePool() {}
  ~ThePool() {}

  void init() { comp_ = Component::create(); }

  void deinit() { comp_->stop(); }

  auto threadCount() { return threadCount_; }

  bool tryAddThread() {
    try {
      std::thread{[this] {
        try {
          comp_->run();
        } catch (const ThreadInterupt&) {
          MAF_LOGGER_INFO("Thread id ", std::this_thread::get_id(), " stopped");
          --threadCount_;
        }
      }}.detach();
      ++threadCount_;
      return true;
    } catch (const std::exception& e) {
      MAF_LOGGER_ERROR("Failed to add new thread to global thread pool: ",
                       e.what());
    }
    return false;
  }

  bool tryRemoveThread() {
    return comp_->execute([] { throw ThreadInterupt{}; });
  }

  ComponentInstance comp_;
  size_t threadCount_ = 0;
};
static ThePool& thepool() {
  static ThePool _;
  return _;
}

bool submit(TaskType task) { return thepool().comp_->execute(std::move(task)); }

bool tryAddThread() { return thepool().tryAddThread(); }

void init() { thepool().init(); }

void deinit() { thepool().deinit(); }

bool tryRemoveThread() { return thepool().tryRemoveThread(); }

size_t threadCount() { return thepool().threadCount(); }

}  // namespace global_threadpool
}  // namespace messaging
}  // namespace maf

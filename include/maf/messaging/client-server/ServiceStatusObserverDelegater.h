#pragma once

#include <maf/utils/ExecutorIF.h>

#include "Exceptions.h"
#include "ServiceStatusObserverIF.h"

namespace maf {
namespace messaging {

class ServiceStatusObserverDelegater : public ServiceStatusObserverIF {
 public:
  using DelegateCallback = std::function<void(Availability, Availability)>;

  ServiceStatusObserverDelegater(std::weak_ptr<util::ExecutorIF> wexecutor,
                                 DelegateCallback callback)
      : callback_(std::move(callback)), wexecutor_(std::move(wexecutor)) {}

  void onServiceStatusChanged(const ServiceID &, Availability oldStatus,
                              Availability newStatus) override {
    auto executor = wexecutor_.lock();
    if (!executor ||
        !executor->execute(std::bind(callback_, oldStatus, newStatus))) {
      throw UnavailableException{};
    }
  }

 private:
  DelegateCallback callback_;
  std::weak_ptr<util::ExecutorIF> wexecutor_;
};

}  // namespace messaging
}  // namespace maf

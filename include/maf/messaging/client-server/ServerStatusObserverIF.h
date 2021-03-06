#pragma once

#include "CSStatus.h"
#include "CSTypes.h"

namespace maf {
namespace messaging {

class ServerStatusObserverIF {
public:
  virtual void onServerStatusChanged(Availability oldStatus,
                                     Availability newStatus) noexcept = 0;
};

} // namespace messaging
} // namespace maf

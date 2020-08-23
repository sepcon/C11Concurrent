
#pragma once
#include <maf/messaging/client-server/CSStatus.h>
#include <maf/utils/Process.h>

#include <chrono>
#include <functional>

#include "SMShared.h"

namespace maf {
namespace messaging {
namespace ipc {
namespace local {
using namespace std::chrono;

class LocalIPCReceiverStatusMonitor {
 public:
  inline void init(const Address& receiverAddr);
  inline Availability getCurrentStatus();
  inline ~LocalIPCReceiverStatusMonitor();

 private:
  SMData readSMData() const;
  SMPath receiverSMPath_;
  SMHandle smhandle_ = InvalidSMHandle;
  SMBuffer buff = nullptr;
  SMData smdata_ = {0};
  Availability currentStatus_ = Availability::Unavailable;
};

void LocalIPCReceiverStatusMonitor::init(const Address& receiverAddr) {
  receiverSMPath_ = constructSMPath(receiverAddr);
}

Availability LocalIPCReceiverStatusMonitor::getCurrentStatus() {
  do {
    if (smhandle_ == InvalidSMHandle) {
      smhandle_ =
          OpenFileMappingA(FILE_MAP_ALL_ACCESS,       // read/write access
                           FALSE,                     // do not inherit the name
                           receiverSMPath_.c_str());  // name of mapping object

      if (smhandle_ == InvalidSMHandle) {
        currentStatus_ = Availability::Unavailable;
        break;
      }

      buff = static_cast<SMBuffer>(
          MapViewOfFile(smhandle_, FILE_MAP_READ, 0, 0, sizeof(SMData)));

      if (!buff) {
        CloseHandle(smhandle_);
        smhandle_ = InvalidSMHandle;
        currentStatus_ = Availability::Unavailable;
        break;
      }
    }

    auto newSMData = readSMData();
    if (newSMData == 0) {
      currentStatus_ = Availability::Unavailable;
    } else {
      if (currentStatus_ == Availability::Unavailable) {
        currentStatus_ = Availability::Available;
      } else if (newSMData != smdata_) {
        currentStatus_ = Availability::Unavailable;
      } else {
        currentStatus_ = Availability::Available;
      }
    }
    smdata_ = newSMData;
  } while (false);
  return currentStatus_;
}

LocalIPCReceiverStatusMonitor::~LocalIPCReceiverStatusMonitor() {
  if (buff) {
    UnmapViewOfFile(buff);
  }
  if (smhandle_) {
    CloseHandle(smhandle_);
  }
}

SMData LocalIPCReceiverStatusMonitor::readSMData() const {
  return *reinterpret_cast<SMData*>(buff);
}

}  // namespace local
}  // namespace ipc
}  // namespace messaging
}  // namespace maf

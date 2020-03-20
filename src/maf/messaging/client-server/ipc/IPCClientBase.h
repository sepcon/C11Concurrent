#pragma once

#include "../ClientBase.h"
#include "BytesCommunicator.h"
#include "IPCTypes.h"
#include <thread>

namespace maf {
namespace messaging {

class SyncTimer;

namespace ipc {

class IPCClientBase : public ClientBase, public BytesCommunicator {
public:
  IPCClientBase(IPCType type);
  ~IPCClientBase() override;

  bool init(const Address &serverAddress,
            long long sersverMonitoringIntervalMs = 1000) override;

  bool deinit() override;

  ActionCallStatus sendMessageToServer(const CSMessagePtr &msg) override;

  void onServerStatusChanged(Availability oldStatus,
                             Availability newStatus) noexcept override;

protected:
  void monitorServerStatus(long long intervalMs);
  std::thread serverMonitorThread_;
  std::unique_ptr<SyncTimer> serverMonitorTimer_;
};

} // namespace ipc
} // namespace messaging
} // namespace maf

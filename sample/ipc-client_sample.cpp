//#include <maf/LocalIPCProxy.h>
//#include <maf/utils/TimeMeasurement.h>

//#include <iostream>

//#include "Client.h"

// using namespace maf;

// static const auto DataTransmissionServerAddress =
//    Address{SERVER_NAME, WEATHER_SERVER_PORT};

// int main() {
//  std::cout.sync_with_stdio(false);
//  logging::init(
//      logging::LOG_LEVEL_FROM_INFO | maf::logging::LOG_LEVEL_DEBUG /*|
//                                     maf::logging::LOG_LEVEL_VERBOSE*/
//      ,
//      [](const std::string &msg) { std::cout << msg << std::endl; },
//      [](const std::string &msg) { std::cerr << msg << std::endl; });

//  util::TimeMeasurement tmeasure([](auto time) {
//    std::cout << "Total execution time = "
//              << static_cast<double>(time.count()) / 1000 << "ms" <<
//              std::endl;
//  });

//  MAF_LOGGER_DEBUG("Client is starting up!");

//  auto clientComponent = ClientComponent{
//      localipc::createProxy(DataTransmissionServerAddress,
//      SID_WeatherService)};

//  clientComponent.onMessage<EndOfRequestChainMsg>(
//      [](auto) { this_component::stop(); });

//  clientComponent.run();

//  MAF_LOGGER_DEBUG("Client shutdown!");

//  return 0;
//}

#include <Windows.h>
#include <conio.h>
#include <maf/logging/Logger.h>
#include <maf/messaging/client-server/Address.h>
#include <maf/messaging/client-server/CSStatus.h>
#include <maf/utils/Process.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#pragma comment(lib, "user32.lib")

#define BUF_SIZE sizeof(int)
TCHAR szName[] = TEXT("Local\\helloworld");

#include <iostream>

namespace maf {
namespace messaging {
namespace ipc {
namespace local {

using SMPath = std::string;
using SMHandle = HANDLE;
using SMBuffer = LPTSTR;
using SMData = util::process::ProcessID;
inline constexpr auto InvalidSMHandle = nullptr;

inline static SMPath constructSMPath(const Address& addr) {
  return "Local\\nocpes.github.com." + addr.get_name();
}

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
int main() {
    using namespace maf::messaging;
    using namespace maf::messaging::ipc::local;
    LocalIPCReceiverStatusMonitor m;
    m.init(maf::messaging::Address{"hello.world"});
    auto currentStatus = Availability::Unavailable;
    do
    {
        auto newStatus = m.getCurrentStatus();
        if(newStatus != currentStatus) {
            currentStatus = newStatus;
            std::cout << "new status = " << newStatus << std::endl;
        }
        _sleep(500);
    } while(true);
    return 0;
}

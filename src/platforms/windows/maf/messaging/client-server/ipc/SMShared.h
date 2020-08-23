#pragma once

#include <maf/messaging/client-server/Address.h>
#include <Windows.h>
#include <maf/logging/Logger.h>
#include <maf/utils/Process.h>

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
  return "Local\\nocpes.github.com\\" + addr.get_name();
}

}  // namespace local
}  // namespace ipc
}  // namespace messaging
}  // namespace maf

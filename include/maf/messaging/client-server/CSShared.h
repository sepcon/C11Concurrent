#pragma once

#include <maf/messaging/client-server/CSMessage.h>
#include <maf/messaging/client-server/CSStatus.h>

#include <functional>

namespace maf {
namespace messaging {

using CSPayloadProcessCallback = std::function<bool(const CSPayloadIFPtr &)>;

}  // namespace messaging
}  // namespace maf

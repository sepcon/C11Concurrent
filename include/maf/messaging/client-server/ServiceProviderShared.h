#pragma once

#include <functional>

#include "CSMessage.h"
#include "CSStatus.h"

namespace maf {
namespace messaging {

class RequestIF;

using AbortRequestCallback = std::function<void(void)>;
using RequestHandlerFunction =
    std::function<void(const std::shared_ptr<RequestIF> &)>;

}  // namespace messaging
}  // namespace maf

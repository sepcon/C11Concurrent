#pragma once

#include <maf/export/MafExport_global.h>
#include <maf/patterns/Patterns.h>

#include <memory>

#include "CSShared.h"

namespace maf {
namespace messaging {

class ServiceRequesterIF;
class ServiceProviderIF;

namespace csmgmt {

MAF_EXPORT std::shared_ptr<ServiceRequesterIF> getServiceRequester(
    const ConnectionType &conntype, const Address &serverAddr,
    const ServiceID &sid) noexcept;
MAF_EXPORT std::shared_ptr<ServiceProviderIF> getServiceProvider(
    const ConnectionType &conntype, const Address &serverAddr,
    const ServiceID &sid) noexcept;

}  // namespace csmgmt
}  // namespace messaging
}  // namespace maf

#pragma once

#include "ServiceProviderShared.h"

namespace maf {
namespace messaging {

class MAF_EXPORT RequestIF {
 public:
  virtual ~RequestIF() = default;
  virtual OpCode getOperationCode() const = 0;
  virtual const OpID& getOperationID() const = 0;
  virtual RequestID getRequestID() const = 0;
  virtual bool valid() const = 0;
  virtual ActionCallStatus respond(const CSPayloadIFPtr& answer) = 0;
  virtual ActionCallStatus reply(const CSPayloadIFPtr& answer) = 0;
  virtual CSPayloadIFPtr getInput() = 0;
  virtual void onAborted(AbortRequestCallback abortCallback) = 0;
  virtual bool serialize(std::ostream& /*os*/) { return false; }
  virtual bool deserialize(std::istream& /*is*/) { return false; }
};

}  // namespace messaging
}  // namespace maf

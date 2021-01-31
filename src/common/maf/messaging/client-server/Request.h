#pragma once

#include <maf/messaging/client-server/CSShared.h>
#include <maf/messaging/client-server/RequestIF.h>
#include <maf/patterns/Patterns.h>

#include <mutex>

namespace maf {
namespace messaging {

class ServiceProviderIF;

class Request : public RequestIF {
 public:
  std::shared_ptr<CSMessage> _csMsg;
  std::weak_ptr<ServiceProviderIF> _svStub;
  AbortRequestCallback _abortCallback;
  mutable std::mutex _mutex;
  bool _valid;

  Request(std::shared_ptr<CSMessage> csMsg,
          std::weak_ptr<ServiceProviderIF> svStub);
  bool invalidate();
  AbortRequestCallback getAbortCallback();
  void setOperationCode(OpCode opCode);

  ActionCallStatus respond_(
      const CSPayloadIFPtr &response,
      ActionCallStatus (ServiceProviderIF::*respondMethod)(
          const CSMessagePtr &csMsg));

 public:
  OpCode getOperationCode() const override;
  const OpID &getOperationID() const override;
  RequestID getRequestID() const override;
  bool valid() const override;
  ActionCallStatus respond(const CSPayloadIFPtr &response) override;
  ActionCallStatus reply(const CSPayloadIFPtr &response) override;
  CSPayloadIFPtr getInput() override;
  void onAborted(AbortRequestCallback abortCallback) override;
};

}  // namespace messaging
}  // namespace maf

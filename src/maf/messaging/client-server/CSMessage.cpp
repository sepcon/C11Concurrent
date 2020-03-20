#include <maf/messaging/client-server/CSMessage.h>

namespace maf {
namespace messaging {

CSMessage::CSMessage(ServiceID sid, OpID opID, OpCode opCode, RequestID reqID,
                     CSMsgContentBasePtr msgContent, Address sourceAddr)
    : _serviceID(std::move(sid)), _operationID(std::move(opID)),
      _requestID(std::move(reqID)), _operationCode(std::move(opCode)),
      _content(std::move(msgContent)), _sourceAddress(std::move(sourceAddr)) {}

CSMessage::~CSMessage() {}

const ServiceID &CSMessage::serviceID() const { return _serviceID; }

void CSMessage::setServiceID(ServiceID serviceID) {
  _serviceID = std::move(serviceID);
}

const OpID &CSMessage::operationID() const { return _operationID; }

void CSMessage::setOperationID(OpID operationID) {
  _operationID = std::move(operationID);
}

OpCode CSMessage::operationCode() const { return _operationCode; }

void CSMessage::setOperationCode(OpCode operationCode) {
  _operationCode = std::move(operationCode);
}

RequestID CSMessage::requestID() const { return _requestID; }

void CSMessage::setRequestID(RequestID requestID) {
  _requestID = std::move(requestID);
}

const Address &CSMessage::sourceAddress() const { return _sourceAddress; }

void CSMessage::setSourceAddress(Address sourceAddress) {
  _sourceAddress = std::move(sourceAddress);
}

CSMsgContentBasePtr CSMessage::content() const { return _content; }

void CSMessage::setContent(CSMsgContentBasePtr content) {
  _content = std::move(content);
}

// Section of CSMessageContentBase
CSMessageContentBase::~CSMessageContentBase() = default;

CSMessageContentBase::Type CSMessageContentBase::type() const { return _type; }

void CSMessageContentBase::setType(Type t) { _type = std::move(t); }

} // namespace messaging
} // namespace maf

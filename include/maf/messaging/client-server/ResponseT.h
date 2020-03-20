#pragma once

#include "CSError.h"
#include <variant>

namespace maf {
namespace messaging {

template <class CSParam> class ResponseT {

public:
  using OutputPtr = std::shared_ptr<CSParam>;
  using ErrorPtr = std::shared_ptr<CSError>;

  template <typename T,
            std::enable_if_t<std::is_same_v<CSParam, T>, bool> = true>
  ResponseT(std::shared_ptr<T> response = {})
      : _response(std::move(response)) {}

  ResponseT(std::shared_ptr<CSError> error) : _response(std::move(error)) {}

  OutputPtr getOutput() {
    return isOutput() ? std::get<OutputPtr>(_response) : nullptr;
  }
  ErrorPtr getError() {
    return isError() ? std::get<ErrorPtr>(_response) : nullptr;
  }
  bool isError() const { return std::holds_alternative<ErrorPtr>(_response); }
  bool isOutput() const { return std::holds_alternative<OutputPtr>(_response); }
  operator bool() const { return isOutput(); }

private:
  std::variant<std::shared_ptr<CSParam>, std::shared_ptr<CSError>> _response;
};

} // namespace messaging
} // namespace maf

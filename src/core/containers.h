#pragma once

#include "string.h"

#include <utility>

// Represent an error
class RError {
public:
  RError() = default;
  explicit RError(const r_string &msg) : msg_{std::move(msg)} {}

  // Returns the error message
  const r_string msg() { return msg_; }

private:
  r_string msg_;
};

// Handle's the result of an execution with a return value.
// If everything went fine it holds the result, an error otherwise
template <typename ResType> class RResult {
public:
  RResult() = default;
  explicit RResult(const ResType &res) : res_(res) { success_ = true; }
  explicit RResult(const RError &err) : err_(err) { success_ = false; }

  // Create a result holding a valid value
  static RResult<ResType> create_ok(const ResType &res) {
    return RResult<ResType>{res};
  }

  // Create a result holding an error
  static RResult<ResType> create_err(const RError &err) {
    return RResult<ResType>{err};
  }

  // Get the result valutruee
  // NOTE: It is suggested to handle eventual errors before getting the result
  // value. (using ok() and err() method)
  ResType &val() { return res_; }

  // Get the result error
  RError &err() { return err_; }

  // Whether if the result is success or an error
  const bool ok() const { return success_; }

private:
  ResType res_;
  RError err_;
  bool success_;
};

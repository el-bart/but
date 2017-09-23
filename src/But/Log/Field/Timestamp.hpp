#pragma once
#include <string>
#include <chrono>
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct Timestamp final
{
  using Clock = std::chrono::high_resolution_clock;
  Timestamp(): tp_{ Clock::now() } { }
  explicit Timestamp(const Clock::time_point tp): tp_{tp} { }

  auto timePoint() const { return tp_; }

private:
  Clock::time_point tp_;
};


inline std::string toType(Timestamp const&) { return "But::Timestamp"; }
Backend::Value toValue(Timestamp uts);

}
}
}

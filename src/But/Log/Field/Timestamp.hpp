#pragma once
#include <string>
#include <chrono>

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


inline std::string typeString(Timestamp const&) { return "But::Timestamp"; }
std::string toString(Timestamp uts);

}
}
}

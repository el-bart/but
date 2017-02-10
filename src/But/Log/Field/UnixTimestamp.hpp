#pragma once
#include <string>
#include <chrono>

namespace But
{
namespace Log
{
namespace Field
{

struct UnixTimestamp final
{
  using Clock = std::chrono::high_resolution_clock;
  UnixTimestamp(): tp_{ Clock::now() } { }
  explicit UnixTimestamp(const Clock::time_point tp): tp_{tp} { }

  auto timePoint() const { return tp_; }

private:
  Clock::time_point tp_;
};


std::string toString(UnixTimestamp uts);

}
}
}

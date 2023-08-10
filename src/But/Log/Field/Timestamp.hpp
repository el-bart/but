#pragma once
#include <chrono>
#include <string>

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


inline constexpr auto fieldName(Timestamp const*) { return std::string_view{"But::Timestamp"}; }
double fieldValue(Timestamp const& ts);

}
}
}

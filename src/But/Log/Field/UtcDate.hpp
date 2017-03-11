#pragma once
#include <string>
#include "UtcIsoDateTime.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct UtcDate final
{
  UtcDate() = default;
  explicit UtcDate(time_t ts): timestamp_{ts} { }
  explicit UtcDate(UtcIsoDateTime ts): timestamp_{ts} { }

  UtcIsoDateTime timestamp_;
};


inline std::string typeString(UtcDate const&) { return "But::UtcDate"; }
inline std::string toString(UtcDate const& d) { return d.timestamp_.date(); }

}
}
}

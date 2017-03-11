#pragma once
#include <string>
#include "UtcIsoDateTime.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct UtcTime final
{
  UtcTime() = default;
  explicit UtcTime(time_t ts): timestamp_{ts} { }
  explicit UtcTime(UtcIsoDateTime ts): timestamp_{ts} { }

  UtcIsoDateTime timestamp_;
};


inline std::string typeString(UtcTime const&) { return "But::UtcTime"; }
inline std::string toString(UtcTime const& d) { return d.timestamp_.time(); }

}
}
}

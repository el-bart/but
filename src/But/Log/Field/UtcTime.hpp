#pragma once
#include <string>
#include "UtcIsoDateTime.hpp"
#include "But/Log/Backend/Value.hpp"

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


inline std::string toType(UtcTime const&) { return "But::UtcTime"; }
inline Backend::Value toValue(UtcTime const& d) { return Backend::Value{ d.timestamp_.time() }; }

}
}
}

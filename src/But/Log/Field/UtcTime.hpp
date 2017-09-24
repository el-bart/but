#pragma once
#include "UtcIsoDateTime.hpp"
#include "But/Log/Backend/Type.hpp"
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


inline auto toType(UtcTime const&) { return Backend::Type{"But::UtcTime"}; }
inline auto toValue(UtcTime const& d) { return Backend::Value{ d.timestamp_.time() }; }

}
}
}

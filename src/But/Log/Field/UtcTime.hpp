#pragma once
#include <But/Log/Field/UtcIsoDateTime.hpp>
#include <string>

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


inline auto fieldName(UtcTime const*) { return std::string_view{"But::UtcTime"}; }
inline auto fieldValue(const UtcTime uts) { return uts.timestamp_.time(); }

}
}
}

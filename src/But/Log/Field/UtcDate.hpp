#pragma once
#include <But/Log/Field/UtcIsoDateTime.hpp>
#include <string>

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


inline auto fieldName(UtcDate const*) { return std::string_view{"But::UtcDate"}; }
inline auto fieldValue(UtcDate const& d) { return d.timestamp_.date(); }

}
}
}

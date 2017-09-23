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

struct UtcDate final
{
  UtcDate() = default;
  explicit UtcDate(time_t ts): timestamp_{ts} { }
  explicit UtcDate(UtcIsoDateTime ts): timestamp_{ts} { }

  UtcIsoDateTime timestamp_;
};


inline std::string toType(UtcDate const&) { return "But::UtcDate"; }
inline Backend::Value toValue(UtcDate const& d) { return Backend::Value{ d.timestamp_.date() }; }

}
}
}

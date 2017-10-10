#pragma once
#include "UtcIsoDateTime.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

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


inline auto toFieldInfo(const UtcTime uts) { return Backend::FieldInfo{ Backend::Tag{"But::UtcTime"}, Backend::Value{ uts.timestamp_.time() } }; }

}
}
}

#pragma once
#include "UtcIsoDateTime.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

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


inline auto toFieldInfo(UtcDate const& d) { return Backend::FieldInfo{ Backend::Tag{"But::UtcDate"}, Backend::Value{ d.timestamp_.date() } }; }

}
}
}

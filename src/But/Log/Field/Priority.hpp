#pragma once
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

enum class Priority
{
  debug   = 0,
  info    = 1,
  warning = 2,
  error   = 3
};

inline auto toType(Priority const&) { return Backend::Type{"But::Priority"}; }
Backend::Value const& toValue(Priority pri);
Backend::Value const& toValueConstLen(Priority pri);

}
}
}

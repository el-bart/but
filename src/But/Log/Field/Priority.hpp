#pragma once
#include <string>
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

inline std::string toType(Priority const&) { return "But::Priority"; }
Backend::Value const& toValue(Priority pri);
Backend::Value const& toValueConstLen(Priority pri);

}
}
}

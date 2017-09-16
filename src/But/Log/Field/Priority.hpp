#pragma once
#include <string>

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
std::string const& toValue(Priority pri);
std::string const& toValueConstLen(Priority pri);

}
}
}

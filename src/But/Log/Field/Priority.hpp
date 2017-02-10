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

inline std::string typeString(Priority const&) { return "But::Priority"; }
std::string const& toString(Priority pri);
std::string const& toStringConstLen(Priority pri);

}
}
}

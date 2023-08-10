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

inline constexpr auto fieldName(Priority const*) { return std::string_view{"But::Priority"}; }
std::string_view fieldValue(Priority pri);

}
}
}

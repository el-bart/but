#pragma once
#include <string>

namespace But
{
namespace Log
{
namespace Field
{

enum class LogLevel
{
  debug   = 0,
  info    = 1,
  warning = 2,
  error   = 3
};

inline constexpr auto fieldName(LogLevel const*) { return std::string_view{"But::LogLevel"}; }
std::string_view fieldValue(LogLevel pri);

}
}
}

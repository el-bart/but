#pragma once

namespace But
{
namespace Log
{

enum class Priority
{
  debug,
  info,
  warning,
  error,
  fatal
};

char const* toString(const Priority pri) noexcept;

char const* toStringConstLen(const Priority pri) noexcept;

}
}

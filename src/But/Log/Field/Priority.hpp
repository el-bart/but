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

std::string const& toString(const Priority pri) noexcept;

std::string const& toStringConstLen(const Priority pri) noexcept;

}
}
}

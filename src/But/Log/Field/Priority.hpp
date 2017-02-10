#pragma once
#include <string>
#include "But/Log/Backend/FieldInfo.hpp"

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

std::string const& toString(Priority pri) noexcept;
std::string const& toStringConstLen(Priority pri) noexcept;

}
}
}

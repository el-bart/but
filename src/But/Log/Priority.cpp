#include <type_traits>
#include <stdexcept>
#include <cassert>

#include "Priority.hpp"

namespace But
{
namespace Log
{

std::string const& toString(const Priority pri) noexcept
{
  static const std::string out[] =
                    {
                      "debug",
                      "info",
                      "warning",
                      "error"
                    };
  const auto num = static_cast<unsigned>(pri);
  assert( num < std::extent<decltype(out)>::value && "unknown priority" );
  return out[num];
}


std::string const& toStringConstLen(const Priority pri) noexcept
{
  static const std::string out[] =
                    {
                      "debug  ",
                      "info   ",
                      "warning",
                      "error  "
                    };
  const auto num = static_cast<unsigned>(pri);
  assert( num < std::extent<decltype(out)>::value && "unknown priority" );
  return out[num];
}

}
}

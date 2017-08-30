#include <type_traits>
#include <stdexcept>
#include "But/assert.hpp"
#include "Priority.hpp"


namespace But
{
namespace Log
{
namespace Field
{

std::string const& toString(const Priority pri)
{
  static const std::string out[] =
                    {
                      "debug",
                      "info",
                      "warning",
                      "error"
                    };
  const auto num = static_cast<unsigned>(pri);
  BUT_ASSERT( num < std::extent<decltype(out)>::value && "unknown priority" );
  return out[num];
}


std::string const& toStringConstLen(const Priority pri)
{
  static const std::string out[] =
                    {
                      "debug  ",
                      "info   ",
                      "warning",
                      "error  "
                    };
  const auto num = static_cast<unsigned>(pri);
  BUT_ASSERT( num < std::extent<decltype(out)>::value && "unknown priority" );
  return out[num];
}

}
}
}

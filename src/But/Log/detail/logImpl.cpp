#include <iostream>
#include <cassert>

#include "logImpl.hpp"

namespace But
{
namespace Log
{
namespace detail
{

void logProxy(const Priority p, char const* file, unsigned line, char const* function, std::string msg) noexcept
{
  assert(file);
  assert(function);
  // TODO: timestamp
  // TODO: appenders should be used here, to allow reasonable control over log sink + customizations from user side
  std::cout << toStringConstLen(p) << " " << file << ":" << line << " [" << function << "]: " << msg << std::endl;
}

}
}
}

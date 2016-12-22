#include <cstdlib>
#include <cassert>

#include "Priority.hpp"

namespace But
{
namespace Log
{

char const* toString(const Priority pri) noexcept
{
  switch(pri)
  {
    case Priority::debug:   return "debug";
    case Priority::info:    return "info";
    case Priority::warning: return "warning";
    case Priority::error:   return "ERROR";
    case Priority::fatal:   return "FATAL";
  }
  assert(!"unknown priority");
  abort();
}


char const* toStringConstLen(const Priority pri) noexcept
{
  switch(pri)
  {
    case Priority::debug:   return "debug  ";
    case Priority::info:    return "info   ";
    case Priority::warning: return "warning";
    case Priority::error:   return "ERROR  ";
    case Priority::fatal:   return "FATAL  ";
  }
  assert(!"unknown priority");
  abort();
}

}
}

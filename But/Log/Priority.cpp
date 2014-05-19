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
    case Priority::DEBUG:   return "debug";
    case Priority::INFO:    return "info";
    case Priority::WARNING: return "warning";
    case Priority::ERROR:   return "ERROR";
    case Priority::FATAL:   return "FATAL";
  }
  assert(!"unknown priority");
}

}
}
